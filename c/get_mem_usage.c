#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <glob.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE (sysconf(_SC_PAGESIZE))
#endif

#define ONE_K 1024L
#define PAGE_SIZE_KB (PAGE_SIZE / ONE_K)

#define MAX_NAME (128)
#define PROC_LINE_LENGTH (512)

int main()
{
   int i = 0; 
   char filename[MAX_NAME + 1] = {0};
   char buf[PROC_LINE_LENGTH + 1] = {0};
   unsigned long totalMem = 1; 

   struct sysinfo info;
   int ret = sysinfo(&info); 
   if (ret != -1) 
   {
        //也可以读取 /proc/meminfo
        totalMem = info.totalram / 1024L; // 转换为 KB
   }
    
   char pattern[MAX_NAME + 1] = {0}; 
   snprintf(pattern, MAX_NAME, "/proc/[0-9]*");

   glob_t pglob; 
   ret = glob(pattern, 0, NULL, &pglob);
   if (ret != 0) 
   {
       perror("glob error!");
       return -1;
   } 

   for (i = 0; i < pglob.gl_pathc; i++) {
       long pid = atol(pglob.gl_pathv[i] + 6); // "/proc/<pid>"
       long m_size = 0, m_resident = 0, m_share = 0, m_trs = 0, m_lrs = 0, m_drs = 0, m_dt = 0;

       snprintf(filename, MAX_NAME, "/proc/%ld/statm", pid); 
       FILE *fp = fopen(filename, "r");
       if (fp && fgets(buf, PROC_LINE_LENGTH, fp) != NULL) 
       {
           int n = sscanf(buf, "%ld %ld %ld %ld %ld %ld %ld", &m_size, &m_resident, &m_share, &m_trs, &m_lrs, &m_drs, &m_dt);

           /* 进程 内存占用率 */ 
           float percent_mem = (m_resident * PAGE_SIZE_KB) / (double)(totalMem) * 100.0;
           printf("pid:<%ld>, percent_mem: %.1f\n", pid, percent_mem);
           fclose(fp);
           fp = NULL;
       }

       if (fp)
           fclose(fp);
   } 
    
   globfree(&pglob);
   return 0;
}