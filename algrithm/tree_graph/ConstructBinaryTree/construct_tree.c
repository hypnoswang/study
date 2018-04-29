#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "construct_tree.h"

TreeNode* build_tree(int* preorder, int preorderSize, int* inorder, int inorderSize) {
    if (preorderSize != inorderSize) return NULL;

    if (preorderSize < 1) {
        return NULL;
    }

    TreeNode* tn = (TreeNode*)malloc(sizeof(TreeNode));
    if (tn == NULL) return NULL;
    tn->val = preorder[0];

    int i = 0;
    while (i < inorderSize && preorder[0] != inorder[i]) {
        i++;
    }

    if (i >= inorderSize) return NULL;

    if (i == 0) {
        tn->left = NULL;
        tn->right = build_tree(preorder+1, preorderSize-1,
                inorder+1, inorderSize-1);
    } else if (i == (inorderSize-1)) {
        tn->right = NULL;
        tn->left = build_tree(preorder+1, preorderSize-1,
                inorder, inorderSize-1);
    } else {
        tn->left = build_tree(preorder+1, i,
                inorder, i);
        tn->right = build_tree(preorder+i+1, preorderSize-i-1,
                inorder+i+1, inorderSize-i-1);
    }

    return tn;
}

void inorder_traversal(TreeNode* root) {
    if (root == NULL) return;

    if (root->left != NULL) inorder_traversal(root->left);

    printf("%d ", root->val);

    if (root->right != NULL) inorder_traversal(root->right);
}

int main(int argc, char **argv) {
    int pre[] = {-68,-96,-99,-79,-7,-64,-65,-36,-57,-59,-20,34,8,26,62,81,77,93,87};
    int  in[] = {-99,-96,-79,-68,-65,-64,-59,-57,-36,-20,-7,8,26,34,62,77,81,87,93};
    char instr[] = "-99 -96 -79 -68 -65 -64 -59 -57 -36 -20 -7 8 26 34 62 77 81 87 93";

    TreeNode* root = build_tree(pre, 19, in, 19);
    if (root == NULL) {
        printf("Build Tree failed!!\n");
        return -1;
    }

    printf("Expected: %s\n", instr);
    printf("ValueGet: ");
    inorder_traversal(root);
    printf("\n");

    return 0;
}
