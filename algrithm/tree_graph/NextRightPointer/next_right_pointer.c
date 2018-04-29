#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "next_right_pointer.h"

TreeLinkNode* build_tree(int* vals, int len) {
    TreeLinkNode* root = NULL;
    TreeLinkNode* p = NULL;

    for (int i = 0; i < len; ++i) {
        int val = vals[i];

        TreeLinkNode* tln = (TreeLinkNode*)malloc(sizeof(TreeLinkNode));
        if (tln == NULL) {   // may cause memory leak
            //should delete the nodes which have been new-ed
            return NULL;
        }

        tln->val = val;
        tln->left = NULL;
        tln->right = NULL;
        tln->next = NULL;

        if (root == NULL) {
            root = tln;
            continue;
        }

        p = root;
        while (1) {
            if (val == p->val ){
                free(tln);
                tln = NULL;
                break;
            } else if (val > p->val) {
                if (p->right == NULL) {
                    p->right = tln;
                    break;
                } else {
                    p = p->right;
                }
            } else {
                if (p->left == NULL) {
                    p->left = tln;
                    break;
                } else {
                    p = p->left;
                }
            }
        }
    }

    return root;
}

void connect(TreeLinkNode* root, TreeLinkNode** headlist) {
    if (NULL == root) return;

    TreeLinkNode* p = root;
    TreeLinkNode* newhead = root;
    TreeLinkNode* newtail = root;

    headlist[0] = root;

    int newline = 1;

    int i = 1;
    while(p != NULL) {
        if (p->left != NULL) {
            if (newline) {
                newhead = p->left;
                newtail = p->left;
                newline = 0;
                headlist[i] = newhead;
            } else {
                newtail->next = p->left;
                newtail = p->left;
            }
        }

        if (p->right != NULL) {
            if (newline) {
                newhead = p->right;
                newtail = p->right;
                newline = 0;
                headlist[i] = newhead;
            } else {
                newtail->next = p->right;
                newtail = p->right;
            }
        }

        p = p->next;
        if (p == NULL) {
            p = newhead;
            newhead = NULL;
            newtail = NULL;
            newline = 1;
            i ++;
        }
    }

}

int main(int argc, char **argv) {
    int vals[] = {-68,-96,-7,34,-99,-64,62,-36,-65,8,81,77,-57,26,93,87,8,-79,-59,-20};

    TreeLinkNode* tree = build_tree(vals, sizeof(vals)/sizeof(int));
    if (tree == NULL) {
        printf("Build tree failed!!\n");
        return -1;
    }

    TreeLinkNode** headlist = (TreeLinkNode**)malloc(100 * sizeof(TreeLinkNode*));
    if (headlist == NULL) {
        printf("Malloc headlist failed!!\n");
        return -1;
    }

    connect(tree, headlist);

    printf("The result is: \n");
    int i = 0;
    while (headlist[i] != NULL) {
        TreeLinkNode* p = headlist[i];
        while (p != NULL) {
            printf("%d ", p->val);
            p = p->next;
        }
        printf("\n");
        i++;
    }

    return 0;

}
