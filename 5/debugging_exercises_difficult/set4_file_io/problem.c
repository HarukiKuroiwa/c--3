// set4: ファイルの行数・単語数・バイト数を数える
#include <stdio.h>
#include <ctype.h>

/* path のファイルの行数・単語数・バイト数を数える。開けなければ -1 を返す */
int count_file(const char *path, long *lines, long *words, long *bytes) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return -1;
    }

    *lines = *words = *bytes = 0;
    int c;
    int in_word = 0;
    while ((c=fgetc(fp)) != EOF) {
        (*bytes)++;
        if (c == '\n') {
            (*lines)++;
        }
        if (isspace((unsigned char)c)) {
            in_word = 0;
        } else if (!in_word) {
            (*words)++;
            in_word = 1;
        }
    }
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        long l, w, c;
        if (count_file(argv[i], &l, &w, &c) != 0) {
            printf("%-20s cannot open\n", argv[i]);
            continue;
        }
        printf("%-20s lines=%ld words=%ld bytes=%ld\n", argv[i], l, w, c);
    }
    return 0;
}
