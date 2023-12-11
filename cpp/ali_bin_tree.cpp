#include <bits/stdc++.h>

using namespace std;

const int MOD = 1e9 + 7;

int main() {
    int n; // 节点个数
    int m; // 最大高度
    cin >> n >> m;

    // dp[i][j] 表示 i 个节点能够组成的高度不超过 j 的树的个数
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1));
    for (int i = 0; i <= m; ++i) {
        dp[0][i] = 1;
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            // 选取一个节点作为根节点
            // k 个节点作为左子树，i - k - 1 个节点作为右子树
            for (int k = 0; k < i; ++k) {
                dp[i][j] = (dp[i][j] + dp[k][j - 1] * dp[i - k - 1][j - 1] % MOD) % MOD;
            }
        }
    }

    cout << dp[n][m] << endl;
}

// 作者：乔木少年
// 链接：https://www.nowcoder.com/exam/test/76577735/submission?examPageSource=Company&pid=30440638&testCallback=https%3A%2F%2Fwww.nowcoder.com%2Fexam%2Fcompany%3FcurrentTab%3Drecommand%26jobId%3D100%26selectStatus%3D0%26tagIds%3D134&testclass=%E8%BD%AF%E4%BB%B6%E5%BC%80%E5%8F%91
// 来源：牛客网
