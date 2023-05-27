#include <iostream>
#include <vector>
using namespace std;

class MyLinkedList {
public:
    // 定义链表节点结构体
    struct LinkedNode {
        int val;
        LinkedNode* next;
        LinkedNode(int val):val(val), next(nullptr){}
    };

    // 初始化链表
    MyLinkedList() {
        _dummyHead = new LinkedNode(0); // 这里定义的头结点 是一个虚拟头结点，而不是真正的链表头结点
        _size = 0;
    }

    // 获取到第index个节点数值，如果index是非法数值直接返回-1， 注意index是从0开始的，第0个节点就是头结点
    int get(int index) {
        if (index > (_size - 1) || index < 0) {
            return -1;
        }
        LinkedNode* cur = _dummyHead->next;
        while(index--){ // 如果--index 就会陷入死循环
            cur = cur->next;
        }
        return cur->val;
    }

    // 在链表最前面插入一个节点，插入完成后，新插入的节点为链表的新的头结点
    void addAtHead(int val) {
        LinkedNode* newNode = new LinkedNode(val);
        newNode->next = _dummyHead->next;
        _dummyHead->next = newNode;
        _size++;
    }

    // 在链表最后面添加一个节点
    void addAtTail(int val) {
        LinkedNode* newNode = new LinkedNode(val);
        LinkedNode* cur = _dummyHead;
        while(cur->next != nullptr){
            cur = cur->next;
        }
        cur->next = newNode;
        _size++;
    }

    // 在第index个节点之前插入一个新节点，例如index为0，那么新插入的节点为链表的新头节点。
    // 如果index 等于链表的长度，则说明是新插入的节点为链表的尾结点
    // 如果index大于链表的长度，则返回空
    // 如果index小于0，则在头部插入节点
    void addAtIndex(int index, int val) {

        if(index > _size) return;
        if(index < 0) index = 0;        
        LinkedNode* newNode = new LinkedNode(val);
        LinkedNode* cur = _dummyHead;
        while(index--) {
            cur = cur->next;
        }
        newNode->next = cur->next;
        cur->next = newNode;
        _size++;
    }

    // 删除第index个节点，如果index 大于等于链表的长度，直接return，注意index是从0开始的
    void deleteAtIndex(int index) {
        if (index >= _size || index < 0) {
            return;
        }
        LinkedNode* cur = _dummyHead;
        while(index--) {
            cur = cur ->next;
        }
        LinkedNode* tmp = cur->next;
        cur->next = cur->next->next;
        delete tmp;
        //delete命令指示释放了tmp指针原本所指的那部分内存，
        //被delete后的指针tmp的值（地址）并非就是NULL，而是随机值。也就是被delete后，
        //如果不再加上一句tmp=nullptr,tmp会成为乱指的野指针
        //如果之后的程序不小心使用了tmp，会指向难以预想的内存空间
        tmp=nullptr;
        _size--;
    }

    // 打印链表
    void printLinkedList() {
        LinkedNode* cur = _dummyHead;
        while (cur->next != nullptr) {
            cout << cur->next->val << " ";
            cur = cur->next;
        }
        cout << endl;
    }
private:
    int _size;
    LinkedNode* _dummyHead;

};
时间复杂度: 涉及 index 的相关操作为 O(index), 其余为 O(1)
空间复杂度: O(n)
#其他语言版本
C:

typedef struct MyLinkedList {
    int val;
    struct MyLinkedList* next;
}MyLinkedList;

/** Initialize your data structure here. */

MyLinkedList* myLinkedListCreate() {
    //这个题必须用虚拟头指针,参数都是一级指针,头节点确定后没法改指向了!!!
    MyLinkedList* head = (MyLinkedList *)malloc(sizeof (MyLinkedList));
    head->next = NULL;
    return head;
}

/** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
int myLinkedListGet(MyLinkedList* obj, int index) {
    MyLinkedList *cur = obj->next;
    for (int i = 0; cur != NULL; i++){
        if (i == index){
            return cur->val;
        }
        else{
            cur = cur->next;
        }
    }
    return -1;
}

/** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
void myLinkedListAddAtHead(MyLinkedList* obj, int val) {
    MyLinkedList *nhead = (MyLinkedList *)malloc(sizeof (MyLinkedList));
    nhead->val = val;
    nhead->next = obj->next;
    obj->next = nhead;

}

/** Append a node of value val to the last element of the linked list. */
void myLinkedListAddAtTail(MyLinkedList* obj, int val) {
    MyLinkedList *cur = obj;
    while(cur->next != NULL){
        cur = cur->next;
    }
    MyLinkedList *ntail = (MyLinkedList *)malloc(sizeof (MyLinkedList));
    ntail->val = val;
    ntail->next = NULL;
    cur->next = ntail;
}

/** Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted. */
void myLinkedListAddAtIndex(MyLinkedList* obj, int index, int val) {
    if (index == 0){
        myLinkedListAddAtHead(obj, val);
        return;
    }
    MyLinkedList *cur = obj->next;
    for (int i = 1 ;cur != NULL; i++){
        if (i == index){
            MyLinkedList* newnode = (MyLinkedList *)malloc(sizeof (MyLinkedList));
            newnode->val = val;
            newnode->next = cur->next;
            cur->next = newnode;
            return;
        }
        else{
            cur = cur->next;
        }
    }
}

/** Delete the index-th node in the linked list, if the index is valid. */
void myLinkedListDeleteAtIndex(MyLinkedList* obj, int index) {
    if (index == 0){
        MyLinkedList *tmp = obj->next;
        if (tmp != NULL){
            obj->next = tmp->next;
            free(tmp);     
        }
        return;
    }
    MyLinkedList *cur = obj->next;
    for (int i = 1 ;cur != NULL && cur->next != NULL; i++){
        if (i == index){
            MyLinkedList *tmp = cur->next;
            if (tmp != NULL) {
                cur->next = tmp->next;
                free(tmp);
            }
            return;
        }
        else{
            cur = cur->next;
        }
    }
    
}

void myLinkedListFree(MyLinkedList* obj) {
    while(obj != NULL){
        MyLinkedList *tmp = obj;
        obj = obj->next;
        free(tmp);
    }
}

/**
 * Your MyLinkedList struct will be instantiated and called as such:
 * MyLinkedList* obj = myLinkedListCreate();
 * int param_1 = myLinkedListGet(obj, index);
 
 * myLinkedListAddAtHead(obj, val);
 
 * myLinkedListAddAtTail(obj, val);
 
 * myLinkedListAddAtIndex(obj, index, val);
 
 * myLinkedListDeleteAtIndex(obj, index);
 
 * myLinkedListFree(obj);
*/

int main() {
 MyLinkedList* obj = myLinkedListCreate();
 int param_1 = myLinkedListGet(obj, index);
  myLinkedListAddAtHead(obj, val);
  myLinkedListAddAtTail(obj, val);
  myLinkedListAddAtIndex(obj, index, val);
  myLinkedListDeleteAtIndex(obj, index);
  myLinkedListFree(obj);
}