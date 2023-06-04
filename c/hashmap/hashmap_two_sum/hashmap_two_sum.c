#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

// leetcode 支持 ut_hash 函式庫

 typedef struct {
     int key;
     int value;
     UT_hash_handle hh; // make this structure hashable
 } map;

map* hashMap = NULL;

 void hashMapAdd(int key, int value){
     map* s;
     // key already in the hash?
     HASH_FIND_INT(hashMap, &key, s);
     if(s == NULL){
         s = (map*)malloc(sizeof(map));
         s -> key = key;
         HASH_ADD_INT(hashMap, key, s);
     }
     s -> value = value;
 }

map* hashMapFind(int key){
     map* s;
     // *s: output pointer
     HASH_FIND_INT(hashMap, &key, s);   
     return s;
 }

 void hashMapCleanup(){
     map* cur, *tmp;
     HASH_ITER(hh, hashMap, cur, tmp){
         HASH_DEL(hashMap, cur);
         free(cur);
     }
 }

 void hashPrint(){
     map* s;
     for(s = hashMap; s != NULL; s=(map*)(s -> hh.next)){
         printf("key %d, value %d\n", s -> key, s -> value);
     }
 }

 
int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    int i, *ans;
    // hash find result
    map* hashMapRes; 
    hashMap = NULL;
    ans = malloc(sizeof(int) * 2);

    for(i = 0; i < numsSize; i++){
        // key 代表 nums[i] 的值，value 代表所在 index;
        hashMapAdd(nums[i], i);
    }

    hashPrint();

    for(i = 0; i < numsSize; i++){
        hashMapRes = hashMapFind(target - nums[i]);
        if(hashMapRes && hashMapRes -> value != i){
            ans[0] = i;
            ans[1] = hashMapRes -> value ;
            *returnSize = 2;
            return ans;
        }
    }
    
    hashMapCleanup();
    return NULL;
}


int main(){
  printf("x\n");
  return 0;
}
