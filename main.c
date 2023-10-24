#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define FFL_FMT              "%s %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__


void d_vlog(const char *fmt, va_list ap)
{
  fprintf(stdout, "%s "FFL_FMT" \n", fmt, FFL);
}

static inline void d_log(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	d_vlog(fmt, ap);
	va_end(ap);
}



#define PRINT(fmt, ...)				    \
	do {                            \
	  printf(fmt##FFL_FMT, ##__VA_ARGS__, FFL);	    \
    fprintf(stdout, fmt##FFL_FMT, fmt, FFL); \
	} while (0)


// gcc -o main main.c;./main
int main(){
  int i = 111;
  int j = 121;
  PRINT("jkwljgalwek:%d, %d", i, j);
  d_log("jkwljgalwek:%d, %d", i, j);
  return 0;
}