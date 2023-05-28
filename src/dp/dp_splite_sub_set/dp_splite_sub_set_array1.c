/**
1. dp数组含义：dp[j]为背包重量为j时，其中可放入元素的最大值
2. 递推公式：dp[j] = max(dp[j], dp[j - nums[i]] + nums[i])
3. 初始化：均初始化为0即可
4. 遍历顺序：先遍历物品，再后序遍历背包
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
    // 背包容量
    int target = sum / 2;

    // 初始化dp数组，元素均为0
    int dp[target + 1];
    memset(dp, 0, sizeof(int) * (target + 1));

    int i, j;
    // 先遍历物品，后遍历背包
    for(i = 0; i < numsSize; ++i) {
        for(j = target; j >= nums[i]; --j) {
            dp[j] = MAX(dp[j], dp[j - nums[i]] + nums[i]);
        }
    }
    
    // 查看背包容量为target时，元素总和是否等于target
    return dp[target] == target;
}
