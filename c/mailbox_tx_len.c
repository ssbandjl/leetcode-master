

/* 公共部分 */
#include <stdio.h>
#include <stdlib.h>

struct mailbox_tx_desc
{
    __uint16_t flags;
    __uint16_t srcid;
    __uint16_t dstid;
    __uint16_t msg_type;
    __uint64_t buf_addr;
    __uint16_t buf_len;
    __uint16_t data_len; //20B
    __uint8_t data[44];  //1B *44 = 44B
};

// gcc -o main main.c;./main
int main(){
    printf("size:%ld\n", sizeof(struct mailbox_tx_desc));

    return 0;
}