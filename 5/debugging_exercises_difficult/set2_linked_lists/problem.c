// set2: 連結リストから指定した値のノードをすべて削除する
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* 配列 v の n 個の値から連結リストを作る */
Node *build(const int *v, int n) {
    Node *head = NULL;
    Node **tail = &head;
    for (int i = 0; i < n; i++) {
        Node *p = malloc(sizeof(Node));
        p->data = v[i];
        p->next = NULL;
        *tail = p;
        tail = &p->next;
    }
    return head;
}

/* data が key と等しいノードをすべて削除する */
void remove_all(Node **head, int key) {
    Node *prev = NULL;
    Node *cur = *head;
    while (cur != NULL) {
        Node *next = cur->next;
        while (cur->data == key) {
            next=cur->next;
            if (prev == NULL) {
                *head = next;
            } else {
                prev->next = next;
            }
            free(cur);
            cur=next;
        }
        prev = cur;
        cur = next;
    }
}

void print_list(const Node *p) {
    for (; p != NULL; p = p->next) {
        printf("%d -> ", p->data);
    }
    printf("NULL\n");
}

void free_list(Node *p) {
    while (p != NULL) {
        Node *next = p->next;
        free(p);
        p = next;
    }
}

void test(const int *v, int n, int key) {
    Node *head = build(v, n);
    printf("before: ");
    print_list(head);
    remove_all(&head, key);
    printf("remove %d: ", key);
    print_list(head);
    free_list(head);
}

int main(void) {
    int a[] = {1, 2, 3, 2, 4};
    int b[] = {5, 7, 7, 8};
    int c[] = {9, 9, 1};
    test(a, 5, 2);
    test(b, 4, 7);
    test(c, 3, 9);
    return 0;
}
