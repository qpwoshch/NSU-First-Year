#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>
#include <windows.h>
#pragma pack(push,1)


typedef struct BMPFile {
    char sign[2];
    unsigned int fsize;
    unsigned int reserv;
    unsigned int imageOffSet;
} BMPFile;

typedef struct BMPInfoHead {
    unsigned int headSize;
    int width;
    int height;
    unsigned short plane;
    unsigned short colorDepth;
    unsigned int compression;
    unsigned int imageSize;
    int xLine;
    int yLine;
    unsigned int countColor;
    unsigned int importantColor;
} BMPInfoHead;


void generate(FILE* file, int WIDTH, int HEIGH) {
    uint8_t data[WIDTH * HEIGH * 3];
    int maxIteration = 255;
    int numbersOfCheck = 100;
    int pixel = 0;
    for (int x = 0; x < HEIGH; x++) {
        for (int y = 0; y < WIDTH; y++) {
            double complex a = ((3.0 * x / WIDTH) - 1.5) + I * ((3.0 * y / HEIGH) - 1.5);
            double complex b = 0;
            int c = 0;
            for (int i = 0; i <= numbersOfCheck; i++) {
                b = b * b + a;
                c++;
                if (cabs(b) > 2) {
                    break;
                }
            }
            if (numbersOfCheck == c) {
                data[pixel] = 0;
                data[pixel + 1] = 0;
                data[pixel + 2] = 0;
            }
            else {
                data[pixel] = (c * 255) / maxIteration;
                data[pixel + 1] = (c * 127) / maxIteration;
                data[pixel + 2] = (c * 63) / maxIteration;
            }
            pixel += 3;
        }
    }
    fwrite(&data, sizeof(data), 1, file);
}

int maxx(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}
int minn(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    int x1, x2, y1, y2, x3, y3, x4, y4;
    printf("Введите 4 точки, по которым будет строить множество Мандельброта:\nx1:");
    scanf("%d", &x1);
    printf("y1:");
    scanf("%d", &y2);
    printf("x2:");
    scanf("%d", &x2);
    printf("y2:");
    scanf("%d", &y2);
    printf("x3:");
    scanf("%d", &x3);
    printf("y3:");
    scanf("%d", &y3);
    printf("x4:");
    scanf("%d", &x4);
    printf("y4:");
    scanf("%d", &y4);
    int WIDTH = maxx(maxx(x1, x2), x3) - minn(minn(x1, x2), x3);
    int HEIGH = maxx(maxx(y1, y2), y3) - minn(minn(y1, y2), y3);

    BMPFile head = { {'B', 'M'}, sizeof(BMPFile) + sizeof(BMPInfoHead), 0, sizeof(BMPFile) + sizeof(BMPInfoHead) };
    BMPInfoHead infoHeader = { sizeof(BMPInfoHead), WIDTH, HEIGH, 1, 24, 0, WIDTH * HEIGH * 3, 2755, 2755, 0, 0 };
    FILE* file = fopen("image.bmp", "wb");
    fwrite(&head, sizeof(head), 1, file);
    fwrite(&infoHeader, sizeof(infoHeader), 1, file);
    generate(file, WIDTH, HEIGH);
    fclose(file);
    return 0;
}
