

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char		    uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	  uint64_t;
typedef unsigned long long  khint64_t;

#define EMU_MAX_STR_LEN_PATH 256
#define EMU_MAX_STR_LEN_PATH 256

/* size:524 = 512 + 2 + 2 + 4 + 4 */
struct virtio_blk_params
{
    uint16_t id;    // 2
    uint8_t vblk_name[EMU_MAX_STR_LEN_PATH];    // 256
    uint8_t bdev_name[EMU_MAX_STR_LEN_PATH];    // 256
    uint16_t num_queue; //2
    uint32_t total_size;    //4
    uint32_t block_size;    //4
};

// gcc -o main main.c;./main
int main(){
    printf("size:%ld\n", sizeof(struct virtio_blk_params));

    return 0;
}