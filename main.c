#include <stdio.h>
#include <stdlib.h>

// gcc -o main main.c;./main
int main(){

  printf("%lu\n", sizeof(pthread_mutex_t));
  return 0;
}