#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "add_2nums.h"

ListNode* get_list(int n) {
    if (n == 0) {
       ListNode* node = (ListNode*)malloc(sizeof(ListNode)); 
       if (node == NULL) {
           return NULL;
       }
       node->val = 0;
       node->next = NULL;
       return node;
    }

    int t = n;
    ListNode* head = NULL;
    ListNode* tail = NULL;
    while (t > 0) {
       ListNode* node = (ListNode*)malloc(sizeof(ListNode)); 
       if (node == NULL) {
           return NULL;
       }
       node->val = t % 10;
       node->next = NULL;
       if (head == NULL) {
           head = node;
       }
       if (tail != NULL) {
           tail->next = node;
       }
       tail = node;

       t = t / 10;
    }

    return head;
}

ListNode* impl1(ListNode* l1, ListNode* l2) {
    ListNode* p1 = l1;
    ListNode* p2 = l2;
    ListNode* head = NULL;
    ListNode* tail = NULL;

    bool exceed = false;
    while(p1 != NULL && p2 != NULL) {
        ListNode* node = (ListNode*)malloc(sizeof(ListNode)); 
        if (node == NULL) {
            return NULL;
        }

        node->next = NULL;
        if ((p1->val + p2->val) >= 10) {
            node->val = (p1->val + p2->val) % 10;
            if (exceed) {
                node->val ++;
            }
            exceed = true;
        } else {
            node->val = p1->val + p2->val;
            if (exceed) {
                if ((node->val+1) >= 10) {
                    node->val = (node->val+1) % 10;
                    exceed = true;
                } else {
                    node->val ++;
                    exceed = false;
                }
            }
        }

        if (head == NULL) {
            head = node;
        }

        if (tail != NULL) {
            tail->next = node;
        }

        tail = node;

        p1 = p1->next;
        p2 = p2->next;
    }

    ListNode* p3 = (p1 != NULL) ? p1 : ((p2 != NULL) ? p2 : NULL);
    while (p3 != NULL) {
        ListNode* node = (ListNode*)malloc(sizeof(ListNode)); 
        if (node == NULL) {
            return NULL;
        }

        node->next = NULL;
        node->val = p3->val;
        if (exceed) {
            if ((p3->val+1) >= 10) {
                node->val = (p3->val+1) % 10;
                exceed = true;
            } else {
                node->val = p3->val + 1;
                exceed = false;
            }
        }

        if (head == NULL) {
            head = node;
        }

        if (tail != NULL) {
            tail->next = node;
        }

        tail = node;

        p3 = p3->next;
    }

    if (exceed) {
        ListNode* node = (ListNode*)malloc(sizeof(ListNode)); 
        if (node == NULL) {
            return NULL;
        }

        node-> next = NULL;
        node->val = 1;
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

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: add2num <num1> <num2>\n");
        return -1;
    }

    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);

    ListNode* h1 = get_list(n1);
    ListNode* h2 = get_list(n2);
    if (h1 == NULL || h2 == NULL) {
        printf("Get input list failed!\n");
        return -1;
    }

    ListNode* h3 = impl1(h1, h2);
    if (h3 == NULL) {
        printf("Impl1 failed!\n");
        return -1;
    }

    int res = 0;
    int e = 0;
    ListNode* p = h3;
    while (p != NULL) {
        res = res + p->val * pow(10, e);
        e++;
        p = p->next;
    }

    printf("The result of %s plus %s is %d\n", argv[1], argv[2], res);

    return 0;
}
