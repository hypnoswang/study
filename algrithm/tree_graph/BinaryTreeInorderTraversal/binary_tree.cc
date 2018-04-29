#include <iostream>
#include <vector>
#include <stack>
#include <deque>

#include "binary_tree.h"

namespace bt {


    BinaryTree::BinaryTree() :
        root(NULL) {
    }

    BinaryTree::~BinaryTree() {
        std::vector<TreeNode*>::iterator it = nodes.begin();
        for (; it != nodes.end(); ++it) {
            delete(*it);
        }

        nodes.clear();
    }


    bool BinaryTree::init(const std::vector<int>& vals) {
        TreeNode* tr = NULL;
        TreeNode* p = NULL;

        std::vector<int>::const_iterator cit = vals.begin();
        for (; cit != vals.end(); ++cit) {
            int val = *cit;
            bt::TreeNode* tn = new TreeNode(val);
            if (tn == NULL) {   // may cause memory leak
                //should delete the nodes which have been new-ed
                return false;
            }

            if (tr == NULL) {
                tr = tn;
                continue;
            }

            p = tr;
            while (true) {
                if (val == p->val ){
                    delete tn;
                    tn = NULL;
                    break;
                } else if (val > p->val) {
                    if (p->right == NULL) {
                        p->right = tn;
                        break;
                    } else {
                        p = p->right;
                    }
                } else {
                    if (p->left == NULL) {
                        p->left = tn;
                        break;
                    } else {
                        p = p->left;
                    }
                }
            }

            if (tn != NULL) {
                nodes.push_back(tn);
            }
        }

        root = tr;

        return true;
    }

    bool BinaryTree::insert(int val) {
    }

    bool BinaryTree::remove(int val) {
    }

    void BinaryTree::inorder(TreeNode* p, std::vector<int>& res) {
        if (p == NULL) {
            return;
        }

        TreeNode* tn = p;
        if (tn->left != NULL) {
            inorder(tn->left, res);
        }

        res.push_back(tn->val);

        if (tn->right != NULL) {
            inorder(tn->right, res);
        }
    }

    std::vector<int> BinaryTree::inorder_traversal() {
        std::vector<int> res;
        if (root == NULL) {
            return res;
        }

        std::stack<TreeNode*> mystack;
        int stat = 0;
        TreeNode* tn = root;
        while(true) {
            if (tn->left != NULL && stat < 1) {
                mystack.push(tn);
                tn = tn->left;
                stat = 0;
                continue;
            } 

            res.push_back(tn->val);

            if (tn->right != NULL) {
                tn = tn->right;
                stat = 0;
                continue;
            }

            if (mystack.empty()) {
                break;
            } else {
                stat = 1;
                tn = mystack.top();
                mystack.pop();
            }
        }

        return res;
    }

    void BinaryTree::inorder_traversal1(std::vector<int>& res) {
        inorder(root, res);
    }

    void BinaryTree::inorder_traversal2(std::vector<int>& res) {
        if (root == NULL) {
            return;
        }

        std::stack<TreeNode*> mystack;

        TreeNode* tn = root;
        bool poped = false;
        while (true) {
            if (!poped && tn->left != NULL) {
                mystack.push(tn);
                tn = tn->left;
                poped = false;
                continue;
            }

            res.push_back(tn->val);

            if (tn->right != NULL) {
                tn = tn->right;
                poped = false;
                continue;
            }

            if (mystack.empty()) {
                break;
            } else {
                tn = mystack.top();
                mystack.pop();
                poped = true;
            }
        }

    }

    void BinaryTree::preorder(TreeNode* p, std::vector<int>& res) {
        if (p == NULL) return;

        TreeNode* tn = p;
        res.push_back(tn->val);

        if (tn->left != NULL) {
            preorder(tn->left, res);
        }

        if (tn->right != NULL) {
            preorder(tn->right, res);
        }
    }

