#include<bits/stdc++.h>
using namespace std;
//一眼就能看出来是固定一个维度后的最长递增子序列问题，和力扣里的俄罗斯套娃问题、马戏团搭人梯问题类似
int helper(vector<vector<int>>& nums, int n){
    sort(nums.begin(), nums.end(), [&](auto& a, auto& b){
        if(a[0] == b[0]) return a[1] > b[1];
        return a[0] < b[0];
    });
    //这里用贪心+二分+dp来从O（n2）优化为O（nlogn）
    vector<int> f;
    f.push_back(nums[0][1]);
    for(int i = 1; i < n; i++){
        if(nums[i][1] > f.back()) f.push_back(nums[i][1]);
        else{
            auto it = lower_bound(f.begin(), f.end(), nums[i][1]);
            *it = nums[i][1];
        }
    }
    return f.size();
}
int main(){
    int count;
    cin >> count;
    while(count-- > 0){
        int n;
        cin >> n;
        vector<vector<int>> nums(n, vector<int>(2));
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < n; j++){
                cin >> nums[j][i];
            }
        }
        cout << helper(nums, n) << endl;
    }
}

// 作者：爆爆只是想帮忙
// 链接：https://www.nowcoder.com/exam/test/76577593/submission?examPageSource=Company&pid=30440638&testCallback=https%3A%2F%2Fwww.nowcoder.com%2Fexam%2Fcompany%3FcurrentTab%3Drecommand%26jobId%3D100%26selectStatus%3D0%26tagIds%3D134&testclass=%E8%BD%AF%E4%BB%B6%E5%BC%80%E5%8F%91
// 来源：牛客网