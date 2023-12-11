#include<bits/stdc++.h>
using namespace std;
const int ll = 1000000007;

//递推公式 f[i] = A * f[i - 1] - B * f[i - 2]
//f[i] = x ^ i + y ^ i
int helper(int A, int B, int n){
    vector<long> dp(3);
    dp[0] = 2;
    dp[1] = A;
    for(int i = 2; i <= n; i++){
        dp[2] = ((A * dp[1] % ll) - (B * dp[0] % ll) + ll) % ll;
        dp[0] = dp[1];
        dp[1] = dp[2];
    }
    return dp[2];
}

int main(){
    int count;
    cin >> count;
    while(count-- > 0){
        int A, B, n;
        cin >> A >> B >> n;
        cout << helper(A, B, n) << endl;
    }
    return 0;
}



// 作者：爆爆只是想帮忙
// 链接：https://www.nowcoder.com/exam/test/76577735/submission?examPageSource=Company&pid=30440638&testCallback=https%3A%2F%2Fwww.nowcoder.com%2Fexam%2Fcompany%3FcurrentTab%3Drecommand%26jobId%3D100%26selectStatus%3D0%26tagIds%3D134&testclass=%E8%BD%AF%E4%BB%B6%E5%BC%80%E5%8F%91
// 来源：牛客网