    void BinaryTree::preorder_traversal1(std::vector<int>& res) {
        if (root == NULL)  {
            return;
        }

        preorder(root, res);
    }

    void BinaryTree::preorder_traversal2(std::vector<int>& res) {
        if (root == NULL) {
            return;
        }

        std::stack<TreeNode*> mystack;

        TreeNode* tn = root;
        bool poped = false;

        while (true) {
            if (!poped) {
                res.push_back(tn->val);
            }

            if (tn->left != NULL && !poped) {
                mystack.push(tn);
                tn = tn->left;
                poped = false;
                continue;
            }

            if (tn->right != NULL) {
                tn = tn->right;
                poped = false;
                continue;
            }

            if (mystack.empty()) {
                break;
            } else {
                tn = mystack.top();
                mystack.pop();
                poped = true;
            }
        }
    }

    void BinaryTree::postorder(TreeNode* p, std::vector<int>& res) {
        if (p == NULL) return;

        TreeNode* tn = p;

        if (tn->left != NULL) {
            postorder(tn->left, res);
        }

        if (tn->right != NULL) {
            postorder(tn->right, res);
        }

        res.push_back(tn->val);
    }

    void BinaryTree::postorder_traversal1(std::vector<int>& res) {
        if (root == NULL)  {
            return;
        }

        postorder(root, res);
    }

    void BinaryTree::postorder_traversal2(std::vector<int>& res) {
        if (root == NULL) {
            return;
        }

        std::stack<TreeNode*> mystack;

        TreeNode* tn = root;
        int stat = 0;

        while(true) {
            if (tn->left != NULL && stat < 1) {
                mystack.push(tn);
                tn = tn->left;
                stat = 0;
                continue;
            }

            if (tn->right != NULL && stat < 2) {
                mystack.push(tn);
                tn = tn->right;
                stat = 0;
                continue;
            }

            res.push_back(tn->val);

            if (mystack.empty()) {
                break;
            } else {
                if (tn == mystack.top()->left) {
                    stat = 1;
                } else if (tn == mystack.top()->right) {
                    stat = 2;
                } else {
                    std::cout << "Internal error" << std::endl;
                    break;
                }
                tn = mystack.top();
                mystack.pop();
            }
        }
    }

    void BinaryTree::zigzag_traversal(std::vector< std::vector<int> >& res) {
        if (root == NULL) return;

        std::deque<TreeNode*> myqueue;
        myqueue.push_back(root);

        bool left = true;
        while (true) {
            std::vector<int> r;
            std::size_t size = myqueue.size();

            for (size_t i = 0; i < size; ++i) {
                if (left) {
                    TreeNode* tn = myqueue.front();
                    myqueue.pop_front();
                    r.push_back(tn->val);
                    if (tn->left != NULL) {
                        myqueue.push_back(tn->left);
                    }
                    if (tn->right != NULL) {
                        myqueue.push_back(tn->right);
                    }
                } else {
                    TreeNode *tn = myqueue.back();
                    myqueue.pop_back();
                    r.push_back(tn->val);
                    if (tn->right != NULL) {
                        myqueue.push_front(tn->right);
                    }
                    if (tn->left != NULL) {
                        myqueue.push_front(tn->left);
                    }
                }
            }

            res.push_back(r);

            if (myqueue.empty()) break;

            left = !left;
        }
    }

    void BinaryTree::kth_smallest(TreeNode* tn, int k, int& seq, int& res) {
        if (tn == NULL) return;

        if (tn->left != NULL) kth_smallest(tn->left, k, seq, res);

        seq++;
        if (seq == k) res = tn->val;

        if (tn->right != NULL) kth_smallest(tn->right, k, seq, res);
    }

    int BinaryTree::kth_smallest_elem(int k) {
        if (root == NULL) return 0;

        int seq = 0;
        int res = 0;
        kth_smallest(root, k, seq, res);

        return res;
    }

}
