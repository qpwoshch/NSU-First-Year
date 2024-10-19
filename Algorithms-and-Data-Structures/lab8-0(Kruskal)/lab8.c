#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX_VERTEX_COUNT 5000

typedef struct
{
    int start;
    int end;
    int weight;
} edge_data;

char* syntaxCheckEdge(int start, int end, unsigned long long int length, int vertex) {
    long long int length1 = (long long int)length;
    if (start < 1 || start > vertex || end < 1 || end > vertex) {
        return "bad vertex";
    }
    else if (length1 < 0 || length1 > INT_MAX) {
        return "bad length";
    }
    return "OK";
}

char* SyntaxCheckQuantityVertexEdge(int vertex, int edge) {
    int check = vertex * (vertex + 1) / 2;
    if (vertex < 0 || vertex > MAX_VERTEX_COUNT) {
        return "bad number of vertices";
    }
    if (edge < 0 || edge > check) {
        return "bad number of edges";
    }
    if (vertex == 0 || (edge == 0 && vertex != 1)) {
        return "no spanning tree";
    }
    if (edge < vertex - 1) {
        return "no spanning tree";
    }
    return "OK";
}

void swap(edge_data* arr, int left, int right) {
    edge_data tmp;
    tmp.start = arr[left].start;
    tmp.end = arr[left].end;
    tmp.weight = arr[left].weight;
    arr[left].start = arr[right].start;
    arr[left].end = arr[right].end;
    arr[left].weight = arr[right].weight;
    arr[right].start = tmp.start;
    arr[right].end = tmp.end;
    arr[right].weight = tmp.weight;
}

void quick_sort(edge_data* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        int pivot = arr[mid].weight;
        int l = left;
        int r = right;
        while (l <= r) {
            while (arr[l].weight < pivot) {
                l++;
            }
            while (arr[r].weight > pivot) {
                r--;
            }
            if (l <= r) {
                swap(arr, l, r);
                l++;
                r--;
            }
        }
        quick_sort(arr, left, r);
        quick_sort(arr, l, right);
    }
}

 void init(int* set, int vertex) {
    for (int i = 0; i < vertex; i++) {
        set[i] = i;
    }
}


 int find(int key, int* set) {
     while (key != set[key]) {
         key = set[key];
     }
     return key;
 }

void unite(int x, int y, int* set, int* rank) {
    x = find(x, set);
    y = find(y, set);
    if (rank[x] < rank[y]) {
        set[x] = y;
    }
    else {
        set[y] = x;
        if (rank[x] == rank[y]) {
            ++rank[x];
        }
    }
}


void kraskal(edge_data* arr, int vertex, int edge, int* set, int* frame, int *pos) {
    int* rank = (int*)calloc(vertex, sizeof(int));
    if (rank == NULL) {
        free(rank);
        return;
    }
    for (int i = 0; i < edge; i++) {

        int start = find(arr[i].start, set);
        int end = find(arr[i].end, set);
        if (start != end) {
            unite(start, end, set, rank);
            frame[*pos] = i;
            (*pos)++;
        }
        
    }
    free(rank);
}


int main() {
    int vertex, edge;
    if (scanf("%d", &vertex) != 1) {
        return -1;
    }
    if (scanf("%d", &edge) != 1) {
        return -1;
    }
    char* syntax1 = SyntaxCheckQuantityVertexEdge(vertex, edge);
    if (vertex == 1) {
        return 0;
    }
    else if (strcmp(syntax1, "bad number of vertices") == 0) {
        printf("bad number of vertices");
        return 0;
    }
    else if (strcmp(syntax1, "bad number of edges") == 0) {
        printf("bad number of edges");
        return 0;
    }
    else if (strcmp(syntax1, "no spanning tree") == 0) {
        printf("no spanning tree");
        return 0;
    }
    edge_data* arr = (edge_data*)malloc(edge * sizeof(edge_data));
    if (arr == NULL) {
        free(arr);
        return -1;
    }
    for (int i = 0; i < edge; i++) {
        int start, end, length;
        unsigned long long int tryLength;
        if (scanf("%d%d%llu", &start, &end, &tryLength) == EOF) {
            printf("bad number of lines");
            free(arr);
            return 0;
        }
        char* syntax2 = syntaxCheckEdge(start, end, tryLength, vertex);
        if (strcmp(syntax2, "bad vertex") == 0) {
            printf("bad vertex");
            free(arr);
            return 0;
        }
        else if (strcmp(syntax2, "bad length") == 0) {
            printf("bad length");
            free(arr);
            return 0;
        }
        else {
            length = (int)tryLength;
        }
        arr[i].start = start-1;
        arr[i].end = end-1;
        arr[i].weight = length;
    }
    quick_sort(arr, 0, edge - 1);
    int* set = (int*)calloc(vertex, sizeof(int));
    if (set == NULL) {
        free(arr);
        free(set);
        return 0;
    }
    init(set, vertex);
    int* frame = (int*)malloc(sizeof(int) * edge);
    if (frame == NULL) {
        free(arr);
        free(set);
        free(frame);
        return 0;
    }
    int pos = 0;
    kraskal(arr, vertex, edge, set, frame, &pos);
    int components = 0;
    for (int i = 0; i < vertex; i++) {
        if (set[i] == i) {
            components++;
        }
    }
    if (components > 1) {
        printf("no spanning tree");
    }
    else {
        for (int i = 0; i < pos; i++) {
            printf("%d %d\n", arr[frame[i]].start + 1, arr[frame[i]].end + 1);
        }
    }
    free(frame);
    free(set);
    free(arr);
    return 0;
}
