#include <execinfo.h>

static inline void print_stacktrace(void)
{
  int  size=32;
  void *array[32]={0};
  int i = 0;
  int stack_num = backtrace(array, size);
  char **stacktrace = backtrace_symbols(array, stack_num);
  printf("########## start ###########\n");
  for(; i<stack_num; ++i)
  {
    printf("QEMU %s\n",stacktrace[i]);
  }
  printf("########### end ##########\n");
  free(stacktrace);
}

