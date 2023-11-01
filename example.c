#include <stdio.h>
#include <stdlib.h>


// gcc -o main example.c;./main
int main(){
  int size=0;
  size = (115 + 7) & ~0x7;
  printf("size:%d\n", size);
  return 0;
}