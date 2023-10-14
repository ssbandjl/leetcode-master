#include <stdio.h>
#include <stdlib.h>

#define FILE_LINE_STR              "%s:%d"
#define FILE_LINE                  __FILE__,__LINE__


// gcc -o main main.c;./main
int main(){
  printf(" "FILE_LINE_STR" \n", FILE_LINE);
  return 0;
}