// 01: 配列の最大値を求める
#include <stdio.h>

int findMax(int arr[], int size) {
    int max;
    for (int i = 0; i <= size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main(void) {
    int a[] = {4, 8, 15, 16, 23, 42};
    int b[] = {-7, -3, -12, -5};

    printf("max(a) = %d\n", findMax(a, 6));
    printf("max(b) = %d\n", findMax(b, 4));
    return 0;
}
