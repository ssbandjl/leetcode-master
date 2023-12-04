class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector <vector <int>> ret;
        if (!root) {
            return ret;
        }

        queue <TreeNode*> q;
        q.push(root);
        while (!q.empty()) { // first cycle it's root
            int currentLevelSize = q.size();
            ret.push_back(vector <int> ()); // dummy_node, use by back()
            for (int i = 1; i <= currentLevelSize; ++i) {
                auto node = q.front(); q.pop();
                ret.back().push_back(node->val); // get last item, then push_back
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        
        return ret;
    }
};


/* 
二叉树广度优先遍历: 
https://leetcode.cn/problems/binary-tree-level-order-traversal/solutions/241885/er-cha-shu-de-ceng-xu-bian-li-by-leetcode-solution/


 */