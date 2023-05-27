#include <stdio.h>
#include <stdlib.h>

int fib(int n){
    //若n小于等于1，返回n
    if(n <= 1)
        return n;
    //否则返回fib(n-1) + fib(n-2)
    return fib(n-1) + fib(n-2);
}

int main(){
  printf("fib_res:%d\n", fib(5));
  return 0;
}