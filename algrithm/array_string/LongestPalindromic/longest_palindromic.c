#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "longest_palindromic.h"

void impl1(const char *str, int len) {
    int s = 0;
    int e = 0;
    int max = 1;

    int i = 0;
    while (i < len) {
        //step 1, find the consecutive repeated characters
        int j = i;
        while (j < len && str[i] == str[j]) {
            j ++;
        }

        if (j >= len && (len-i) > max) {
            if (len-i > max) {
                s = i;
                e = len-1;
                max = len - i;
            }
            break;
        }

        //step 2, find the palindromic string based current i and u
        int l = i - 1;
        int u = j;
        while (l >= 0 && u <= len-1 && str[l] == str[u]) {
            l --;
            u ++;
        }

        if (u - l - 1 > max) {
            s = l + 1;
            e = u - 1;
            max = u - l -1;
        }

        i = j;
    }

    char *rv = (char *)malloc(max + 1);
    memset(rv, 0, max+1);
    memcpy(rv, &str[s], max);

    printf("The longest palindromic of %s is: %s\n", str, rv);

    free(rv);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: lpal <string>\n");
        return -1;
    }

    impl1(argv[1], strlen(argv[1]));

    return 0;
}
