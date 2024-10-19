#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 11000
#define I_AM_NO_A_VALID_RESULT 9999

typedef int StackElement;

struct Stack {
    StackElement data[MAX_STACK_SIZE];
    size_t size;
};

void stack_init(struct Stack* s) {
    s->size = 0;
}

void stack_push(struct Stack* s, StackElement x) {
    if (s->size < MAX_STACK_SIZE) {
        s->data[s->size] = x;
        s->size++;
    }
}

StackElement stack_pop(struct Stack* s) {
    if (s->size > 0) {
        s->size--;
        return s->data[s->size];
    }
    else {
        return I_AM_NO_A_VALID_RESULT;
    }
}


int calculation(char* string) {
    struct Stack stack;
    stack_init(&stack);
    char* substring = strtok(string, " ");
    while (substring != NULL) {
        if (isdigit(substring[0])) {
            int num = atoi(substring);
            stack_push(&stack, num);
        }
        else {
            int b = stack_pop(&stack);
            int a = stack_pop(&stack);
            if (substring[0] == '+') {
                stack_push(&stack, a + b);
            }
            else if (substring[0] == '-') {
                stack_push(&stack, a - b);
            }
            else if (substring[0] == '*') {
                stack_push(&stack, a * b);
            }
            else if (substring[0] == '/') {
                if (b == 0) {
                    printf("division by zero");
                    return -1;
                }
                stack_push(&stack, a / b);
            }
        }
        substring = strtok(NULL, " ");
    }
    return stack_pop(&stack);
}




char* toPolishEntry(const char* input, char* polishEntry) {
    struct Stack stack;
    stack_init(&stack);
    size_t outputIndex = 0;
    for (size_t i = 0; i < strlen(input); i++) {
        char element = input[i];
        if (isdigit((unsigned char)element)) {
            while (isdigit((unsigned char)input[i])) {
                polishEntry[outputIndex++] = input[i++];
            }
            polishEntry[outputIndex++] = ' ';
            i--;
        }
        else if (element == '(') {
            stack_push(&stack, element);
        }
        else if (element == ')') {
            while (stack.size > 0 && stack.data[stack.size - 1] != '(') {
                polishEntry[outputIndex++] = stack_pop(&stack);
                polishEntry[outputIndex++] = ' ';
            }
            if (stack.size > 0 && stack.data[stack.size - 1] == '(') {
                stack_pop(&stack); 
            }
        }
        else if (element == '+' || element == '-') {
            while (stack.size > 0 && (stack.data[stack.size - 1] == '+' || stack.data[stack.size - 1] == '-' || stack.data[stack.size - 1] == '*' || stack.data[stack.size - 1] == '/')) {
                polishEntry[outputIndex++] = stack_pop(&stack);
                polishEntry[outputIndex++] = ' ';
            }
            stack_push(&stack, element);
        }
        else if (element == '*' || element == '/') {
            while (stack.size > 0 && (stack.data[stack.size - 1] == '*' || stack.data[stack.size - 1] == '/')) {
                polishEntry[outputIndex++] = stack_pop(&stack);
                polishEntry[outputIndex++] = ' ';
            }
            stack_push(&stack, element);
        }
    }
    while (stack.size > 0) {
        polishEntry[outputIndex++] = stack_pop(&stack);
        polishEntry[outputIndex++] = ' ';
    }
    polishEntry[outputIndex] = '\0';
    return polishEntry;
}


int SyntaxError(const char* string, int null) {
    int bracketCount = 0;
    if (!isdigit(string[0]) && string[0] != '(') {
        return -1;
    }
    if (!isdigit(string[null - 1]) && string[null - 1] != ')') {
        return -1;
    }
    for (int i = 0; i < null; i++) {
        if (!isdigit(string[i]) && string[i] != '-' && string[i] != '+' && string[i] != '*' && string[i] != '/' && string[i] != ')' && string[i] != '(') {
            return -1;
        }
        if (string[i] == '+' || string[i] == '-' || string[i] == '*' || string[i] == '/') {
            if ((!isdigit(string[i - 1]) && string[i - 1] != ')') || (!isdigit(string[i + 1]) && string[i + 1] != '(')) {
                return -1;
            }
        }
        if (string[i] == '(') {
            bracketCount++;
        }
        else if (string[i] == ')') {
            bracketCount--;
        }
        if (bracketCount < 0) {
            return -1;
        }
        if (string[i] == '(' && string[i + 1] == ')') {
            return -1;
        }
    }
    if (bracketCount != 0) {
        return -1;
    }
    return 0;
}

int main() {
    int null = 0;
    char* input = malloc(11000);
    if (input == NULL) {
        return -1;
    }
    if (!fgets(input, 11000, stdin)) {
        free(input);
        return -1;
    }
    for (size_t i = 0; i < strlen(input); i++) {
        if (input[i] == '\n') {
            input[i] = '\0';
            null = i;
            break;
        }
    }
    int error = SyntaxError(input, null);
    if (error == -1) {
        printf("syntax error");
    }
    else {
        char* polishEntry = malloc(11000);
        if (polishEntry == NULL) {
            free(input);
            return -1;
        }
        polishEntry = toPolishEntry(input, polishEntry);
        int answer = calculation(polishEntry);
        if (answer != -1) {
            printf("%d", answer);
        }
        free(polishEntry);
    }
    free(input);
    return 0;
}
