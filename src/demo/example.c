#include <stdio.h>
#include <stdlib.h>

int main(){
  // unsigned char cmnd[321];
  // printf("cmnd_len:%u\n", sizeof(cmnd));
  // unsigned char cmnd[32]; /* SCSI CDB */
  // cmnd[6] = (unsigned char)0xc0;
  // // printf("cmnd_len:%u\n", sizeof(cmnd));
  // printf("reserverd:%lu\n", cmnd[6]);
  int a = 0;
  a |=(1<<6);
  a |=(1<<7);
  printf("a:%d\n", a);
  return 0;
}