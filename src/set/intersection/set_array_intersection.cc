#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> result_set; // 存放结果，之所以用set是为了给结果集去重
        int hash[1005] = {0}; // 默认数值为0
        for (int num : nums1) { // nums1中出现的字母在hash数组中做记录
            hash[num] = 1;
        }
        for (int num : nums2) { // nums2中出现话，result记录
            if (hash[num] == 1) {
                result_set.insert(num);
            }
        }
        return vector<int>(result_set.begin(), result_set.end());
    }
};

int main() {
  Solution solution;
  vector<int> nums1 {2, 2, 5, 9};
  vector<int> nums2 {1, 2, 5, 0};
  vector<int> result = solution.intersection(nums1, nums2);
  for(int i : result){
    cout << "solution_res:" << i << endl;
  }
}
