#include <stdio.h>
#include <stdlib.h>



static size_t min_size(size_t s1, size_t s2)
{
	return s1 < s2 ? s1 : s2;
}

// gcc -o main example.c;./main
int main(){
  printf("UCT_IB_MAX_IOV:%lu\n", UCT_IB_MAX_IOV);
  return 0;
}