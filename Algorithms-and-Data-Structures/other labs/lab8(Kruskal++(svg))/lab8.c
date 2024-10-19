#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#define MAX_VERTEX_COUNT 5000
#define M_PI 3.141593

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


void kraskal(edge_data* arr, int vertex, int edge, int* set, int* frame, int* pos) {
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

void svg(int vertex, edge_data* arr, int* frame, int pos, int edge) {
    FILE* file;
    file = fopen("file.svg", "w");
    if (file == NULL) {
        return;
    }
    fprintf(file, "<svg width=\"3000\" height=\"3000\" xmlns=\"http://www.w3.org/2000/svg\">\n<circle cx=\"300\" cy=\"300\" r=\"180\" fill=\"none\" stroke=\"black\" stroke-width=\"0\"/>\n");
    int** coordinate = (int**)malloc(vertex * sizeof(int*));
    if (coordinate == NULL) {
        fclose(file);
        return;
    }
    double PI = 2*M_PI;
    double quantity = PI / vertex;
    double radiusBig = 180;
    double radiusSmall = 20;
    for (int i = 0; i < vertex; i++) {
        coordinate[i] = (int*)malloc(2 * sizeof(int));
        if (coordinate[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(coordinate[j]);
            }
            free(coordinate);
            fclose(file);
            return;
        }
        double sinn = sin(quantity * i);
        double coss = cos(quantity * i);
        int x = (int)radiusBig * sinn + 300; 
        int y = (int)radiusBig * coss + 300;
        coordinate[i][0] = x;
        coordinate[i][1] = y;
        fprintf(file, "<circle cx=\"");
        fprintf(file, "%d", x);
        fprintf(file, "\" cy=\"");
        fprintf(file, "%d", y);
        fprintf(file, "\" r=\"20\" fill=\"none\" stroke=\"black\" stroke-width=\"2\"/>\n");
        fprintf(file, "<text text-anchor=\"middle\" x=\"");
        fprintf(file, "%d", x);
        fprintf(file, "\" y=\"");
        fprintf(file, "%d", y);
        fprintf(file, "\">");
        fprintf(file, "%d", i + 1);
        fprintf(file, "</text>\n");
        
        sinn = -sinn;
        coss = -coss;
        x = (int)radiusSmall * sinn + coordinate[i][0];
        y = (int)radiusSmall * coss + coordinate[i][1];
        coordinate[i][0] = x;
        coordinate[i][1] = y;
        
        
    }
    for (int i = 0; i < edge; i++) {
        int start = arr[i].start;
        int end = arr[i].end;
        int xstart = coordinate[start][0];
        int ystart = coordinate[start][1];
        int xend = coordinate[end][0];
        int yend = coordinate[end][1];
        fprintf(file, "<line x1=\"");
        fprintf(file, "%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:black;\" />", xstart, ystart, xend, yend);
    }
    for (int i = 0; i < pos; i++) {
        int start = arr[frame[i]].start;
        int end = arr[frame[i]].end;
        int xstart = coordinate[start][0];
        int ystart = coordinate[start][1];
        int xend = coordinate[end][0];
        int yend = coordinate[end][1];
        fprintf(file, "<line x1=\"");
        fprintf(file, "%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:red; stroke-width:2;\" />", xstart, ystart, xend, yend);

    }
    fprintf(file, "</svg>");
    for (int i = 0; i < vertex; i++) {
        free(coordinate[i]);
    }
    fclose(file);
    free(coordinate);
}


int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "-g") == 0) {
        setlocale(LC_ALL, "Ru");
        printf("Введите количество вершин и ребер\n");
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
        printf("Введите данные ребер(начало, конец и длинну)\n");
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
            arr[i].start = start - 1;
            arr[i].end = end - 1;
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
            printf("Должны быть выделены следующие ребра:\n");
            for (int i = 0; i < pos; i++) {
                printf("%d %d\n", arr[frame[i]].start + 1, arr[frame[i]].end + 1);
            }
        }
        svg(vertex, arr, frame, pos, edge);
        free(frame);
        free(set);
        free(arr);
        return 0;
    }
    else {
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
            arr[i].start = start - 1;
            arr[i].end = end - 1;
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
}
