#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* temp; // 保存cur的下一个节点
        ListNode* cur = head;
        ListNode* pre = NULL;
        while(cur) {
            temp = cur->next;  // 保存一下 cur的下一个节点，因为接下来要改变cur->next
            cur->next = pre; // 翻转操作
            // 更新pre 和 cur指针, 移动到下一个元素
            pre = cur;
            cur = temp;
        }
        return pre;
    }
};

int main() {
  Solution solution;
  cout << "solution_res:" << solution.reverseList() << endl;
}