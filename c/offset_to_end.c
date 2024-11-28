

#include <stdio.h>
#include <stdlib.h>

#include <linux/stddef.h>

typedef unsigned char		    uint8_t;
typedef unsigned short int	    uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	    uint64_t;
typedef unsigned long long      khint64_t;

struct irdma_resize_cq_req {
	uint64_t user_cq_buffer;
};

// gcc -o main main.c;./main
int main(){
    uint64_t offset_cq = 0;
    
    offset_cq = offsetofend(struct irdma_resize_cq_req, user_cq_buffer);

    printf("offset_cq:%lu\n", offset_cq);

    return 0;
}