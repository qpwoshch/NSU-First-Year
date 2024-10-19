#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned char byte;

typedef struct Node {
    int count;
    int symbol;
    struct Node* left;
    struct Node* right;
} node;

struct BitArray {
    byte* data;
    size_t size;
    size_t capacity;
};

struct BitArray* BitArray_Init(struct BitArray* ba, size_t capacity) {
    ba->data = (byte*)calloc(capacity, sizeof(byte));
    if (ba->data == NULL) {
        return 0;
    }
    ba->capacity = 0;
    ba->size = 0;
    return ba;
}


struct BitArray* BitArray_Reset(struct BitArray* ba) {
    ba->size = 0;
    ba->capacity = 0;
    ba->data = (byte*)calloc(1, sizeof(byte));
    if (ba->data == NULL) {
        free(ba->data);
        return NULL;
    }
    return ba;
}

void BitArray_Destroy(struct BitArray* ba) {
    free(ba->data);
}



node* create(uint frequency, int index, node* right, node* left) {
    node* nodes = (node*)malloc(sizeof(node));
    if (nodes == NULL) {
        return NULL;
    }
    nodes->left = left;
    nodes->right = right;
    nodes->count = frequency;
    nodes->symbol = index;
    return nodes;
}

node* tree(uint* frequency) {
    node* nodes[256] = { NULL };
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (frequency[i] != 0) {
            nodes[count] = create(frequency[i], i, NULL, NULL);
            if (nodes[count] == NULL) {
                return NULL;
            }
            count++;
        }
    }
    while (count > 1) {
        for (int i = 0; i < count - 1; ++i) {
            for (int j = 0; j < count - i - 1; ++j) {
                if (nodes[j]->count < nodes[j + 1]->count) {
                    node* temp = nodes[j + 1];
                    nodes[j + 1] = nodes[j];
                    nodes[j] = temp;
                }
            }
        }
        node* new_node = create(nodes[count - 2]->count + nodes[count - 1]->count, 0, nodes[count - 1], nodes[count - 2]);
        if (new_node == NULL) {
            return NULL;
        }
        nodes[count - 2] = new_node;
        count--;
    }
    return nodes[0];
}

void free_tree(node* root) {
    if (root->left != NULL) {
        free_tree(root->left);
    }
    if (root->right != NULL) {
        free_tree(root->right);
    }
    if (root != NULL) {
        free(root);
    }
}

uchar** make_table(uchar* path, uchar lvl, uchar** tabel, node* root) {
    if (root->right || root->left) {
        path[lvl] = '0';
        if (make_table(path, lvl + 1, tabel, root->left) == NULL) {
            return NULL;
        }
        path[lvl] = '1';
        if (make_table(path, lvl + 1, tabel, root->right) == NULL) {
            return NULL;
        }
    }
    else {
        tabel[root->symbol] = (uchar*)malloc((lvl + 1) * sizeof(uchar));
        if (tabel[root->symbol] == NULL) {
            return NULL;
        }
        path[lvl] = '\0';
        memcpy(tabel[root->symbol], path, lvl + 1);
    }
    return tabel;
}

void free_table(uchar** table) {
    for (int i = 0; i < 256; i++) {
        if (table[i] != NULL) {
            free(table[i]);
        }
    }
}

void Addbit(struct BitArray* ba, uchar data) {
    if ((ba->size) % 8 == 0 && ba->size != 0) {
        //       fwrite(&ba->data[ba->capacity], sizeof(byte), 1, stdout);
        ba->capacity++;
        byte* help = (byte*)realloc(ba->data, (ba->capacity + 1) * sizeof(byte));
        if (help == NULL)
            return;
        ba->data = help;
        ba->data[ba->capacity] = 0;
    }
    ba->data[ba->capacity] |= (data << (7 - ((ba->size) % 8)));
    ba->size++;
}

void Addbyte(struct BitArray* ba, uchar byte) {
    for (int i = 0; i < 8; i++) {
        Addbit(ba, (byte >> (7 - i % 8)));
    }
}

/*
void TreeToBit(node* root, struct BitArray* ba) {
    if (root) {
        if ((!root->left) && (!root->right)) {
            Addbit(ba, 1);
            Addbyte(ba, root->symbol);
        }
        else {
            Addbit(ba, 0);
            TreeToBit(root->left, ba);
            TreeToBit(root->right, ba);
        }
    }

}
*/

