#include <stdio.h>
#include <stdlib.h>

typedef struct HashNodeTag {
    int key;   /* num  */
    struct HashNodeTag *next;
}HashNode;

/* Calcualte the hash key */
static inline int hash(int key, int size) {
    int index = key % size;
    return (index > 0) ? (index) : (-index);
}

/* Calculate the sum of the squares of its digits*/
static inline int calcSquareSum(int num) {
    unsigned int sum = 0;
    while(num > 0) {
        sum += (num % 10) * (num % 10);
        num = num/10; 
    }
    return sum;
}

int main(){
  printf("x\n");
  return 0;
}
