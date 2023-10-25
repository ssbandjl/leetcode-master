#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FFL_FMT              "%s %s:%d"
#define FFL                  __FUNCTION__,__FILE__,__LINE__

/* Unsigned.  */
typedef unsigned char		    uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	  uint64_t;
typedef unsigned long long  khint64_t;

struct media_error_msg {
	int			 mem_err_type;
	int			 mem_tgt_id;
};


#define D_ALLOC(ptr, size)	D_ALLOC_CORE(ptr, size, 1)
#define D_ALLOC_PTR(ptr)	D_ALLOC(ptr, sizeof(*ptr))

#define D_ALLOC_CORE(ptr, size, count)                          \
	do {                                                          \
		(ptr) = (__typeof__(ptr))d_calloc((count), (size));         \
	} while (0)


void *
d_calloc(size_t count, size_t eltsize)
{
	void *ptr;
	ptr = calloc(count, eltsize);
	return ptr;
}


/*

*/

// gcc -g -Og -o main main.c;./main
int main(){
  struct media_error_msg	*mem;
  D_ALLOC_PTR(mem);
  if (mem == NULL) {
    printf("err no mem "FFL_FMT"\n", FFL);
    return -1;
  }
  mem->mem_err_type = 111;
  printf("mem:%p "FFL_FMT"\n", mem, FFL);
  printf("mem->mem_err_type:%d "FFL_FMT"\n", mem->mem_err_type, FFL);



  return 0;
}