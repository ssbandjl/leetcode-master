

/* 公共部分 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>


#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

static void log_report(const char *fmt, ...)
{
	char buf[128];
	va_list args;
	struct timeval tv;
	struct tm *tm;

	(void)gettimeofday(&tv, 0);
	tm = localtime(&tv.tv_sec);

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	printf("%02d/%02d/%02d-%02d:%02d:%02d.%02ld %s", 
		tm->tm_year + 1900,
		tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec,
		(long int)tv.tv_usec / 10000,
		buf);
}

#define debug(format, arg...)                                  \
do {                                                                           \
	log_report(format " %s() %s:%d\n" ,                          \
		      ##arg, __FUNCTION__, __FILENAME__, __LINE__);   \
} while (0)

// gcc -o main example.c;./main
int main(){
    int err = 1;

	struct timeval tv;
	struct tm *tm;

	(void)gettimeofday(&tv, 0);
	tm = localtime(&tv.tv_sec);
	printf("%02d/%02d/%02d-%02d:%02d:%02d.%02ld\n",
			 tm->tm_year + 1900,
			 tm->tm_mon + 1, tm->tm_mday,
			 tm->tm_hour, tm->tm_min, tm->tm_sec,
			 (long int)tv.tv_usec / 10000);
    debug("ffl %d", err);

    return 0;
}
