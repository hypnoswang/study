#include <iostream>
#include <vector>

#include "binary_tree.h"

int main(int argc, char **argv) {
    std::vector<int> vals = {-68,-96,-7,34,-99,-64,62,-36,-65,8,81,77,-57,26,93,87,8,-79,-59,-20};

    bt::BinaryTree mytree;
    if (!mytree.init(vals)) {
        std::cerr << "Initialize tree failed" << std::endl;
        return -1;
    }

    std::vector<int> res1; 
    mytree.inorder_traversal1(res1);
    std::cout << "After inorder_traversal1:" << std::endl;
    for (std::size_t i = 0; i < res1.size(); ++i) {
        std::cout << res1[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<int> res2; 
    mytree.inorder_traversal2(res2);
    std::cout << "After inorder_traversal2:" << std::endl;
    for (std::size_t i = 0; i < res2.size(); ++i) {
        std::cout << res2[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<int> res3; 
    mytree.preorder_traversal1(res3);
    std::cout << "After preorder_traversal1:" << std::endl;
    for (std::size_t i = 0; i < res3.size(); ++i) {
        std::cout << res3[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<int> res4; 
    mytree.preorder_traversal2(res4);
    std::cout << "After preorder_traversal2:" << std::endl;
    for (std::size_t i = 0; i < res4.size(); ++i) {
        std::cout << res4[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<int> res5; 
    mytree.postorder_traversal1(res5);
    std::cout << "After postorder_traversal1:" << std::endl;
    for (std::size_t i = 0; i < res5.size(); ++i) {
        std::cout << res5[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector<int> res6; 
    mytree.postorder_traversal2(res6);
    std::cout << "After postorder_traversal2:" << std::endl;
    for (std::size_t i = 0; i < res6.size(); ++i) {
        std::cout << res6[i] << " ";
    }
    std::cout << std::endl << std::endl;

    std::vector< std::vector<int> > res7; 
    mytree.zigzag_traversal(res7);
    std::cout << "After zigzag_traversal:" << std::endl;
    //expected: [[-68], [-7, -96], [-99, -79, -64, 34], [62, 8, -36, -65], [-57, -20, 26, 81], [93, 77, -59], [87]]
    for (std::size_t i = 0; i < res7.size(); ++i) {
        std::vector<int> r = res7[i];
        for (std::size_t j = 0; j < r.size(); ++j) {
            std::cout << r[j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;

    int elem_1st = mytree.kth_smallest_elem(1);
    std::cout << "The 1st smallest element is " << elem_1st << std::endl;
    int elem_8th = mytree.kth_smallest_elem(8);
    std::cout << "The 8th smallest element is " << elem_8th << std::endl;
    int elem_19th = mytree.kth_smallest_elem(19);
    std::cout << "The 19th smallest element is " << elem_19th << std::endl;

    return 0;

}
