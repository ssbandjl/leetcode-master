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
    ListNode* sortList(ListNode* head) {
        if (head == nullptr) {
            return head;
        }
        int length = 0;
        ListNode* node = head;
        while(node != nullptr) {
            length++;
            node = node->next;
        }
        ListNode* dummyHead = new ListNode(0, head);
        for (int subLength = 1; subLength < length; subLength <<=1) {
            ListNode* prev = dummyHead, *curr = dummyHead->next;
            while (curr != nullptr) {
                ListNode* head1 = curr;
                for (int i = 1; i < subLength && curr->next != nullptr; i++) {
                    curr = curr->next;
                }
                ListNode* head2 = curr->next;
                curr->next = nullptr;
                curr = head2;
                for (int i = 1; i < subLength && curr != nullptr && curr->next != nullptr; i++) {
                    curr = curr->next;
                }
                ListNode* next = nullptr;
                if (curr != nullptr) {
                    next = curr->next;
                    curr->next = nullptr;
                }
                ListNode* merged = merge(head1, head2);
                prev->next = merged;
                while (prev->next != nullptr) {
                    prev = prev->next;
                }
                curr = next;
            }
        }
        return dummyHead->next;
    }
    ListNode* merge(ListNode* head1, ListNode* head2) {
        ListNode* dummyHead = new ListNode(0);
        ListNode* temp = dummyHead, *temp1 = head1, *temp2 = head2;
        while (temp1 != nullptr && temp2 != nullptr) {
            if (temp1->val <= temp2->val) {
                temp->next = temp1;
                temp1 = temp1->next;
            } else {
                temp->next = temp2;
                temp2 = temp2->next;
            }
            temp = temp->next;
        }
        if (temp1 != nullptr) {
            temp->next = temp1;
        } else if (temp2 != nullptr) {
            temp->next = temp2;
        }
        return dummyHead->next;
    }
};


/* 
方法二：自底向上归并排序
使用自底向上的方法实现归并排序，则可以达到 O(1)O(1)O(1) 的空间复杂度。

首先求得链表的长度 length\textit{length}length，然后将链表拆分成子链表进行合并。

具体做法如下。

用 subLength\textit{subLength}subLength 表示每次需要排序的子链表的长度，初始时 subLength=1\textit{subLength}=1subLength=1。

每次将链表拆分成若干个长度为 subLength\textit{subLength}subLength 的子链表（最后一个子链表的长度可以小于 subLength\textit{subLength}subLength），按照每两个子链表一组进行合并，合并后即可得到若干个长度为 subLength×2\textit{subLength} \times 2subLength×2 的有序子链表（最后一个子链表的长度可以小于 subLength×2\textit{subLength} \times 2subLength×2）。合并两个子链表仍然使用「21. 合并两个有序链表」的做法。

将 subLength\textit{subLength}subLength 的值加倍，重复第 2 步，对更长的有序子链表进行合并操作，直到有序子链表的长度大于或等于 length\textit{length}length，整个链表排序完毕。

如何保证每次合并之后得到的子链表都是有序的呢？可以通过数学归纳法证明。

初始时 subLength=1\textit{subLength}=1subLength=1，每个长度为 111 的子链表都是有序的。

如果每个长度为 subLength\textit{subLength}subLength 的子链表已经有序，合并两个长度为 subLength\textit{subLength}subLength 的有序子链表，得到长度为 subLength×2\textit{subLength} \times 2subLength×2 的子链表，一定也是有序的。

当最后一个子链表的长度小于 subLength\textit{subLength}subLength 时，该子链表也是有序的，合并两个有序子链表之后得到的子链表一定也是有序的。

因此可以保证最后得到的链表是有序的。

作者：力扣官方题解
链接：https://leetcode.cn/problems/sort-list/solutions/492301/pai-xu-lian-biao-by-leetcode-solution/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


 */