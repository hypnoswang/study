#ifndef LINK_LIST_H
#define LINK_LIST_H

#include <iostream>
#include <vector>

template<class T> 
class Node {
    public:
        explicit Node(T& v);
        virtual ~Node();

        inline T& getv() { return val_; }
        inline void setv(T& v) { val_ = v; }

        inline Node* getp() { return next_; }
        inline void setp(Node* p) { next_ = p; }

    private:
        T val_;
        Node *next_;
};


template<class T>
Node<T>* build_list(std::vector<T>& vals) {
    Node<T>* head = NULL;

    typename std::vector<T>::iterator it = vals.begin();
    for (; it != vals.end(); ++it) {
        Node<T>* p = new Node<T>(*it);
        if (p == NULL) {
            // error handle
            // handle the already allocated nodes
            return NULL;
        }

        p->setp(head);
        head = p;
    }

    return head;
}

template<class T>
void print_list(Node<T>* head) {
    if (head == NULL) return;

    Node<T>* p = head;
    while (p != NULL) {
        std::cout << p->getv() << " ";
        p = p->getp();
    }
    std::cout << std::endl;
    std::cout << std::endl;

    return;
}

template<class T>
void reverse_list(Node<T>** head) {
    if (head == NULL || *head == NULL) {
        return;
    }

    Node<T>* c = (*head)->getp();
    Node<T>* p = *head;
    Node<T>* n = c;

    p->setp(NULL);
    while (c != NULL) {
        n = c->getp();
        c->setp(p);
        p = c;
        c = n;
    }

    *head = p;
        
    return;
}



#endif
