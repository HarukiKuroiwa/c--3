// 03: 名簿から名前を検索して年齢を表示する
#include <stdio.h>
#include <string.h>

struct Person {
    const char *name;
    int age;
};

// name に一致する人を探す。見つからなければ NULL を返す
struct Person *findPerson(struct Person list[], int n, const char *name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(list[i].name, name) == 0) {
            return &list[i];
        }
    }
    return NULL;
}

int main(void) {
    struct Person members[] = {
        {"Sato", 21}, {"Suzuki", 22}, {"Tanaka", 20}
    };
    const char *query[] = {"Suzuki", "Kato", "Tanaka"};

    for (int i = 0; i < 3; i++) {
        struct Person *p = findPerson(members, 3, query[i]);
        if(p==NULL){
            printf("%s: %s\n",query[i],"not found");
        }else{
        printf("%s: %d\n", p->name, p->age);
        }
    }
    return 0;
}
