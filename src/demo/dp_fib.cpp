#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int fib(int N) {
        if (N <= 1) return N;
        vector<int> dp(N + 1);
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2; i <= N; i++) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[N];
    }
};

class Solution2 {
public:
    int fib(int N) {
        if (N <= 1) return N;
        int dp[2];
        // 定义两个下标, 分别代替dp[i-1], dp[i-2]
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2; i <= N; i++) {
            int sum = dp[0] + dp[1];
            dp[0] = dp[1];
            dp[1] = sum; // 更新dp[1], 为下轮遍历,准备参数值
        }
        return dp[1];
    }
};

int main() {
  // Solution solution;
  Solution2 solution;
  // 1 1 2 3 5 8
  cout << "solution_res:" << solution.fib(6) << endl;

}