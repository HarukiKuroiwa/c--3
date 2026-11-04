// 02: 文字列の末尾にタグを付ける
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* addTag(char *str) {
    int size=0;
    char* tmp=str;
    while(*tmp!='\0'){
        tmp++;
        size++;
    }
    char* nstr=malloc(size+sizeof(" [checked]")+1);
    strcpy(nstr,str);
    strcat(nstr, " [checked]");
    return nstr;
}

int main(void) {
    char text[12] = "hello world";

    printf("before: %s (%zu)\n", text, strlen(text));
    char *ntext=addTag(text);
    printf("after : %s (%zu)\n", ntext, strlen(ntext));
    return 0;
}
