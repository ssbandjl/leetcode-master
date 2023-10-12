#include <stdio.h>
#include <stdlib.h>

// gcc -o main -g scanf.c;./main
int main(){

  char word[128];
  printf("input:");
  scanf("%s", word);
  printf("output:%s\n", word);
  return 0;
}