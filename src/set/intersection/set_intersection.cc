#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> result_set; // 存放结果，之所以用set是为了给结果集去重
        unordered_set<int> nums_set(nums1.begin(), nums1.end());
        for (int num : nums2) {
            // 发现nums2的元素 在nums_set里又出现过
            if (nums_set.find(num) != nums_set.end()) {
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
  for(int i : result) {
    cout << "solution_res:" << i << endl;
  }
}