void writeInFile(struct BitArray* ba, FILE* output) {
    if (!fwrite(ba->data, sizeof(byte), (ba->size + 7) / 8, output)) {
        return;
    }
}

void TextToBit(uchar** table, FILE* input, struct BitArray* ba) {
    while (1) {
        uchar element;
        if (fread(&element, sizeof(uchar), 1, input) == 0) {
            break;
        }
        size_t size = strlen((char*)table[element]);
        uchar* binaryView = table[element];
        for (size_t i = 0; i < size; i++) {
            uchar help;
            if (binaryView[i] == 48) {
                help = 0;
            }
            else {
                help = 1;
            }
            //          printf("%u", help);
            Addbit(ba, help);
        }

    }
}

/*
void printRoot(node* root) {
    if (root == NULL) {
        return;
    }
    if (root->left == NULL && root->right == NULL) {
        printf("Symbol: %c, Frequency: %d\n", root->symbol, root->count);
    }
    else {
        printf("Node: Frequency: %d\n", root->count);
    }
    printRoot(root->left);
    printRoot(root->right);
}


void printTable(uchar** table) {
    for (int i = 0; i < 256; i++) {
        if (table[i] != NULL) {
            printf("Symbol: %c, Code: %s\n", i, table[i]);
        }
    }
}

void printBitArray(struct BitArray* ba) {
    printf("\n");
    for (size_t i = 0; i < ba->size; i++) {
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
        printf("%d", (ba->data[i / 8] >> (7 - (i % 8))) & 1);
    }
    printf("\n");
}


void print_bits_of_file(FILE* file) {


    int byte;
    while ((byte = fgetc(file)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            putchar((byte & (1 << i)) ? '1' : '0');
        }
        putchar(' ');
    }


}
*/
uint countSymbol(uchar** table) {
    uint count = 0;
    for (int i = 0; i < 256; i++) {
        if (table[i] != NULL) {
            count++;
        }
    }
    return count;
}
/*
void charToBinary(unsigned char symbol, char* binary) {
    for (int i = 7; i >= 0; i--) {
        binary[7 - i] = (symbol & (1 << i)) ? '1' : '0';
    }
    binary[8] = '\0';
}
*/

void table_to_bit(uchar** table, struct BitArray* ba, uint length) {
    for (int i = 0; i < 256; i++) {
        if (table[i] != NULL) {
            uchar size = (uchar)strlen((const char*)table[i]);
            uchar sizee = size + '0';
            Addbyte(ba, sizee);
            Addbyte(ba, i);
            if (sizee == '0') {
                uchar h = (uchar)length + '0';
                Addbyte(ba, h);
            }
            uchar* binaryView = table[i];
            for (size_t i = 0; i < size; i++) {
                uchar help;
                if (binaryView[i] == 48) {
                    help = 0;
                }
                else {
                    help = 1;
                }
                Addbit(ba, help);
            }
        }
    }
}

int CountSymbolInFile(uchar** table, FILE* input) {
    int count = 0;
    fseek(input, 1, SEEK_SET);
    while (1) {
        uchar element;
        if (fread(&element, sizeof(uchar), 1, input) == 0) {
            break;
        }
        size_t size = strlen((char*)table[element]);
        for (size_t i = 0; i < size; i++) {
            count++;
        }

    }
    return count;
}

