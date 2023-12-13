#include <iostream>
#include <vector>
#include <algorithm>
// #include <boost/algorithm/string.hpp>
// #include <string>


using namespace std;
// using namespace boost;


/* 
LCR 181. 字符串中的单词反转（双指针，清晰图解: https://leetcode.cn/problems/fan-zhuan-dan-ci-shun-xu-lcof/solutions/195224/mian-shi-ti-58-i-fan-zhuan-dan-ci-shun-xu-shuang-z/

*/

std::string& ltrim(std::string &str, std::string const &whitespace = " \r\n\t\v\f")
{
    str.erase(0, str.find_first_not_of(whitespace));
    return str;
}
 
std::string& rtrim(std::string &str, std::string const &whitespace = " \r\n\t\v\f")
{
    str.erase(str.find_last_not_of(whitespace) + 1);
    return str;
}
 
std::string& trim(std::string &str, std::string const &whitespace=" \r\n\t\v\f")
{
   return ltrim(rtrim(str, whitespace), whitespace);
}

class Solution {
public:
    string reverseMessage(string message) {
        // message.erase(std::remove(message.begin(), message.end(), ' '), message.end()); // 移除所有空格
        trim(message); // 删除首尾空格
        int i, j = message.size()-1;
        string res = "";
        while (i >= 0) {
            while (i>=0 && message[i] != ' ') {
                i--;
                res.append(message[i+1 : j+1]);
            }
        }

        return res;
    }
};

// g++ -o main main.cc && ./main
int main() {
    Solution solution;
    string in = "    hello world my love   ";
    cout << in << endl;
    string out = solution.reverseMessage(in);
    cout << out << endl;
}