#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

char* syntaxCheck1(int vertices_in_the_graph, int numberOfRibs) {
    if (vertices_in_the_graph > 5000 || vertices_in_the_graph < 0) {
        return "bad number of vertices";
    }
    int check = vertices_in_the_graph * (vertices_in_the_graph + 1) / 2;
    if (numberOfRibs > check || numberOfRibs < 0) {
        return "bad number of edges";
    }
    return "OK";
}

char* syntaxCheck2(int lengthRibs, int startRibs, int endRibs, int vertices_in_the_graph) {
    if (startRibs < 1 || endRibs < 1 || startRibs > vertices_in_the_graph || endRibs > vertices_in_the_graph) {
        return "bad vertex";
    }
    else if (lengthRibs < 0 || lengthRibs > INT_MAX) {
        return "bad length";
    }
    return "OK";
}




int nextIndex(long long int* min_values, int* visited, int vertices) {
    long long int minn = LLONG_MAX;
    int min_index = -1;
    for (int i = 0; i < vertices; i++) {
        if (visited[i] != 1 && (min_index == -1 || min_values[i] < minn)) {

            minn = min_values[i];
            min_index = i;
        }
    }
    return min_index;
}

void dijkstra(int start, int end, int vertices, int** matrix) {
    long long int* min_values = (long long int*)calloc(vertices, sizeof(long long int));
    int* visited = (int*)calloc(vertices, sizeof(int));

    for (int i = 0; i < vertices; i++) {
        min_values[i] = LLONG_MAX;
    }
    min_values[start - 1] = 0;

    for (int i = 0; i < vertices; i++) {
        int minn = nextIndex(min_values, visited, vertices);
        visited[minn] = 1;

        for (int j = 0; j < vertices; j++) {
            if (!visited[j] && matrix[minn][j] != 0 && min_values[minn] + matrix[minn][j] < min_values[j]) {
                min_values[j] = min_values[minn] + matrix[minn][j];
            }
        }

        for (int j = 0; j < vertices; j++) {
            if (!visited[j] && matrix[j][minn] != 0 && min_values[minn] + matrix[j][minn] < min_values[j]) {
                min_values[j] = min_values[minn] + matrix[j][minn];
            }
        }
    }
    if (min_values[0] == LLONG_MAX) {
        printf("oo");
    }
    else if (min_values[0] > INT_MAX) {
        printf("INT_MAX+");
    }
    else {
        printf("%lld", min_values[0]);
    }
    for (int i = 1; i < vertices; i++) {
        if (min_values[i] == LLONG_MAX) {
            printf(" oo");
        }
        else if (min_values[i] > INT_MAX) {
            printf(" INT_MAX+");
        }

        else {
            printf(" %lld", min_values[i]);
        }
    }
    printf("\n");
    int count = 0;
    for (int i = 0; i < vertices; i++) {
        if (matrix[i][end - 1] != 0 || matrix[end - 1][i] != 0) {
            count++;
        }
        if (count > 1) {
            break;
        }
    }
    if (min_values[end - 1] > INT_MAX && count > 1) {
        printf("overflow");
    }
    else {
        end -= 1;
        start -= 1;
        int* ver = (int*)malloc(vertices * sizeof(int));
        if (ver == NULL) {
            free(ver);
            free(min_values);
            free(visited);
            return;
        }
        ver[0] = end + 1;
        int prevVertexIndex = 0;

        int weight = min_values[end];
        int index_min;
        int index_max;
        while (end != start) {
            int found_path = 0;
            for (int i = 0; i < vertices; i++) {
                if (end >= i) {
                    index_max = end;
                    index_min = i;
                }
                else {
                    index_max = i;
                    index_min = end;
                }
                if (matrix[index_min][index_max] != 0) {
                    int temp = weight - matrix[index_min][index_max];
                    if (temp == min_values[i]) {
                        weight = temp;
                        end = i;
                        ver[++prevVertexIndex] = i + 1;
                        found_path = 1;
                        break;
                    }
                }
            }
            if (!found_path) {
                break;
            }
        }

        for (int i = 0; i <= prevVertexIndex; i++) {
            printf("%d", ver[i]);
            printf(" ");
        }
        free(ver);

    }
    free(min_values);
    free(visited);
}



int main() {
    int vertices_in_the_graph, start, end, numberOfRibs;
    if (scanf("%d", &vertices_in_the_graph) != 1) {
        return -1;
    }
    if (scanf("%d%d", &start, &end) != 2) {
        return -1;
    }
    if (scanf("%d", &numberOfRibs) != 1) {
        return -1;
    }

    if (vertices_in_the_graph <= 0) {
        printf("bad vertex");
        return 0;
    }

    char* syntax = syntaxCheck1(vertices_in_the_graph, numberOfRibs);
    if (strcmp(syntax, "bad number of vertices") == 0) {
        printf("bad number of vertices");
        return 0;
    }
    else if (strcmp(syntax, "bad number of edges") == 0) {
        printf("bad number of edges");
        return 0;
    }

    if (numberOfRibs == 0) {
        if (start == end) {
            printf("%d\n", 0);
            printf("%d", start);
            return 0;
        }
        else {
            printf("%d oo\n", 0);
            printf("no path");
            return 0;
        }
    }

    int** array = (int**)calloc(vertices_in_the_graph, sizeof(int*));
    if (array == NULL) {
        free(array);
        return -1;
    }

    for (int i = 0; i < vertices_in_the_graph; i++) {
        array[i] = (int*)calloc(vertices_in_the_graph, sizeof(int));
        if (array[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            return -1;
        }
    }



    for (int i = 0; i < numberOfRibs; i++) {
        int startRibs, endRibs;
        int lengthRibs;
        if (scanf("%d%d%d", &startRibs, &endRibs, &lengthRibs) != 3) {
            for (int i = 0; i < vertices_in_the_graph; i++) {
                free(array[i]);
            }
            free(array);
            return 0;
        }
        char* syntax2 = syntaxCheck2(lengthRibs, startRibs, endRibs, vertices_in_the_graph);
        int min_index;
        int max_index;
       if (startRibs - 1 >= endRibs - 1) {
            min_index = endRibs - 1;
            max_index = startRibs - 1;
        }
        else {
            min_index = startRibs - 1;
            max_index = endRibs - 1;
        }

        array[min_index][max_index] = lengthRibs;

        if (strcmp(syntax2, "bad vertex") == 0) {
            printf("bad vertex");
            for (int i = 0; i < vertices_in_the_graph; i++) {
                free(array[i]);
            }
            free(array);
            return 0;
        }
        else if (strcmp(syntax2, "bad length") == 0) {
            printf("bad length");
            for (int i = 0; i < vertices_in_the_graph; i++) {
                free(array[i]);
            }
            free(array);
            return 0;
        }
        
    }

    dijkstra(start, end, vertices_in_the_graph, array);

    for (int i = 0; i < vertices_in_the_graph; i++) {
        free(array[i]);
    }
    free(array);
    return 0;
}
