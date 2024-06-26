class Solution {
public:
    int reverse(int x) {
        int rev = 0;
        while (x != 0) {
            if (rev < INT_MIN / 10 || rev > INT_MAX / 10) {
                return 0;
            }
            int digit = x % 10; // pop x last num
            x /= 10;

            rev = rev * 10 + digit; // push digit to rev last
        }
        return rev;
    }
};

/* 
7. 整数反转, https://leetcode.cn/problems/reverse-integer/description/
中等
3.9K
相关企业
给你一个 32 位的有符号整数 x ，返回将 x 中的数字部分反转后的结果。

如果反转后整数超过 32 位的有符号整数的范围 [−231,  231 − 1] ，就返回 0。

假设环境不允许存储 64 位整数（有符号或无符号）。

 */