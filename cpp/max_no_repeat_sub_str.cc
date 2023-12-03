/* 
给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。
https://leetcode.cn/problems/longest-substring-without-repeating-characters/description/

*/


class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> occ;
        int n = s.size();
        int rk = -1, ans = 0;
        for (int i = 0; i < n; i++) {
            // 每轮遍历, 左指针右移一位
            if (i != 0) {
                occ.erase(s[i - 1]);
            }
            while (rk + 1 < n && !occ.count(s[rk + 1])) {
                occ.insert(s[rk + 1]);
                rk++;
            }
            // 获取最长子串长度
            ans = max(ans, rk - i +1);
        }
    return ans;
    }
};

