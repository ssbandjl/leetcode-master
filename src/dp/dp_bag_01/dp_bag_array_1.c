#include <stdio.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ARR_SIZE(arr) ((sizeof((arr))) / sizeof((arr)[0]))
#define BAG_WEIGHT 4

void test_back_pack(int* weights, int weightSize, int* values, int valueSize, int bagWeight) {
    int dp[bagWeight + 1];
    memset(dp, 0, sizeof(int) * (bagWeight + 1));

    int i, j;
    // 先遍历物品
    for(i = 0; i < weightSize; ++i) {
        // 后遍历重量。从后向前遍历
        for(j = bagWeight; j >= weights[i]; --j) {
            dp[j] = MAX(dp[j], dp[j - weights[i]] + values[i]);
        }
    }

    // 打印最优结果
    printf("%d\n", dp[bagWeight]);
}

int main(int argc, char** argv) {
    int weights[] = {1, 3, 4};
    int values[] = {15, 20, 30};
    test_back_pack(weights, ARR_SIZE(weights), values, ARR_SIZE(values), BAG_WEIGHT);
    return 0;
}