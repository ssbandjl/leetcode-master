#include <stdio.h>
#include <stdlib.h>


int fib(int n){
    //当n <= 1时，返回n
    if(n <= 1)
        return n;
    //动态开辟一个int数组，大小为n+1
    int *dp = (int *)malloc(sizeof(int) * (n + 1));
    //设置0号位为0，1号为为1
    dp[0] = 0;
    dp[1] = 1;

    //从前向后遍历数组(i=2; i <= n; ++i),下标为n时的元素为dp[i-1] + dp[i-2]
    int i;
    for(i = 2; i <= n; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

int main(){
  int res = 0;
  res = fib(6);
  printf("res:%d\n", res);
  return 0;
}