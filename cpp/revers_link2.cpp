/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        int i = 0, j = 0;
        ListNode *cur = head;
        while (cur) {
            if (i == left) {
                ListNode *pre = cur;
                ListNode *cur_next = cur->next;
                cur = cur -> next;
                while (left < right) {
                     pre = cur->next;
                     
                     cur = cur->next;   
                }
            }
            i++;
            cur = cur->next;
        }
    }
};
