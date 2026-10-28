// set1: 行列を時計回りに90度回転する
#include <stdio.h>
#include <stdlib.h>

/* rows 行 cols 列の行列 src を時計回りに90度回転し、
   cols 行 rows 列の行列 dst に入れる。
   行列は1次元配列に「行ごとに」並べて格納している（i 行 j 列 → [i * 列数 + j]） */
void rotate(const int *src, int *dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j * cols + (rows - 1 - i)] = src[i * cols + j];
        }
    }
}

void print_matrix(const char *title, const int *m, int rows, int cols) {
    printf("%s (%dx%d)\n", title, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%3d", m[i * cols + j]);
        }
        printf("\n");
    }
}

void test(int rows, int cols) {
    int *src = malloc(sizeof(int) * rows * cols);
    int *dst = malloc(sizeof(int) * rows * cols);
    for (int k = 0; k < rows * cols; k++) {
        src[k] = k + 1;
        dst[k] = 0;
    }
    print_matrix("original", src, rows, cols);
    rotate(src, dst, rows, cols);
    print_matrix("rotated", dst, cols, rows);
    printf("\n");
    free(src);
    free(dst);
}

int main(void) {
    test(3, 3);
    test(2, 4);
    test(4, 2);
    return 0;
}
