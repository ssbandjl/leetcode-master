#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
1. dp数组含义：dp[i][j]为背包重量为j时，从[0-i]元素和最大值
2. 递推公式：dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - nums[i]] + nums[i])
3. 初始化：dp[i][0]初始化为0。因为背包重量为0时，不可能放入元素。dp[0][j] = nums[0]，当j >= nums[0] && j < target时
4. 遍历顺序：先遍历物品，再遍历背包
*/
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int getSum(int* nums, int numsSize) {
    int sum = 0;

    int i;
    for(i = 0; i < numsSize; ++i) {
        sum += nums[i];
    }
    return sum;
}

bool canPartition(int* nums, int numsSize){
    // 求出元素总和
    int sum = getSum(nums, numsSize);
    // 若元素总和为奇数，则不可能得到两个和相等的子数组
    if(sum % 2)
        return false;

    // 若子数组的和等于target，则nums可以被分割
    int target = sum / 2;
    // 初始化dp数组
    int dp[numsSize][target + 1];
    // dp[j][0]都应被设置为0。因为当背包重量为0时，不可放入元素
    memset(dp, 0, sizeof(int) * numsSize * (target + 1));

    int i, j;
    // 当背包重量j大于nums[0]时，可以在dp[0][j]中放入元素nums[0]
    for(j = nums[0]; j <= target; ++j) {
        dp[0][j] = nums[0];
    }

    for(i = 1; i < numsSize; ++i) {
        for(j = 1; j <= target; ++j) {
            // 若当前背包重量j小于nums[i]，则其值等于只考虑0到i-1物品时的值
            if(j < nums[i])
                dp[i][j] = dp[i - 1][j];
            // 否则，背包重量等于在背包中放入num[i]/不放入nums[i]的较大值
            else
                dp[i][j] = MAX(dp[i - 1][j], dp[i - 1][j  - nums[i]] + nums[i]);
        }
    }
    // 判断背包重量为target，且考虑到所有物品时，放入的元素和是否等于target
    return dp[numsSize - 1][target] == target;
}

int main(){
  int nums[4] = {1, 5, 5, 11};
  printf("res:%d\n", canPartition(nums, sizeof(nums)/sizeof(int)));
  return 0;
}