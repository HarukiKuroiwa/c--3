// 02: 文字列の末尾にタグを付ける
#include <stdio.h>
#include <string.h>

void addTag(char *str) {
    strcat(str, " [checked]");
}

int main(void) {
    char text[256] = "hello world";

    printf("before: %s (%zu)\n", text, strlen(text));
    addTag(text);
    printf("after : %s (%zu)\n", text, strlen(text));
    return 0;
}
