#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void swap(int* first, int* second) {
    int help = *first;
    *first = *second;
    *second = help;
}

void heapify(int arr[], int quantity, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < quantity && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < quantity && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, quantity, largest);
    }
}

void buildHeap(int arr[], int quantity) {
    for (int i = quantity / 2 - 1; i >= 0; i--) {
        heapify(arr, quantity, i);
    }
}



int main() {
    int quantity;
    if (!scanf("%d", &quantity) || quantity < 0) {
        return -1;
    }
    int* initialarray = (int*)malloc(quantity * sizeof(int));
    if (initialarray == NULL) {
        return -1;
    }
    for (int i = 0; i < quantity; i++) {
        if (!scanf("%d", &initialarray[i])) {
            free(initialarray);
            return -1;
        }
    }
    buildHeap(initialarray, quantity);
    for (int i = quantity - 1; i >= 0; i--) {
        swap(&initialarray[0], &initialarray[i]);
        heapify(initialarray, i, 0);
    }

    for (int i = 0; i < quantity; i++) {
        printf("%d ", initialarray[i]);
    }
    free(initialarray);
    return 0;
}
