#include <iostream>
#include <vector>


using namespace std;


/* 
LCR 181. 字符串中的单词反转（双指针，清晰图解: https://leetcode.cn/problems/fan-zhuan-dan-ci-shun-xu-lcof/solutions/195224/mian-shi-ti-58-i-fan-zhuan-dan-ci-shun-xu-shuang-z/

*/

class Solution {
public:
    string reverseWords(string s) {
        string res;
        int n = s.size();
        if (n==0) return res;
        int right = n -1;
        while (right >= 0) {
            while (right >= 0 && s[right] == ' ') {
                right--;
            }
            if (right < 0) break;
            
            int left = right;
            while (left >=0 && s[left] != ' ') left--;

            // add words
            res +=s.substr(left + 1, right - left);
            res+= ' '; // 最后会加一个字符串, 需要去掉

            right = left;
        }
        if(!res.empty()) res.pop_back();
        return res;
    }    
};


// g++ -o main main.cc && ./main
int main() {
    Solution solution;
    string in = "    hello world my love   ";
    cout << in << endl;
    string out = solution.reverseWords(in);
    cout << out << endl;
}