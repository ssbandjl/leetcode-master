/* 选择排序, 「选择排序 selection sort」的工作原理非常简单：开启一个循环，每轮从未排序区间选择最小的元素，将其放到已排序区间的末尾 */
void selectionSort(vector<int> &nums) {
    int n = nums.size();
    // 外循环：未排序区间为 [i, n-1]
    for (int i = 0; i < n - 1; i++) {
        // 内循环：找到未排序区间内的最小元素
        int k = i;
        for (int j = i + 1; j < n; j++) {
            if (nums[j] < nums[k])
                k = j; // 记录最小元素的索引
        }
        // 将该最小元素与未排序区间的首个元素交换
        swap(nums[i], nums[k]);
    }
}

// void selectionSort(vector<int> &nums) {
//     int n = nums.size();
//     for (int i = 0; i < n - 1; i++) {
//         int k = i;
//         for (int j = i + 1; j < n; j++) {
//             if (nums[j] < nums[k]) {
//                 k = j;
//             }
//         }
//         swap(nums[i], nums[k]);
//     }
// }


// void selectionSort(vector<int> &nums) {
//     int n = nums.size();
//     for (int i = 0; i < n - 1; i++) {
//         int j = i;
//         for (int k = j + 1; k < n; k++) {
//             if (nums[k] < nums[j]) {
//                 j = k;
//             }
//         }
//         swap(nums[i], nums[j]);
//     }
// }