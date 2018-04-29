#include <stdio.h>


int main(int argc, char **argv) {
    int ba[10] = {0};
    int* base = ba;

    printf("base: %p\n", base);
    printf("base + 1: %p\n", base+1);
    printf("base + 2: %p\n", base+2);
    printf("base + 6: %p\n", base+6);

    return 0;
}
