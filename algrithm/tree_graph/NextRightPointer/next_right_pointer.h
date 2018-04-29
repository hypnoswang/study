#ifndef NEXT_RIGHT_POINTER_H
#define NEXT_RIGHT_POINTER_H

typedef struct TreeLinkNode {
    int val;
    struct TreeLinkNode *left;
    struct TreeLinkNode *right;
    struct TreeLinkNode *next;
} TreeLinkNode;

#endif
