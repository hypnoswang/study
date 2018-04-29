#include <iostream>
#include <vector>

#include "link_list.h"


template<class T>
Node<T>::Node(T& v):val_(v), next_(NULL) {
}

template<class T>
Node<T>::~Node(){
}



int main(int argc, char** argv) {
    int nums[] = {250,-136,864,940,-931,-368,94,-981,413,368,835,320,-148,859,-320,852,-347,461,-406,-528};

    std::vector<int> vals;
    for (int i = 0; i < sizeof(nums)/sizeof(int); ++i) {
        vals.push_back(nums[i]);
    }

    Node<int>* head = build_list<int>(vals);
    if (head == NULL) {
        std::cerr << "build list failed" << std::endl;
        return -1;
    }

    std::cout << "Before reverse: " << std::endl;
    print_list<int>(head);

    reverse_list<int>(&head);

    std::cout << "After reverse: " << std::endl;
    print_list<int>(head);

    return 0;
}
