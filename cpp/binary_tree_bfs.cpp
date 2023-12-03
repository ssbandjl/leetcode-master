/* 层序遍历 */
vector<int> levelOrder(TreeNode *root) {
    // 初始化队列，加入根节点
    queue<TreeNode *> queue;
    queue.push(root);
    // 初始化一个列表，用于保存遍历序列
    vector<int> vec;
    while (!queue.empty()) {
        TreeNode *node = queue.front();
        queue.pop();              // 队列出队
        vec.push_back(node->val); // 保存节点值
        if (node->left != nullptr)
            queue.push(node->left); // 左子节点入队
        if (node->right != nullptr)
            queue.push(node->right); // 右子节点入队
    }
    return vec;
}

/* white line by line */
// vector<int> levelOrder(TreeNode *root) {
//     queue<TreeNode *> queue;
//     queue.push(root);
//     vector<int> vec;
//     while (!queue.empty()) {
//         TreeNode *node = queue.front();
//         queue.pop();
//         vec.push_back(node->val);
//         if (node->left != nullptr) {
//             queue.push(node->left);
//         }
//         if (node->right != nullptr) {
//             queue.push(node->right);
//         }
//     }
//     return vec;
// }