
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int climbStairs(int n) {
        if (n <= 1) return n;
        int dp[3];
        dp[1] = 1;
        dp[2] = 2;
        for (int i = 3; i <= n; i++) {
            int sum = dp[1] + dp[2];
            dp[1] = dp[2];
            dp[2] = sum;
        }
        return dp[2];
    }
};

// class Solution {
// public:
//     int climbStairs(int n) {
//         if (n<=1) return n;
//         int dp[4];
//         dp[1] = 1;
//         dp[2] = 2;
//         for(int i = 3; i<=n; i++){
//             dp[3] = dp[1] + dp[2];
//             dp[1] = dp[2];
//             dp[2] = dp[3];
//         }
//         return dp[2];
//     }
// };

int main() {
  Solution solution;
  cout << "solution_res:" << solution.climbStairs(6) << endl;

}