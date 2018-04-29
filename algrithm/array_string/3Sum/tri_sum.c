#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "tri_sum.h"

//暴力查找
void impl1(int* haystack, int len) {
    if (len < 3) return;

    printf("\n");
    for (int i = 0; i < len-2; ++i) {
        for (int j = i+1; j < len-1; ++j) {
            for (int k = j+1; k < len; ++k) {
                int sum = haystack[i] + haystack[j] + haystack[k];
                if (sum == 0) {
                    printf("impl1\t%d %d %d\n", haystack[i], haystack[j], haystack[k]);
                }
            }
        }
    }
    printf("\n");
}

void quick_sort(int* haystack, int l, int u) {
    if (l >= u ) { 
        return;
    } 

    int std = haystack[l];
    int i = l+1;
    int j = u;

    while (1) {
        while (haystack[i] < std && i < j) {
            i++;
        }

        while (haystack[j] >= std && j >= i) {
            j--;
        }

        if (i < j) {
            int tmp = haystack[i];
            haystack[i] = haystack[j];
            haystack[j] = tmp; 
        } else { // i > j
            haystack[l] = haystack[j];
            haystack[j] = std; 
            quick_sort(haystack, l, j-1);
            quick_sort(haystack, j+1, u);
            break;
        }
    }
}

int binary_search(int *haystack, int l, int u, int needle) {
    if (l > u) {
        return -1;
    }

    int m = (l + u) / 2;
    int mv = haystack[m];

    if (mv == needle) {
        return m;
    } else if (needle > mv) {
        return binary_search(haystack, m+1, u, needle);
    } else {
        return binary_search(haystack, l, m-1, needle);
    }

}

int find_last_negative(int *haystack, int l, int u) {
    if (l > u) {
        return -1;
    }

    int m = (l + u) / 2;
    int mv = haystack[m];

    if (mv >= 0) {
        return find_last_negative(haystack, l, m-1);
    } else {
        if (haystack[m+1] >= 0) {
            return m;
        } else {
            return find_last_negative(haystack, m+1, u);
        }
    }
}

int find_first_positive(int *haystack, int l, int u) {
    if (l > u) {
        return -1;
    }

    int m = (l + u) / 2;
    int mv = haystack[m];

    if (mv <= 0) {
        return find_first_positive(haystack, m+1, u);
    } else {
        if (haystack[m-1] <= 0) {
            return m;
        } else {
            return find_first_positive(haystack, l, m-1);
        }
    }
}

void print_all_zeros(int* haystack, int len, int fzi) {
    int count = 0;
    for (int i = fzi; i < len && haystack[i] == 0; ++i) {
        count ++;
    }

    if (count > 2) {
        int num = (count * (count-1) * (count-2)) / 6;
        for (int i = 0; i < num; ++i) {
            printf("impl2\t0 0 0\n");
        }
    } 
}

void impl2(int* haystack, int len) {
    if (len < 3) return;

    printf("\n");
    quick_sort(haystack, 0, len-1);
    //for (int i = 0; i < len; ++i) {
    //    printf("%d ", haystack[i]);
    //}
    //printf("\n");
    int ni = find_last_negative(haystack, 0, len-1);
    if (ni < 0) {
        if (haystack[0] == 0) {
            print_all_zeros(haystack, len, 0);
        }
        return;
    } else if (ni < len-3) {
        print_all_zeros(haystack, len, ni + 1);
    }

    for (int i = 0; i < ni; ++i) {
        for (int j = i + 1; j <= ni; ++j) {
            int iv = haystack[i];
            int jv = haystack[j];
            int needle = -(iv + jv); 
            int pi = binary_search(haystack, ni+1, len-1, needle);
            int tpi = pi + 1;
            if (pi > 0) {
                do {
                    printf("impl2\t%d %d %d\n", iv, jv, needle);
                    pi --;
                } while(pi > ni && haystack[pi] == needle);
            }

            while (haystack[tpi] == needle) {
                printf("impl2\t%d %d %d\n", iv, jv, needle);
                tpi ++;
                if (tpi >= len) break;
            }
        }
    }

    for (int i = ni + 1; i < len-1; ++i) {
        for (int j = i + 1; j <= len-1; ++j) {
            int iv = haystack[i];
            int jv = haystack[j];
            int needle = -(iv + jv); 
            int ni2 = binary_search(haystack, 0, ni, needle);
            int tni = ni2 + 1;
            if (ni2 >= 0) {
                do {
                    printf("impl2\t%d %d %d\n", iv, jv, needle);
                    ni2 --;
                } while(ni2 >= 0 && haystack[ni2] == needle);
            }

            while (haystack[tni] == needle) {
                printf("impl2\t%d %d %d\n", iv, jv, needle);
                tni ++;
                if (tni > ni) break;
            }
        }
    }
}

long long micro_now() {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000*1000 + tv.tv_usec;
}

int main(int argc, char** argv) {
    long long t1, t2; 

    t1 = micro_now();
    //printf("### stdarray len: %d\n", sizeof(stdarray)/sizeof(int));
    impl1(stdarray, sizeof(stdarray)/sizeof(int));
    t2 = micro_now();
    printf("#### impl1 time spent: %lld us", t2 - t1);

    int* stdarray1 = (int *)malloc(sizeof(stdarray));
    memcpy(stdarray1, stdarray, sizeof(stdarray));
    t1 = micro_now();
    //printf("### stdarray len: %d\n", sizeof(stdarray)/sizeof(int));
    impl2(stdarray1, sizeof(stdarray)/sizeof(int));
    t2 = micro_now();
    printf("#### impl2 time spent: %lld us", t2 - t1);
    free(stdarray1);


    return 0;
}