void arch(FILE* input, FILE* output, int seek) {
    uint* frequency = (uint*)calloc(256, sizeof(uint));
    if (frequency == NULL) {
        return;
    }
    if (seek == 1) {
        fseek(input, 1, SEEK_SET);
    }
    uint length = 0;
    while (1) {
        uchar symbol;
        if (fread(&symbol, sizeof(uchar), 1, input) == 0) {
            break;
        }
        length++;
        frequency[symbol]++;
    }
    uchar sum = 0;
    for (int i = 0; i < 256; i++) {
        if (frequency[i] != 0) {
            sum++;
        }
    }
    node* root = tree(frequency);
    if (root == NULL) {
        free(frequency);
        return;
    }
    //   printRoot(root);
    uchar** table = (uchar**)malloc(256 * sizeof(uchar*)); 
    if (table == NULL) {
        free(frequency);
        return;
    }
    for (int i = 0; i < 256; i++) {
        table[i] = NULL;
    }
    uchar* path = (uchar*)calloc(32, sizeof(uchar)); 
    if (path == NULL) {
        free(frequency);
        free(table);
        return;
    }
    if (make_table(path, 0, table, root) == NULL) {
        free(frequency);
        free(table);
        free(path);
        return;
    }
    uint count = 0;
    count = countSymbol(table);
    int countSymbol = CountSymbolInFile(table, input);
    uint uintCountSymbolInShifr;
//    if (countSymbol < 250) {
    uintCountSymbolInShifr = (uint)countSymbol;
//    }
//    printf("%d\n", count);
    if (!fwrite(&count, sizeof(uint), 1, output)) {
        free(frequency);
        free(table);
        free(path);
        return;
    }
    if (!fwrite(&uintCountSymbolInShifr, sizeof(uint), 1, output)) {
        free(frequency);
        free(table);
        free(path);
        return;
    }
    //    printf("%d\n", count);
//    printTable(table);
    //    fwrite(&length, sizeof(uint), 1, output);
    //   TreeToBit(root, &ba);
    //   printBitArray(&ba);
    //   writeInFile(&ba, output);
    //   printf("%d\n\n", length);
    if (seek == 1) {
        fseek(input, 1, SEEK_SET);
    }
    else {
        fseek(input, 0, SEEK_SET);
    }
    struct BitArray ba;
    BitArray_Init(&ba, 16);
    table_to_bit(table, &ba, length);
    //   printBitArray(&ba);
    writeInFile(&ba, output);
    BitArray_Reset(&ba);
    TextToBit(table, input, &ba);
    writeInFile(&ba, output);
    //   printBitArray(&ba);
    free(frequency);
    free_table(table);
    free_tree(root);
    BitArray_Destroy(&ba);
    free(table);
    free(path);
}

int read_bit(FILE* file, uint* bit_position, uchar* current_byte) {
    if (*bit_position == 8) {
        int byte_read = fgetc(file);
        if (byte_read == EOF) {
            return -1;
        }
        *current_byte = (uchar)byte_read;
        *bit_position = 0;
    }

    int bit = (*current_byte >> (7 - *bit_position)) & 1;
    //    printf("%d", bit);
    (*bit_position)++;
    return bit;
}

int read_bits(FILE* file, uint* bit_position, uchar* current_byte) {
    if (*bit_position == 8) {
        int byte_read = fgetc(file);
        if (byte_read == EOF) {
            return -1;
        }
        *current_byte = (uchar)byte_read;
        *bit_position = 0;
    }

    int bit = (*current_byte >> (7 - *bit_position)) & 1;
    (*bit_position)++;
    return bit;
}



unsigned char read_byte(FILE* file, uint* bit_position, uchar* current_byte) {
    unsigned char byte = 0;
    for (int i = 0; i < 8; i++) {
        int bit = read_bit(file, bit_position, current_byte);
        if (bit == -1) {
            return (unsigned char)-1;
        }
        byte = (byte << 1) | bit;
    }
    return byte;
}

void skip_bits(FILE* file, uint* bit_position, uchar* current_byte, uint bits_to_skip) {
    for (uint i = 0; i < bits_to_skip; i++) {
        if (read_bits(file, bit_position, current_byte) == -1) {
            return;
        }
    }
}


void uncoder(FILE* input, FILE* output, uchar** table, uint count, uchar* symbol, uint* position) {
    skip_bits(input, position, symbol, 8);
    for (uint i = 0; i < count; i++) {
        uchar help = 0;
        help = read_byte(input, position, symbol);
        uchar letter = 0;
        letter = read_byte(input, position, symbol);
        //       printf("%u %u %c\n",i+1, help-'0', letter);
        int length = 0;
        length = (int)help - '0';
        //       printf("%d", length);
        if (length == 0) {
            uchar coul = 0;
            coul = read_byte(input, position, symbol);
            //            printf("%u\n", coul - '0');
            for (int i = 0; i < (int)coul - '0'; i++) {
                if (!fwrite(&letter, sizeof(uchar), 1, output)) {
                    return;
                }
            }
            break;
        }
        //       printf("%d\n", help);
        uchar* code = (uchar*)malloc((length + 1) * sizeof(uchar));
        if (code == NULL) {
            return;
        }
        for (int j = 0; j < length; j++) {
            uchar bit = read_bit(input, position, symbol);
            if (bit == 1) {
                code[j] = '1';
            }
            else if (bit == 0) {
                code[j] = '0';
            }
        }
        code[length] = '\0';
        //       printf("%s\n", code);
        table[letter] = code;
    }
}

