#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <string.h>



typedef struct node {
    int data;
    unsigned char height;
    struct node* left;
    struct node* right;
} node;

node* create_node_array(node* tree_node, int verices) {
    if (tree_node != NULL) {
        tree_node->data = verices;
        tree_node->left = tree_node->right = NULL;
        tree_node->height = 1;
    }
    return tree_node;
}

unsigned char tree_size(node* root) {
    if (root != NULL) {
        return root->height;
    }
    else {
        return 0;
    }

}

int balancingCheck(node* root) {
    if (root == NULL) {
        return 0;
    }
    else {
        int leftsize = tree_size(root->left);
        int rightsize = tree_size(root->right);
        return rightsize - leftsize;
    }
}



void heightCorrection(node* root) {
    unsigned char l = tree_size(root->left);
    unsigned char r = tree_size(root->right);
    unsigned char maxx;
    if (l > r) {
        maxx = l;
    }
    else {
        maxx = r;
    }
    root->height = maxx + 1;
}

node* turnright(node* root) {
    node* tree = root->left;
    root->left = tree->right;
    tree->right = root;
    heightCorrection(root);
    heightCorrection(tree);
    return tree;
}

node* turnleft(node* root) {
    node* tree = root->right;
    root->right = tree->left;
    tree->left = root;
    heightCorrection(root);
    heightCorrection(tree);
    return tree;
}

node* balance(node* root) {
    if (balancingCheck(root) == 2) {
        if (balancingCheck(root->right) < 0) {
            root->right = turnright(root->right);
        }
        return turnleft(root);
    }
    if (balancingCheck(root) == -2) {
        if (balancingCheck(root->left) > 0) {
            root->left = turnleft(root->left);
        }
        return turnright(root);
    }
    return root;
}

node* insert(node* root, node* tree_node, int vertices) {
    if (root == NULL) {
        return create_node_array(tree_node, vertices);
    }
    if (vertices < root->data) {
        root->left = insert(root->left, tree_node, vertices);
    }
    else {
        root->right = insert(root->right, tree_node, vertices);
    }
    heightCorrection(root);
    return balance(root);
}

void printfree(node* root) {
    if (root != NULL) {
        printfree(root->left);
        printf("%d ", root->data);
        printfree(root->right);
    }
}

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "-t") == 0) {
        setlocale(LC_ALL, "Rus");
        printf("Введите количество вершин\n");
        int vertices;
        if (scanf("%d", &vertices) != 1) {
            return 0;
        }


        if (vertices == 0) {
            printf("%d\n", vertices);
            return 0;
        }

        node* root = (node*)malloc(vertices * sizeof(node));
        if (!root) {
            free(root);
            return -1;
        }
        int number;
        printf("Введите числа для добавления в АВЛ дерево\n");
        if (scanf("%d", &number) != 1) {
            free(root);
            return -1;
        }
        root[0].data = number;
        root[0].height = 1;
        root[0].left = root[0].right = NULL;
        node* head = &root[0];
        for (int i = 1; i < vertices; i++) {
            if (scanf("%d", &number) != 1) {
                free(root);
                return -1;
            }
            head = insert(head, &root[i], number);
        }
        int size = tree_size(head);
        printf("Размер дерева:\n");
        printf("%d\n", size);
        printf("само дерево:\n");
        printfree(head);
        free(root);
        return 0;
    }
    else {
        int vertices;
        if (scanf("%d", &vertices) != 1) {
            return 0;
        }


        if (vertices == 0) {
            printf("%d\n", vertices);
            return 0;
        }

        node* root = (node*)malloc(vertices * sizeof(node));
        if (!root) {
            free(root);
            return -1;
        }
        int number;
        if (scanf("%d", &number) != 1) {
            free(root);
            return -1;
        }
        root[0].data = number;
        root[0].height = 1;
        root[0].left = root[0].right = NULL;
        node* head = &root[0];
        for (int i = 1; i < vertices; i++) {
            if (scanf("%d", &number) != 1) {
                free(root);
                return -1;
            }
            head = insert(head, &root[i], number);
        }
        unsigned char size = tree_size(head);
        printf("%d\n", size);
        free(root);
        return 0;
    }
}
