

#include <stdio.h>
#include <stdlib.h>

#include <linux/stddef.h>

typedef unsigned char		    uint8_t;
typedef unsigned short int	    uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	    uint64_t;
typedef unsigned long long      khint64_t;

typedef unsigned long long __u64;


/* Get Low Bit */
#define MASK_LOW		((1UL << 44) -1)


// Assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

// gcc -o main main.c;./main
int main(){
    // uint8_t i = 111;
    uint64_t h = 0;
    uint64_t l = 0;
    uint64_t *addr = NULL;
    addr = calloc(1, 64);
    if (!addr) {
        printf("alloc mem failed\n");
        return -1;
    }
    // printf("addr:%p\n", addr);
    // 0000000000000000010101100011100110100100100111110011001010100000
    printBits(sizeof(*addr), &addr);
    l = ((uint64_t)(addr)) & MASK_LOW;
    // printf("h:0x%x\n", h);
    printBits(sizeof(l), &l);

    // h = i >> 4;
    // printBits(sizeof(h), &h);

    return 0;
}

