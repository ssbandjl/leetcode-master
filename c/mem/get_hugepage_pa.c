#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <inttypes.h>


#include <stdarg.h>

#define PFN_MASK_SIZE	8


/** Physical address */
typedef uint64_t phys_addr_t;
typedef uint64_t rte_iova_t;
#define RTE_BAD_IOVA ((rte_iova_t)-1)

static FILE *log_fp;
void __log(const char *fmt, ...)
{
    log_fp = stderr;
	va_list args;
    va_start(args, fmt);
    vfprintf(log_fp, fmt, args);
    va_end(args);
}
#define debug(format, arg...)                                  \
do {                                                                           \
	__log(format " %s() %s:%d\n" ,                          \
		      ##arg, __FUNCTION__, __FILE__, __LINE__);   \
} while (0)



phys_addr_t
rte_mem_virt2phy(const void *virtaddr)
{
	int fd, retval;
	uint64_t page, physaddr;
	unsigned long virt_pfn;
	int page_size;
	off_t offset;

	/* standard page size */
	page_size = getpagesize();

	fd = open("/proc/self/pagemap", O_RDONLY);
	if (fd < 0) {
		debug("cannot open /proc/self/pagemap");
		return RTE_BAD_IOVA;
	}

	virt_pfn = (unsigned long)virtaddr / page_size;
	offset = sizeof(uint64_t) * virt_pfn;
	if (lseek(fd, offset, SEEK_SET) == (off_t) -1) {
        debug("seek error in /proc/self/pagemap");
		close(fd);
		return RTE_BAD_IOVA;
	}

	retval = read(fd, &page, PFN_MASK_SIZE);
	close(fd);
	if (retval < 0) {
        debug("cannot read /proc/self/pagemap");
		return RTE_BAD_IOVA;
	} else if (retval != PFN_MASK_SIZE) {
        debug("cannot read /proc/self/pagemap");
		debug("read %d bytes from /proc/self/pagemap but expected %d",
            retval, PFN_MASK_SIZE);
		return RTE_BAD_IOVA;
	}

	/*
	 * the pfn (page frame number) are bits 0-54 (see
	 * pagemap.txt in linux Documentation)
	 */
	// debug("page 0x%" PRIx64 " ", page);
	debug("page 0x%" PRIx64 "", page);
	if ((page & 0x7fffffffffffffULL) == 0) {
		debug("not find pfn");
		return RTE_BAD_IOVA;
	}

	physaddr = ((page & 0x7fffffffffffffULL) * page_size)
		+ ((unsigned long)virtaddr % page_size);

	return physaddr;
}


//计算虚拟地址对应的地址，传入虚拟地址vaddr，通过paddr传出物理地址
void get_pa(const void *virtaddr, unsigned long *paddr)
{
	int pageSize = getpagesize();//调用此函数获取系统设定的页面大小

	unsigned long v_pageIndex = (unsigned long)virtaddr / pageSize;//计算此虚拟地址相对于0x0的经过的页面数
	unsigned long v_offset = v_pageIndex * sizeof(uint64_t);//计算在/proc/pid/page_map文件中的偏移量
	unsigned long page_offset = (unsigned long)virtaddr % pageSize;//计算虚拟地址在页面中的偏移量
	uint64_t item = 0;//存储对应项的值

	int fd = open("/proc/self/pagemap", O_RDONLY); // 以只读方式打开/proc/pid/page_map
	if(fd == -1)//判断是否打开失败
	{
		debug("open /proc/self/pagemap error\n");
		return;
	}

	if(lseek(fd, v_offset, SEEK_SET) == -1)//将游标移动到相应位置，即对应项的起始地址且判断是否移动失败
	{
		debug("sleek error\n");
		return;	
	}

	if(read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t))//读取对应项的值，并存入item中，且判断读取数据位数是否正确
	{
		debug("read item error\n");
		return;
	}

	if((((uint64_t)1 << 63) & item) == 0)//判断present是否为0
	{
		debug("page present is 0");
		return ;
	}

	uint64_t phy_pageIndex = (((uint64_t)1 << 55) - 1) & item;//计算物理页号，即取item的bit0-54

	*paddr = (phy_pageIndex * pageSize) + page_offset;//再加上页内偏移量就得到了物理地址
}

int main(int argc, char **argv){
	phys_addr_t buf_pa;
	unsigned long phy = 0;//物理地址
	static char *filepath = "/dev/hugepages/get_pa_test_file"; // touch 
	int fd;


	fd = open(filepath, O_CREAT | O_RDWR, 0600);
	if (fd < 0) {
		debug("open file failed, fd:%d", fd);
	}

    void *buf = mmap(NULL, 8 * (1 << 21), PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_POPULATE | MAP_HUGETLB , fd, 0);
    if (buf == MAP_FAILED) {
        debug("map failed, buf:%p", buf);
        return RTE_BAD_IOVA;
    }
    debug("huge_buf_va:%p", buf);
	buf_pa = rte_mem_virt2phy(buf);
	if (buf_pa == RTE_BAD_IOVA)
		debug("RTE_BAD_IOVA");
	else
		debug("[rte_mem_virt2phy] huge_buf_pa:%p", buf_pa);
	get_pa(buf, &phy);
	debug("[get_pa] huge_buf_pa:%p", buf_pa);
    return 0;
}


