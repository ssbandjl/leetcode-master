/*
  huge-mmap is a benchmark program which compares performance between
  accessing to huge size pages and small size pages.
  

  Here is a brief instruction on preparing hugetlb'ed page

  ex.

  0. boot with HUGETLB configured 2.6 linux kernel

  1. check hugepages
  # grep Huge /proc/meminfo
  HugePages_Total:     0
  HugePages_Free:      0
  Hugepagesize:     4096 kB

  2. set hugepages
  # echo 2 > /proc/sys/vm/nr_hugepages

  3. mount hugetlbfs
  # [ -d /mnt/huge ] || mkdir -p /home/huge
  # mount none /mnt/huge -t hugetlbfs -o rw,mode=0777

  4. run benchmark
  # make
  # ./huge-mmap


  note: 
  - hugetlb code portions come from linux/Documentation/vm/hugetlbpage.txt  
  - support x86 only now

  Kazutomo Yoshii <kazutomo@mcs.anl.gov>
*/
 

#if defined(__i386__) || defined(__x86_64__)

static __inline__ unsigned long long int rdtsc(void)
{
  unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
}
#else

#error "No tick counter is available!"

#endif  // architecture


/* from twister.c */
typedef unsigned long uint32;
void seedMT(uint32 seed);
uint32 randomMT(void);


/*
 *
 * Example of using hugepage memory in a user application using the mmap
 * system call.  Before running this application, make sure that the
 * administrator has mounted the hugetlbfs filesystem (on some directory
 * like /mnt) using the command mount -t hugetlbfs nodev /mnt. In this
 * example, the app is requesting memory of size 256MB that is backed by
 * huge pages.
 *
 * For ia64 architecture, Linux kernel reserves Region number 4 for hugepages.
 * That means the addresses starting with 0x800000... will need to be
 * specified.  Specifying a fixed address is not required on ppc64, i386
 * or x86_64.
 * 使用 mmap 系统调用在用户应用程序中使用大页内存的示例。 在运行此应用程序之前，请确保管理员已使用命令 mount -t Hugetlbfs nodev /mnt 安装了hugetlbfs 文件系统（在 /mnt 等目录上）。 在此示例中，应用程序请求大小为 256MB 的内存，该内存由大页面支持。 对于 ia64 架构，Linux 内核为大页保留区域号 4。 这意味着需要指定以 0x800000... 开头的地址。 ppc64、i386 或 x86_64 上不需要指定固定地址
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>
#include <time.h>


static unsigned long  LENGTH = (4UL*1024*1024);

#define FILE_NAME "/mnt/huge/page"

#define PROTECTION (PROT_READ | PROT_WRITE)

/* Only ia64 requires this */
#ifdef __ia64__
#define ADDR (void *)(0x8000000000000000UL)
#define FLAGS (MAP_SHARED | MAP_FIXED)
#else
#define ADDR (void *)(0x0UL)
#define FLAGS (MAP_SHARED)
#endif



static uint64_t bench_hugepage( uint64_t (*bench)( double* ptr))
{
  uint64_t t = 0;
  void *addr;
  int fd;

  fd = open(FILE_NAME, O_CREAT | O_RDWR, 0755);
  if (fd < 0) {
    perror("open");
    exit(1);
  }

  addr = mmap(ADDR, LENGTH, PROTECTION, FLAGS, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    unlink(FILE_NAME);
    exit(1);
  }

  t = bench(addr);

  munmap(addr, LENGTH);
  close(fd);
  unlink(FILE_NAME);

  return t;
}


static uint64_t bench_smallpage( uint64_t (*bench)( double* ptr))
{
  uint64_t t = 0;
  void* addr;

  addr = malloc( LENGTH );
  if( ! addr ) {
    perror( addr );
    exit(1);
  }
  
  t = bench(addr);

  free(addr);

  return t;
}

static int BENCH_LOOP_CNT;

static int dcomp( const void *a, const void *b )
{
  if( *((double*)a) == *((double*)b) ) return 0;
  else if( *((double*)a) > *((double*)b) ) return 1;
  return -1;
}


static uint64_t qsort_bench( double* array )
{
  uint64_t start;
  int i;
  int size = LENGTH/sizeof(double);

  /* set random number */
  for( i=0; i<size; i++ ) {
    array[i] = (double)randomMT();
  }

  start = rdtsc();
  qsort(array, size, sizeof(double), dcomp );
  return rdtsc() - start;
}

static uint64_t memtest_bench( double* array )
{
  int i,a1,a2,a3;
  uint64_t start;
  int size = LENGTH/sizeof(double);

  start = rdtsc();
  for( i=0; i<BENCH_LOOP_CNT; i++ ) {
    a1 = randomMT() % size;				
    a2 = randomMT() % size;
    a3 = randomMT() % size;
    array[a1] = array[a2] * array[a3] + (double)a1;
  }

  return rdtsc() - start;
}

uint64_t get_cpufreq_HZ()
{
  char buf[1024];
  double freq;
  FILE* fp = fopen("/proc/cpuinfo", "r");
  if( !fp ) {
    perror("fopen");
    exit(1);
  }
  while(fgets(buf, sizeof(buf), fp ) != (char*)NULL ) {
    if( strncmp( buf, "cpu MHz", 7 ) == 0 ) {
      char* p;
      p = buf;
      while( *p && (!isdigit(*p)) ) p++;
      freq = atof(p);
      break;
    }
  }

  fclose(fp);

  return (uint64_t)(freq*1000*1000);
}


int main(int argc, char* argv[])
{
  int i;
  uint64_t t1,t2;
  uint64_t cpufreq = get_cpufreq_HZ();

  if( argc < 2 ) {
    printf("Usage: %s niter [allocsize_MB]\n", argv[0]);
    printf("allocsize is 4MB by default\n");
    return 1;
  }

  BENCH_LOOP_CNT = atoi( argv[1] );
  if( BENCH_LOOP_CNT <=0 ) { 
    printf("please enter valid value [0, 2^32)\n");
    return 0;
  }

  if( argc > 2 ) {
    LENGTH = (unsigned long)(1024* 1024 * atoi( argv[2] ) );
  }
  if( LENGTH <=0 ) { 
    printf("please enter valid value [0, PHYCONT_MB)\n");
    return 0;
  }


  seedMT( (uint32)time(NULL) );

  printf("# BENCH_LOOP_CNT=%d\n", BENCH_LOOP_CNT);
  printf("# LENGTH=%lu MB\n", LENGTH>>20);
  printf("# small_page=4KB in x86\n");
  printf("# huge_page=4MB in x86\n");

  printf("# try small_page huge_page\n");


  for( i=0; i<10; i++ ) {
    t1 = bench_smallpage( qsort_bench );
    t2 = bench_hugepage( qsort_bench );
    printf("%d %f %f\n",
	   i, 
	   (double)t1/(double)cpufreq,
	   (double)t2/(double)cpufreq );
  }

  return 0;
}