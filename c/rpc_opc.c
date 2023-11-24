

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

enum {
    READ = 11111,
    WRITE,
    OPEN,
};

#define HOS_RPC_MOD_ID 0x0e
#define HOS_RPC_ID(index) ((HOS_RPC_MOD_ID << 16) | (0xFFFF & index))
#define HOS_RPC_ID_TO_OPC(rpcid) (rpcid & 0xFFFF)

/* 
1 -> index
1110 -> 0x0e
1110 0000 0000 0000 0000 -> HOS_RPC_MOD_ID << 16

     1111 1111 1111 1111 -> 0xFFFF
                       1 -> index
     0000 0000 0000 0001 -> 0xFFFF & index)

1110 0000 0000 0000 0001 -> (HOS_RPC_MOD_ID << 16) | (0xFFFF & index)

revers:
0000 1111 1111 1111 1111

*/


// gcc -o main example.c;./main
int main(){
    uint32_t rpc_id = (uint32_t)HOS_RPC_ID(WRITE);
    printf("%u\n", rpc_id);
    printf("%u\n", HOS_RPC_ID(WRITE));
    printf("%u\n", HOS_RPC_ID(2));
    // printf("%u\n", HOS_RPC_MOD_ID);
    printf("%u\n", HOS_RPC_ID_TO_OPC(rpc_id));
    // printf("%u\n", HOS_RPC_ID_TO_OPC(917506));
    return 0;
}