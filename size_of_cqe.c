

#include <stdio.h>
#include <stdlib.h>

#include <linux/stddef.h>

typedef unsigned char		    uint8_t;
typedef unsigned short int	    uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	    uint64_t;
typedef unsigned long long      khint64_t;

typedef unsigned long long __u64;

struct irdma_cqe {
	__u64 buf[4];
};
// gcc -o main main.c;./main
int main(){

    printf("sizeof_buf:%lu\n", sizeof(struct irdma_cqe));

    return 0;
}