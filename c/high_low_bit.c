#include <stdio.h>
#include <stdlib.h>

#define FILE_LINE_STR              "%s:%d"
#define FILE_LINE                  __FILE__,__LINE__


/* Unsigned.  */
typedef unsigned char		    uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		    uint32_t;
typedef unsigned long int	  uint64_t;
typedef unsigned long long  khint64_t;


// typedef struct oid {
//   uint64_t high;
//   uint32_t low;
// } oid_t;


struct {
    uint64_t high;
    uint32_t low;
} const oid = {
    .high = 0xAFFFFFFFFFFFFFFA,
    .low = 0xFFFFFFFA
};

// gcc -o main main.c;./main
int main(){
  // unsigned n = 8;
  // unsigned low8bits = n & 0xFF;

  // printf("low8bits: %d "FILE_LINE_STR" \n", low8bits, FILE_LINE);

  // uint32_t i32 = 0xFFFFFFFF; // 4字节: 4294967295
  // printf("i32: %u "FILE_LINE_STR" \n", i32, FILE_LINE);
  // unsigned low8bits = i32 & 0xFF;  // low8bits: 255 main.c:25 
  // printf("low8bits: %d "FILE_LINE_STR" \n", low8bits, FILE_LINE);

  // uint16_t low16bits = (uint16_t)i32;  // 65535
  // printf("low16bits: %d "FILE_LINE_STR" \n", low16bits, FILE_LINE);


  printf("oid.high: %lu "FILE_LINE_STR" \n", oid.high, FILE_LINE);
  printf("oid.low: %u "FILE_LINE_STR" \n", oid.low, FILE_LINE);
  uint32_t h1 = oid.high >> 8;
  uint32_t h2 = (uint32_t)(oid.high & 0xFFFFFFFF);
  printf("h1: %u "FILE_LINE_STR" \n", h1, FILE_LINE);
  printf("h2: %u "FILE_LINE_STR" \n", h2, FILE_LINE);


  return 0;
}

