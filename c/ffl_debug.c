#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdarg.h>

#define FFL_FMT              "%s() %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__
static FILE *verbs_log_fp;

void __verbs_log(
		 const char *fmt, ...)
{
    verbs_log_fp = stderr;
	va_list args;
    va_start(args, fmt);
    vfprintf(verbs_log_fp, fmt, args);
    va_end(args);
}

#define debug(format, arg...)                                  \
do {                                                                           \
	__verbs_log(format " %s() %s:%d\n" ,                          \
		      ##arg, __FUNCTION__, __FILE__, __LINE__);   \
} while (0)


//计算虚拟地址对应的地址，传入虚拟地址vaddr，通过paddr传出物理地址
void mem_addr(unsigned long vaddr, unsigned long *paddr)
{
	int pageSize = getpagesize();//调用此函数获取系统设定的页面大小

	unsigned long v_pageIndex = vaddr / pageSize;//计算此虚拟地址相对于0x0的经过的页面数
	unsigned long v_offset = v_pageIndex * sizeof(uint64_t);//计算在/proc/pid/page_map文件中的偏移量
	unsigned long page_offset = vaddr % pageSize;//计算虚拟地址在页面中的偏移量
	uint64_t item = 0;//存储对应项的值

	int fd = open("/proc/self/pagemap", O_RDONLY); //以只读方式打开/proc/pid/page_map
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
		debug("page present is 0\n");
		return ;
	}

	uint64_t phy_pageIndex = (((uint64_t)1 << 55) - 1) & item;//计算物理页号，即取item的bit0-54

	*paddr = (phy_pageIndex * pageSize) + page_offset;//再加上页内偏移量就得到了物理地址
}

const int a = 100;//全局常量

//  rm -f main;gcc -g -Og -o main main.c;./main
int main()
{
	int b = 100;//局部变量
	static int c = 100;//局部静态变量
	const int d = 100;//局部常量
	char *str = "Hello World!";

	unsigned long phy = 0;//物理地址

	char *p = (char*)malloc(100);//动态内存
	
	int pid = fork();//创建子进程
	if(pid == 0)
	{
		//p[0] = '1';//子进程中修改动态内存
		mem_addr((unsigned long)&a, &phy);
		debug("0 pid = %d, virtual addr = 0x%x , physical addr = 0x%lx", getpid(), &a, phy);
	}
	else
	{ 
		mem_addr((unsigned long)&a, &phy);
		debug("1 pid = %d, virtual addr = 0x%x , physical addr = 0x%lx", getpid(), &a, phy);
	}

	sleep(100);
	free(p);
	// waitpid();
	return 0;
}

