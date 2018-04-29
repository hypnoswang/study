#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "intersection.h"

ListNode* common = NULL;

void make_list(ListNode **h, ListNode **t, int c) {
    for (int i = 0; i < c; ++i) {
        ListNode* node = (ListNode*)malloc(sizeof(ListNode));
        if (node == NULL) {
            // memleak here
            return;
        }

        node->val = time(NULL) + i;
        node->next = NULL;
        if (*h == NULL) {
            *h = node;
        }

        if (*t != NULL) {
            (*t)->next = node;
        }

        *t = node;

        //sleep(1);
    }
}

ListNode* get_list(int type, int c) {
    ListNode* head = NULL;
    ListNode* tail = NULL;

    make_list(&head, &tail, c);

    switch (type) {
        case 0:
        case 1:
            break;
        case 2:
            if (tail != NULL && common != NULL) {
                tail->next = common;
            }
            break;
        default:
            return NULL;
    }

    return head;
}

ListNode* reverse(ListNode* head) {
    ListNode* new_head = NULL;
    ListNode* new_tail = NULL;
    ListNode* p = head;

    while(p != NULL) {
        ListNode* cur = p;
        p = p->next;
        cur->next = NULL;

        if (new_head == NULL) {
            new_head = cur;
        }

        if (new_tail != NULL) {
            new_tail->next = cur;
        }

        new_tail = cur;
    }

    return new_head;
}

int len(ListNode *head) {
    int cnt = 0;

    ListNode* p = head;
    while(p != NULL) {
        cnt++;
        p = p->next;
    }

    return cnt;
}

ListNode* impl1(ListNode* headA, ListNode* headB) {
    if (headA == headB) {
        return headA;
    }

    if (headA == NULL || headB == NULL) {
        return NULL;
    }

    int lenA = len(headA);
    int lenB = len(headB);
    int diff = lenA > lenB ? (lenA - lenB) : (lenB - lenA); 
    ListNode* p1 = lenA > lenB ? headA : headB;
    ListNode* p2 = lenA > lenB ? headB : headA;
    for (int i = 0; i < diff; ++i) {
        p1 = p1->next;
    }

    while (p1 != NULL && p2 != NULL) {
        if (p1 == p2) {
            return p1;
        }

        p1 = p1->next;
        p2 = p2->next;
    }

    return NULL;
}

int main(int argc, char ** argv) {
    common = get_list(0, 7); 
    printf("The val of the first common node is: %d\n", common->val);

    ListNode* ha1 = get_list(2, 9);
    ListNode* hb1 = get_list(2, 13);
    ListNode* int1 = impl1(ha1, hb1);
    printf("The intersection of ha1 and hb1 is: %d\n", int1->val);

    ListNode* ha2 = get_list(2, 7);
    ListNode* hb2 = get_list(2, 7);
    ListNode* int2 = impl1(ha2, hb2);
    printf("The intersection of ha2 and hb2 is: %d\n", int2->val);

    ListNode* ha3 = get_list(2, 7);
    ListNode* hb3 = get_list(1, 7);
    ListNode* int3 = impl1(ha3, hb3);
    printf("The intersection of ha3 and hb3 is: %p\n", int3);

    ListNode* ha4 = get_list(2, 7);
    ListNode* hb4 = common;
    ListNode* int4 = impl1(ha4, hb4);
    printf("The intersection of ha4 and hb4 is: %d\n", int4->val);

    return 0;
}


