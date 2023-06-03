#include <iostream>
#include <vector>
using namespace std;

// 时间复杂度：O(n)
// 空间复杂度：O(1)
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int slowIndex = 0;
        for (int fastIndex = 0; fastIndex < nums.size(); fastIndex++) {
            if (val != nums[fastIndex]) {
                nums[slowIndex++] = nums[fastIndex];
            }
        }
        return slowIndex;
    }
};

int main() {
  Solution solution;
  vector<int> nums {1, 2, 2, 3, 4};
  cout << "solution_res:" << solution.removeElement(nums, 3) << endl;
}
