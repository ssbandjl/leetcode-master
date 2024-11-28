

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char		    uint8_t;
typedef unsigned short int	    uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	    uint64_t;
typedef unsigned long long      khint64_t;


#define BLK_SECTOR_SIZE_SHIFT 9

// gcc -o main main.c;./main
int main(){
    uint64_t capacity = 0;
    uint32_t num_blocks = 488280064;
    uint32_t block_size = 512;

    capacity = ((uint64_t)num_blocks * (uint64_t)block_size) >> BLK_SECTOR_SIZE_SHIFT;
    // capacity = 488280064;
    printf("Blk capacity:%lu, num_blocks:%u, block_size:%u %s() %s:%d\n",
        capacity, num_blocks, block_size, __FUNCTION__, __FILE__, __LINE__);
    return 0;
}