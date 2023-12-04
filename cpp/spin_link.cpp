class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (k == 0 || head == nullptr || head->next == nullptr) {
            return head;
        }
        int n = 1;
        ListNode* iter = head;
        while (iter->next != nullptr) {
            iter = iter->next; // item已经指到了最后一个节点
            n++;
        }
        int add = n - k % n;
        if (add == n) {
            return head;
        }
        iter->next = head; // 最后一个节点指向head, 形成一个环形
        // 后递减, 将add-1, 返回值为add原始值
        while (add--) {
            iter = iter->next; // 找到需要断开的位置
        }
        ListNode* ret = iter->next; //返回新的头部
        iter->next = nullptr; // 断开环形
        return ret;
    }
};



// 61. 旋转链表, https://leetcode.cn/problems/rotate-list/description/
/* 

方法一：闭合为环
思路及算法

记给定链表的长度为 nnn，注意到当向右移动的次数 k≥nk \geq nk≥n 时，我们仅需要向右移动 k mod nk \bmod nkmodn 次即可。因为每 nnn 次移动都会让链表变为原状。这样我们可以知道，新链表的最后一个节点为原链表的第 (n−1)−(k mod n)(n - 1) - (k \bmod n)(n−1)−(kmodn) 个节点（从 000 开始计数）。

这样，我们可以先将给定的链表连接成环，然后将指定位置断开。

具体代码中，我们首先计算出链表的长度 nnn，并找到该链表的末尾节点，将其与头节点相连。这样就得到了闭合为环的链表。然后我们找到新链表的最后一个节点（即原链表的第 (n−1)−(k mod n)(n - 1) - (k \bmod n)(n−1)−(kmodn) 个节点），将当前闭合为环的链表断开，即可得到我们所需要的结果。

特别地，当链表长度不大于 111，或者 kkk 为 nnn 的倍数时，新链表将与原链表相同，我们无需进行任何处理

作者：力扣官方题解
链接：https://leetcode.cn/problems/rotate-list/solutions/681812/xuan-zhuan-lian-biao-by-leetcode-solutio-woq1/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
 */
