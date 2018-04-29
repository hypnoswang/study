#ifndef INORDER_TRAVERSAL_H
#define INORDER_TRAVERSAL_H

#include<vector>

namespace bt {

    struct TreeNode {
        int val;
        TreeNode* left;
        TreeNode* right;

        TreeNode(int v):val(v) {
            left = NULL;
            right = NULL;
        }
    };

    class BinaryTree {
        public:
            BinaryTree();
            ~BinaryTree();

            bool init(const std::vector<int>& vals);
            bool insert(int val);
            bool remove(int val);

            std::vector<int> inorder_traversal();
            void inorder_traversal1(std::vector<int>& res);
            void inorder_traversal2(std::vector<int>& res);

            void preorder_traversal1(std::vector<int>& res);
            void preorder_traversal2(std::vector<int>& res);

            void postorder_traversal1(std::vector<int>& res);
            void postorder_traversal2(std::vector<int>& res);

            void zigzag_traversal(std::vector< std::vector<int> >& res);

            int kth_smallest_elem(int k);

        private:
            void inorder(TreeNode* p, std::vector<int>& res);
            void preorder(TreeNode* p, std::vector<int>& res);
            void postorder(TreeNode* p, std::vector<int>& res);

            void kth_smallest(TreeNode* tn, int k, int& seq, int& res);

        private:
            TreeNode* root;
            std::vector<TreeNode*> nodes;
    };

}


#endif
