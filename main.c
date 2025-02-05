

#include <stdio.h>
#include <stdlib.h>

#include <linux/stddef.h>

typedef unsigned char		    uint8_t;
typedef unsigned short int	    uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	    uint64_t;
typedef unsigned long long      khint64_t;

typedef unsigned long long __u64;


#define XTRDMA_GRM_HMC_SD_ID_SPEC 2048

// gcc -o main main.c;./main
int main(){

    printf("%d\n", BITS_TO_LONGS(XTRDMA_GRM_HMC_SD_ID_SPEC));

    return 0;
}

