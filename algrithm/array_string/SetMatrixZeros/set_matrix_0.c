#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set_matrix_0.h"

void print_mx(int mx[][8], int m) {
    int n = 8;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d\t", mx[i][j]);
        }
        printf("\n");
    }
}

void impl1() {
    int m = 10;
    int n = 8;

    int cols[8] = {0};

    for (int i = 0; i < m; ++i) {
        int flag = 0;
        for (int j = 0; j < n; ++j) {
            if (mx[i][j] == 0) {
                cols[j] = 1;
                flag = 1;
            }
        }
        if (flag) {
            memset(&mx[i][0], 0, sizeof(int)*8);
        }
    }

    for (int i = 0; i < 8; ++i) {
        if (cols[i]) {
            for (int j = 0; j < m; ++j) {
                mx[j][i] = 0;
            }
        }
    }
}

void impl2() {
    int m = 10;
    int n = 8;

    int flag_of_line0 = 0;
    for (int i = 0; i < m; ++i) {
        int flag_of_this_line = 0;
        for (int j = 0; j < n; ++j) {
            if (mx[i][j] == 0) {
                if (i != 0) {
                    mx[0][j] = 0;
                    flag_of_this_line = 1;
                } else {
                    flag_of_line0 = 1;
                }
            }
        }
        if (flag_of_this_line) {
            memset(&mx[i][0], 0, sizeof(int)*n);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (mx[0][i] == 0) {
            for (int j = 1; j < m; ++j) {
                mx[j][i] = 0;
            }
        }
    }

    if (flag_of_line0) {
        memset(&mx[0][0], 0, sizeof(int) * n);
    }
    
}

void impl3(int **mtx, int r, int c) {
    int m = r;
    int n = c;

    int* b = (int *)mtx;

    if (m <= 0 || n <= 0) {
        return;
    }

    int flag_of_line0 = 0;
    for (int i = 0; i < m; ++i) {
        int flag_of_this_line = 0;
        for (int j = 0; j < n; ++j) {
            if (*(b + i*n + j) == 0) {
                if (i != 0) {
                    *(b + j) = 0;
                    flag_of_this_line = 1;
                } else {
                    flag_of_line0 = 1;
                }
            }
        }
        if (flag_of_this_line) {
            memset(b + i*n, 0, sizeof(int)*n);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (*(b + i) == 0) {
            for (int j = 1; j < m; ++j) {
                *(b + j*n + i) = 0;
            }
        }
    }

    if (flag_of_line0) {
        memset(b, 0, sizeof(int) * n);
    }
    
}

int main(int argc, char **argv) {
    print_mx(mx, 10);

    //impl1();
    //printf("\nAfter impl1:\n\n");
    //print_mx(mx, 10);

    //impl2();
    //printf("\nAfter impl2:\n\n");
    //print_mx(mx, 10);

    impl3((int **)mx, 10, 8);
    printf("\nAfter impl3:\n\n");
    print_mx(mx, 10);

    return 0;
}
