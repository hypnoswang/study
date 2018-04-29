#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "odd_even.h"

ListNode* get_list(int n) {
    if (n <= 0) {
        return NULL;
    }

    int t = n;
    ListNode* head = NULL;
    ListNode* tail = NULL;
    for (int i = 1; i <= n; ++i) {
        ListNode* node = (ListNode*)malloc(sizeof(ListNode)); 
        if (node == NULL) {
            return NULL;
        }
        node->val = i;
        node->next = NULL;
        if (head == NULL) {
            head = node;
        }
        if (tail != NULL) {
            tail->next = node;
        }
        tail = node;
    }

    return head;
}

ListNode* impl1(ListNode* head) {
    ListNode* ohead = NULL;
    ListNode* otail = NULL;
    ListNode* ehead = NULL;
    ListNode* etail = NULL;

    ListNode* p = head;
    int cnt = 1;
    while(p != NULL) {
        ListNode* c = p;
        p = p->next;
        c->next = NULL;

        if (cnt % 2 == 1) {
            if (ohead == NULL) {
                ohead = c;
            }

            if (otail != NULL) {
                otail->next = c;
            }

            otail = c;
        } else {
            if (ehead == NULL) {
                ehead = c;
            }

            if (etail != NULL) {
                etail->next = c;
            }

            etail = c;
        }

        ++ cnt;
    }

    if (otail != NULL) {
        otail->next = ehead;
    }

    return ohead;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: oddeven <count>\n");
        return -1;
    }

    int cnt = atoi(argv[1]);
    if (cnt <= 0) {
        printf("Input count is invalid\n");
        return -1;
    }

    ListNode* head = get_list(cnt);
    if (head == NULL) {
        printf("Get input list failed\n");
        return -1;
    }

    ListNode* rv = impl1(head);
    if (rv == NULL) {
        printf("impl1 failed\n");
        return -1;
    }

    ListNode* p = rv;
    while(p != NULL) {
        printf("%d ", p->val);
        p = p->next;
    }

    printf("\n");

    return 0;
}
