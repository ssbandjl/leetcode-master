#include <stdio.h>
#include <stdlib.h>



// gcc -o main main.c;./main
int main(){
  char *tmp_str;
  int i;
  int size = 16;

  tmp_str = calloc(1, size);

  for (i = 0; i < (size - 1); ++i) {
      tmp_str[i] = 'A' + (i % 26);
      printf("i:%d, s:%c\n", i, tmp_str[i]);
  }
  printf("%s\n", tmp_str);
  return 0;
}

