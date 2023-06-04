#include <stdio.h>
#include <stdlib.h>
#include "../../uthash.h"

struct hashTable {
    int key;
    int val;
    UT_hash_handle hh;
};

int fourSumCount(int* A, int ASize, int* B, int BSize, int* C, int CSize, int* D, int DSize) {
    struct hashTable* tmp;
    struct hashTable* hashtable_header = NULL;
    for (int i = 0; i < ASize; ++i) {
        for (int j = 0; j < BSize; ++j) {
            int ikey = A[i] + B[j];
            HASH_FIND_INT(hashtable_header, &ikey, tmp);
            if (tmp == NULL) {
                struct hashTable* tmp = malloc(sizeof(struct hashTable));
                tmp->key = ikey, tmp->val = 1;
                HASH_ADD_INT(hashtable_header, key, tmp);
            } else {
                tmp->val++;
            }
        }
    }
    int ans = 0;
    for (int i = 0; i < CSize; ++i) {
        for (int j = 0; j < DSize; ++j) {
            int ikey = -C[i] - D[j];
            HASH_FIND_INT(hashtable_header, &ikey, tmp);
            if (tmp != NULL) {
                ans += tmp->val;
            }
        }
    }
    return ans;
}


int main(){
  int a[3] = {1, 2, 3};
  int b[3] = {1, 2, 3};
  int c[3] = {1, -2, 3};
  int d[3] = {1, 2, -3};
  int count = 0;
  count = fourSumCount(a, 3, b, 3, c, 3, d, 3);
  printf("count:%d", count);
  return 0;
}
