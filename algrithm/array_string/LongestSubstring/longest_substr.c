#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "longest_substr.h"


void impl1(const char* str, int len) {
    int b = 0;
    int e = 0;
    int max = 0;
    int flags[127];

    int i = 0;
    memset(flags, -1, sizeof(flags));
    for (int j = 0; j < len; j++) {
        char c = str[j];
        if (flags[c] >= 0) {
            for (int t = i; t < flags[c]; ++t) {
                char tc = str[t];
                flags[tc] = -1;
            }
            i = flags[c] + 1;
        } 

        if ((j-i+1) > max) {
            printf("#######   j = %d, i = %d \n", j, i);
            max = j - i + 1;
            b = i;
            e = j;
        }

        flags[c] = j;
    }

    char* r = (char *)malloc(max + 1);
    if (r == NULL) {
        return;
    }

    memset(r, 0 , max + 1);
    memcpy(r, str + b, max);

    printf("The max substr len with no repeat characters of %s is: %d \n", str, max);
    printf("The max substr with no repeat characters of %s is: %s \n", str, r);
}

int main(int argc, char **argv) {
    char str1[] = "congratulations";
    char str2[] = "abcabcbb";
    char str3[] = "bbbbb";
    char str4[] = "pwwkew";
    char str5[] = "abc";

    impl1(str1, strlen(str1));
    impl1(str2, strlen(str2));
    impl1(str3, strlen(str3));
    impl1(str4, strlen(str4));
    impl1(str5, strlen(str5));

    impl1(test, strlen(test));

    return 0;
}
