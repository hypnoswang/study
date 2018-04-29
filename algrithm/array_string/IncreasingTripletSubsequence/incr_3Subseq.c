#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "incr_3Subseq.h"


bool impl1(int *nums, int size) {
    int a = 0;
    int b = 0;
    int min = nums[0];
    int second_min = nums[0];
    int t1 = 0;
    int t2 = 0;
    for (int i = 0; i < size; ++i) {
        if (nums[i] > second_min && second_min > min) {
            printf("The result is [%d]:%d < [%d]:%d < [%d]:%d\n", a, min, b, second_min, i,nums[i]);
            return true;
        }

        if (nums[i] > nums[t1] && (t1 == t2 || nums[i] < nums[t2])) {
            t2 = i;
            a = t1;
            b = t2;
            min = nums[t1];
            second_min = nums[t2];
        } else if (nums[i] < nums[t1]) {
            t1 = i;
            t2 = i;
        }
    }

    printf("No Increasing-Triplet-Subsequence exists!\n");

    return false;
}

int main(int argc, char **argv) {
    impl1(test1, 6);
    impl1(test2, 6);
    impl1(test3, 3);
    impl1(test4, 4);
    impl1(test5, 7);

    impl1(test6, sizeof(test6)/sizeof(int));

    return 0;
}
