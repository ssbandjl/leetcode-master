#include <iostream>
#include <vector>

using namespace std;


/* 
LCR 181. 字符串中的单词反转（双指针，清晰图解: https://leetcode.cn/problems/fan-zhuan-dan-ci-shun-xu-lcof/solutions/195224/mian-shi-ti-58-i-fan-zhuan-dan-ci-shun-xu-shuang-z/

*/

class Solution {
public:
    string reverseMessage(string message) {
        return message;
    }
};

// g++ -o main main.cc && ./main
int main() {
    Solution solution;
    string in = "hello world my love";
    cout << in << endl;
    string out = solution.reverseMessage(in);
    cout << out << endl;
}