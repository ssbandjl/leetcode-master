#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/utsname.h>

#define LINUX_VERSION(x,y,z) (0x10000*(x) + 0x100*(y) + z)
#define MIN(x,y) ((x) < (y) ? (x) : (y))

int linux_version(void)
{
    int linux_version_code = 0;

    int x = 0, y = 0, z = 0;

    static struct utsname uts;
    if (uname(&uts) == -1)
        exit(1);

    (void)sscanf(uts.release, "%d.%d.%d", &x, &y, &z);
    
    linux_version_code = LINUX_VERSION(x,y,z); 
    
    return linux_version_code;
}


unsigned long get_min_free_kbytes(void)
{
    char buffer[1024] = {0};
    char *tail = NULL;

    FILE *fp = fopen("/proc/sys/vm/min_free_kbytes", "r");
    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    unsigned long kb_min_free = (unsigned long) strtoull(buffer, &tail, 10);

    return kb_min_free;
}

int main()
{
    FILE *fp = fopen("/proc/meminfo", "r"); 

    unsigned long kb_page_cache = 0;
    unsigned long kb_main_used = 0;
    unsigned long kb_main_free = 0;
    unsigned long kb_main_total = 0;
    unsigned long kb_main_buffers = 0;
    unsigned long kb_main_available = 0;
    unsigned long kb_slab_reclaimable = 0; 
    unsigned long kb_inactive_file = 0;
    unsigned long kb_active_file = 0;

    char buffer[1024] = {0};
    
    while (fgets(buffer, sizeof(buffer), fp)) {
       unsigned long *val = NULL; 
       if (strncasecmp(buffer, "MemTotal:", 9) == 0) {
           val = &kb_main_total;
       } else if (strncasecmp(buffer, "MemFree:", 8) == 0) {
           val = &kb_main_free;
       } else if (strncasecmp(buffer, "Buffers:", 8) == 0) {
           val = &kb_main_buffers;
       } else if (strncasecmp(buffer, "Cached:", 7) == 0) {
           val = &kb_page_cache;
       } else if (strncasecmp(buffer, "MemAvailable:", 13) == 0) {
           val = &kb_main_available;
       } else if (strncasecmp(buffer, "SReclaimable:", 13) == 0) {
           val = &kb_slab_reclaimable;
       } else if (strncasecmp(buffer, "Inactive(file):", 15) == 0) {
           val = &kb_inactive_file;
       } else if (strncasecmp(buffer, "Active(file):", 13) == 0) {
           val = &kb_active_file;
       } else {
           continue;
       }

       sscanf(buffer, "%*s %lu", val); 
    }

    fclose(fp);

    unsigned long kb_main_cached = kb_page_cache + kb_slab_reclaimable;
    kb_main_used = kb_main_total - kb_main_free - kb_main_cached - kb_main_buffers;
    
    if (!kb_main_available) {
        if (linux_version() < LINUX_VERSION(2, 6, 27))
            kb_main_available = kb_main_free;
        else {
            unsigned long kb_min_free = get_min_free_kbytes(); 

            unsigned long watermark_low = kb_min_free * 5 / 4;

            signed long mem_available = (signed long)kb_main_free - watermark_low
                + kb_inactive_file + kb_active_file - MIN((kb_inactive_file + kb_active_file) / 2, watermark_low)
                + kb_slab_reclaimable - MIN(kb_slab_reclaimable / 2, watermark_low);
            
            if (mem_available < 0) mem_available = 0;
                kb_main_available = (unsigned long) mem_available;
        }

        if (kb_main_available != 0)
            kb_main_used = kb_main_total - kb_main_available;
    }
    
    printf("total: %lu\n", kb_main_total);
    printf("used:  %lu\n", kb_main_used);
    printf("free:  %lu\n", kb_main_free);

    return 0;
}