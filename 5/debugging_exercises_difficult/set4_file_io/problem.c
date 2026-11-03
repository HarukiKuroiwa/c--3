// set4: ファイルの行数・単語数・バイト数を数える
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 1024

/* path のファイルの行数・単語数・バイト数を数える。開けなければ -1 を返す */
int count_file(const char *path, long *lines, long *words, long *bytes) {
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return -1;
    }

    char buf[BUF_SIZE];
    *lines = *words = *bytes = 0;
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        (*lines)++;
        *bytes += strlen(buf);
        for (char *tok = strtok(buf, " \t\n"); tok != NULL; tok = strtok(NULL, " \t\n")) {
            (*words)++;
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
