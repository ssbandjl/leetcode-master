// 糖果为体积, 牛的数量为价值, 求体积不超过m的最大价值 
// 若第i头牛有约定, 则将i和j绑定成一个物品: 体积为a[i] + a[j], 价值为2; 否则i是体积为a[i],价值为1的物品 
 
#include <bits/stdc++.h>

using namespace std;
const int N = 1010;
int f[N];
int w[N], v[N];
bool st[N];

void solve(){
    int n, m, k;
    cin >> n >> m;
    for (int i = 1; i <= n; i ++ )
        cin >> w[i];

    cin >> k;
    vector<int> ww, vv;
    for (int i = 1; i <= k; i ++ ) {
        int u, v;
        cin >> u >> v;
        ww.push_back(w[u] + w[v]), vv.push_back(2);
        st[u] = st[v] = true;
    }
    for (int i = 1; i <= n; i ++ ) 
        if (st[i] == false)
            ww.push_back(w[i]), vv.push_back(1);

    int nn = ww.size();

    for (int i = 0; i < nn; i ++ )
        for (int j = m; j >= ww[i]; j -- )
            f[j] = max(f[j], f[j - ww[i]] + vv[i]);
    cout << f[m] << endl;
}

int main()
{   
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    int t;
    // cin >> t;
    t = 1;
    while (t -- )
        solve();
    return 0;
}



// 作者：只想拿个offer啊
// 链接：https://www.nowcoder.com/exam/test/76601496/submission?examPageSource=Company&pid=30440638&testCallback=https%3A%2F%2Fwww.nowcoder.com%2Fexam%2Fcompany%3FcurrentTab%3Drecommand%26jobId%3D100%26selectStatus%3D0%26tagIds%3D134&testclass=%E8%BD%AF%E4%BB%B6%E5%BC%80%E5%8F%91
// 来源：牛客网