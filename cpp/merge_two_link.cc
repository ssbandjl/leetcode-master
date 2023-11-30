#include <iostream>
#include <vector>
#include "./list.h"
using namespace std;


/* 
21. 合并两个有序链表:
https://leetcode.cn/problems/merge-two-sorted-lists/solutions/226408/he-bing-liang-ge-you-xu-lian-biao-by-leetcode-solu/

*/

/* 合并两个链表 */
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    if (l1 == nullptr) {
        return l2;
    } else if (l2 == nullptr) {
        return l1;
    } else if (l1->val < l2->val) {
        l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    } else {
        l2->next = mergeTwoLists(l1, l2->next);
        return l2;
    }
}

int main() {
    MyLinkedList* l1 = new MyLinkedList();
    l1->addAtTail(1);
    l1->addAtTail(2);
    l1->addAtTail(5);
    l1->addAtTail(8);
    l1->addAtTail(111);
    l1->for_each();

    MyLinkedList* l2 = new MyLinkedList();
    l2->addAtTail(2);
    l2->addAtTail(5);
    l2->addAtTail(8);
    l2->addAtTail(9);
    l2->addAtTail(100);
    l2->for_each();
    
    ListNode *out = mergeTwoLists(l1->head, l2->head);
    while (out != NULL){
        cout << out->val;
        cout << ",";
        out = out->next;
    }
    cout << "\n";
}   