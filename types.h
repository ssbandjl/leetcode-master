/* 公共部分 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FFL_FMT              "%s() %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__
#define NOW \
  do  { \
    time_t timer; \
    char buffer[26]; \
    struct tm* tm_info; \
    timer = time(NULL); \
    tm_info = localtime(&timer); \
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info); \
    printf("%s ", buffer); \
	} while (0)
#define DEBUG(fmt, ...) \
do { \
  NOW; \
  printf(fmt, ##__VA_ARGS__); \
  printf(" "FFL_FMT"\n", FFL); \
} while (0)

/* Unsigned.  */
typedef unsigned char		    uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	  uint64_t;
typedef unsigned long long  khint64_t;
/* 公共部分 END */

