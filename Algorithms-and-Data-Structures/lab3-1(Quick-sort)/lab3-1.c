#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void swap(int* initialarray, int l, int r) {
    int temp = initialarray[l];
    initialarray[l] = initialarray[r];
    initialarray[r] = temp;
}

void quick_sort(int* initialarray, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    int pivot = initialarray[mid];
    int l = left, r = right;
    while (l <= r) {
        while (initialarray[l] < pivot) l++;
        while (initialarray[r] > pivot) r--;

        if (l <= r) {
            swap(initialarray, l, r);
            l++;
            r--;
        }
    }

    quick_sort(initialarray, left, r);
    quick_sort(initialarray, l, right);
}



int main() {
    int quantity;
    if (!scanf("%d", &quantity) || quantity < 0) return -1;
    int* initialarray = (int*)malloc(quantity * sizeof(int));
    if (initialarray == NULL) return -1;
    for (int i = 0; i < quantity; i++) {
        if (!scanf("%d", &initialarray[i])) {
            free(initialarray);
            return -1;
        }
    }
    quick_sort(initialarray, 0, quantity - 1);
    for (int i = 0; i < quantity; i++) printf("%d ", initialarray[i]);
    free(initialarray);
    return 0;
}