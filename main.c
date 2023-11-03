
#include <string.h>

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


static uint32_t obj_id_gen	= 1000;
typedef struct {
	/** least significant (low) bits of object ID */
	uint64_t	lo;
	/** most significant (high) bits of object ID */
	uint64_t	hi;
} daos_obj_id_t;

daos_obj_id_t
dts_oid_gen(unsigned seed)
{
	daos_obj_id_t	oid;
	uint64_t	hdr;

	hdr = seed;
	hdr <<= 32;

	/* generate a unique and not scary long object ID */
	oid.lo	= obj_id_gen++;
	oid.lo	|= hdr;
	oid.hi	= rand() % 100;


	return oid;
}


// gcc -o main example.c;./main
int main(){
    daos_obj_id_t	oid;
    oid = dts_oid_gen(1111);
    DEBUG("oid.lo:%lu", oid.lo);
    DEBUG("oid.hi:%lu", oid.hi);

    char data[16];
    memset(data, 'a', 16);
    DEBUG("data:%s", data);
    return 0;
}