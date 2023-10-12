#include <stdio.h>
#include <stdlib.h>

typedef unsigned int		uint32_t;


static inline uint32_t
tse_task_buf_size(int size)
{
	return (size + 7) & ~0x7; // 8字节对齐?
}


0000 0000

0000 0111  0x7
1111 1000 ~0x7





int main(){
  int size;
  size = tse_task_buf_size(100);
  printf("size:%d\n", size);
}