void printCode(FILE* input, FILE* output, uchar** table, uchar* symbol, uint* position, uint lenBinCode) {
    uchar code[256] = { 0 };
    int length = 0;
    if (lenBinCode > 40) {
        while (1) {
            uchar bit = read_bit(input, position, symbol);
            if (bit != 1 && bit != 0) {
                return;
            }
            code[length] = bit + '0';
            //        printf("%c", code[length]); 
            length++;
            for (int i = 0; i < 256; i++) {
                if (table[i] != NULL) {
                    code[length] = '\0';
                    //              printf("%s %s\n", table[i], code);
                    if (strcmp((const char*)code, (const char*)table[i]) == 0) {
                        if (!fwrite(&i, sizeof(uchar), 1, output)) {
                            return;
                        }
                        memset(code, 0, 256);
                        length = 0;
                    }
                }
            }
        }
    }
    else {
        for (uint i = 0; i < lenBinCode; i++) {
            uchar bit = read_bit(input, position, symbol);
            if (bit != 1 && bit != 0) {
                return;
            }
            code[length] = bit + '0';
            //        printf("%c", code[length]); 
            length++;
            for (int i = 0; i < 256; i++) {
                if (table[i] != NULL) {
                    code[length] = '\0';
                    //              printf("%s %s\n", table[i], code);
                    if (strcmp((const char*)code, (const char*)table[i]) == 0) {
                        if (!fwrite(&i, sizeof(uchar), 1, output)) {
                            return;
                        }
                        memset(code, 0, 105);
                        length = 0;
                    }
                }
            }
        }
    }
}


void unarch(FILE* input, FILE* output, int seek) {
    if (seek == 1) {
        fseek(input, 1, SEEK_SET);
    }
    uint count = 0;
    uint count1 = 0;
    if (!fread(&count, sizeof(uint), 1, input)) {
        printf("%s", "");
        return;
    }
    count1 = (uint)count;
    uint UcharLenBinCode = 0;
    if (seek == 1) {
        fseek(input, 5, SEEK_SET);
    }
    else {
        fseek(input, 4, SEEK_SET);
    }
    if (!fread(&UcharLenBinCode, sizeof(uint), 1, input)) {
        return;
    }
    uint lenBinCode = (uint)UcharLenBinCode;
 //   printf("%d %d",count,  UcharLenBinCode);
    if (seek == 1) {
        fseek(input, 9, SEEK_SET);
    }
    else {
        fseek(input, 8, SEEK_SET);
    }
    uchar** table = (uchar**)malloc(256 * sizeof(uchar*));
    if (table == NULL) {
        return;
    }
    //   printf("%u", count1);
    for (int i = 0; i < 256; i++) {
        table[i] = NULL;
    }
    uint position = 0;
    uchar symbol = 0;
    uncoder(input, output, table, count1, &symbol, &position);
 //   printTable(table);
    position = 8;
    printCode(input, output, table, &symbol, &position, lenBinCode);
    free_table(table);
    free(table);
}



int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "-c") == 0) {
        const char* filenameIn = argv[2];
        const char* filenameOut = argv[3];
        FILE* input;
        FILE* output;
        input = fopen(filenameIn, "rb");
        if (input == NULL) {
            return 0;
        }
        output = fopen(filenameOut, "wb");
        arch(input, output, 0);
        fclose(input);
        fclose(output);
        return 0;
    }
    else if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        const char* filenameIn = argv[2];
        const char* filenameOut = argv[3];
        FILE* input;
        FILE* output;
        input = fopen(filenameIn, "rb");
        if (input == NULL) {
            return 0;
        }
        output = fopen(filenameOut, "wb");
        unarch(input, output, 0);
        fclose(input);
        fclose(output);
        return 0;
    }
    else {
        char firstSymbol;
        FILE* input;
        FILE* output;
        //   input = fopen("..\\x64\\Debug\\out.txt", "rb");
        input = fopen("in.txt", "rb");
        if (input == NULL) {
            return 0;
        }
        output = fopen("out.txt", "wb");
        //   output = fopen("in.txt", "wb");
        if (!fread(&firstSymbol, sizeof(char), 1, input)) {
            fclose(input);
            fclose(output);
            return 0;
        }
        if (firstSymbol == 'c') {
            arch(input, output, 1);
        }
        else {//if (firstSymbol == 'd') {
            unarch(input, output, 1);
        }
        fclose(input);
        fclose(output);
        return 0;
    }
}
