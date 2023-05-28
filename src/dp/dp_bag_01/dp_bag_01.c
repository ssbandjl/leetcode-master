#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 两数取其大
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ARR_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#define BAG_WEIGHT 4

void backPack(int* weights, int weightSize, int* costs, int costSize, int bagWeight) {
    // 开辟dp数组
    int dp[weightSize][bagWeight + 1];
    memset(dp, 0, sizeof(int) * weightSize * (bagWeight + 1));

    int i, j;
    // 当背包容量大于物品0的重量时，将物品0放入到背包中
    for(j = weights[0]; j <= bagWeight; ++j) {
        dp[0][j] = costs[0];
    }

    // 先遍历物品，再遍历重量
    for(j = 1; j <= bagWeight; ++j) {
        for(i = 1; i < weightSize; ++i) {
            // 如果当前背包容量小于物品重量
            if(j < weights[i])
                // 背包物品的价值等于背包不放置当前物品时的价值
                dp[i][j] = dp[i-1][j];
            // 若背包当前重量可以放置物品
            else
                // 背包的价值等于放置该物品或不放置该物品的最大值
                dp[i][j] = MAX(dp[i - 1][j],  dp[i - 1][j - weights[i]] + costs[i]);
        }
    }

    printf("%d\n", dp[weightSize - 1][bagWeight]);
}

int main(int argc, char* argv[]) {
    int weights[] = {1, 3, 4};
    int costs[] = {15, 20, 30};
    backPack(weights, ARR_SIZE(weights), costs, ARR_SIZE(costs), BAG_WEIGHT);
    return 0;
}
