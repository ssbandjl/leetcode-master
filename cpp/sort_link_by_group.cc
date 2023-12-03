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
        return sortList(head, nullptr);
    }

    ListNode* sortList(ListNode *head, ListNode *tail) {
        // if small is null, just return
        if (head == nullptr) {
            return head;
        }
        // if just two, jutst return left(small)
        if (head->next == tail) {
            head->next = nullptr;
            return head;
        }
        ListNode *slow = head, *fast = head;
        while (fast != tail) {
            slow = slow->next;
            fast = fast->next;
            if (fast != tail) {
                // if not the end, fast go two times than slow, then slow is mid (bellow)
                fast = fast->next;
            }
        }
        ListNode *mid = slow;
        // compare two node
        return merge(sortList(head, mid), sortList(mid, tail));
    }

    // sort two link
    ListNode* merge(ListNode *head1, ListNode *head2) {
        ListNode *dummyHead = new ListNode(0); // tmp_node
        ListNode *temp = dummyHead, *temp1 = head1, *temp2 = head2;
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

排序链表: https://leetcode.cn/problems/sort-list/solutions/492301/pai-xu-lian-biao-by-leetcode-solution/
力扣官方题解
412369
2020.11.20
发布于 未知归属地
排序
链表
双指针
C
6+
前言
「147. 对链表进行插入排序」要求使用插入排序的方法对链表进行排序，插入排序的时间复杂度是 O(n2)O(n^2)O(n 
2
 )，其中 nnn 是链表的长度。这道题考虑时间复杂度更低的排序算法。题目的进阶问题要求达到 O(nlog⁡n)O(n \log n)O(nlogn) 的时间复杂度和 O(1)O(1)O(1) 的空间复杂度，时间复杂度是 O(nlog⁡n)O(n \log n)O(nlogn) 的排序算法包括归并排序、堆排序和快速排序（快速排序的最差时间复杂度是 O(n2)O(n^2)O(n 
2
 )），其中最适合链表的排序算法是归并排序。

归并排序基于分治算法。最容易想到的实现方式是自顶向下的递归实现，考虑到递归调用的栈空间，自顶向下归并排序的空间复杂度是 O(log⁡n)O(\log n)O(logn)。如果要达到 O(1)O(1)O(1) 的空间复杂度，则需要使用自底向上的实现方式。

方法一：自顶向下归并排序
对链表自顶向下归并排序的过程如下。

找到链表的中点，以中点为分界，将链表拆分成两个子链表。寻找链表的中点可以使用快慢指针的做法，快指针每次移动 222 步，慢指针每次移动 111 步，当快指针到达链表末尾时，慢指针指向的链表节点即为链表的中点。

对两个子链表分别排序。

将两个排序后的子链表合并，得到完整的排序后的链表。可以使用「21. 合并两个有序链表」的做法，将两个有序的子链表进行合并。

上述过程可以通过递归实现。递归的终止条件是链表的节点个数小于或等于 111，即当链表为空或者链表只包含 111 个节点时，不需要对链表进行拆分和排序。

 */