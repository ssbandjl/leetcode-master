#include <stdio.h>
#include <stdlib.h>

#define FFL_FMT              "%s %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__


// gcc -o main main.c;./main
int main(){
  printf(" "FFL_FMT" \n", FFL);
  return 0;
}