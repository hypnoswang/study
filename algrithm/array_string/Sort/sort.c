#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"

void quick_sort(int* array, int low, int high) {
    if (low >= high) {
        return;
    }

    int i = low;
    int j = high;

    int std = array[low];

    while (i < j) {
        while (i < j && array[j] >= std) {
            j --;
        }

        while (i < j && array[i] < std) {
            i ++;
        }

        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }

    quick_sort(array, low, i);
    quick_sort(array, i+1, high);

}

void heap_print(int* h, int len) {
    for (int i = 1; i <= len; ++i) {
        printf("%d ", h[i]);
    }

    printf("\n");
}

void heap_adjust(int *a, int low, int high) {
    int cand = a[low];
    int idx = low;

    for (int i = 2 * low; i <= high; i = i * 2) {
        if (i < high && a[i] < a[i+1]) {
            i ++;
        }

        if (cand >= a[i]) {
            break;
        }

        a[idx] = a[i];
        a[i] = cand;
        idx = i;
    }
}

void heap_sort(int* array, int len) {
    int* heap = (int*)malloc(sizeof(int)*(len+1));
    if (heap == NULL) return;

    memcpy(heap+1, array, len * sizeof(int));

    for (int i = len / 2; i > 0; --i) {
        heap_adjust(heap, i, len);
    }

    for (int i = len; i > 1; --i) {
        int tmp = heap[1];
        heap[1] = heap[i];
        heap[i] = tmp;
        heap_adjust(heap, 1, i-1);
    }

    for (int i = 1; i <= len; ++i) {
        printf("%d ", heap[i]);
    }
}

int main(int argc, char **argv) {
    printf("test1 after qsort:\n");
    quick_sort(test1, 0, sizeof(test1)/sizeof(int)-1);
    for (int i = 0; i < sizeof(test1)/sizeof(int); ++i) {
        printf("%d ", test1[i]);
    }
    printf("\n\n");

    printf("test after heap_sort:\n");
    heap_sort(test, sizeof(test)/sizeof(int));
    printf("\n\n");
    
    printf("test2 after heap_sort:\n");
    heap_sort(test2, sizeof(test2)/sizeof(int));
    printf("\n\n");

    printf("test2 after qsort:\n");
    quick_sort(test2, 0, sizeof(test2)/sizeof(int)-1);
    for (int i = 0; i < sizeof(test2)/sizeof(int); ++i) {
        printf("%d ", test2[i]);
    }
    printf("\n\n");


    return 0;
}
