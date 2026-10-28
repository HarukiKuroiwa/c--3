// set5: 二分探索木からノードを削除する
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

Node *insert(Node *r, int x) {
    if (r == NULL) {
        r = malloc(sizeof(Node));
        r->data = x;
        r->left = r->right = NULL;
        return r;
    }
    if (x < r->data) {
        r->left = insert(r->left, x);
    } else if (x > r->data) {
        r->right = insert(r->right, x);
    }
    return r;
}

/* 部分木の中で最小の値を持つノード */
Node *min_node(Node *r) {
    while (r->left != NULL) {
        r = r->left;
    }
    return r;
}

/* key を持つノードを削除し、削除後の部分木の根を返す */
Node *delete(Node *r, int key) {
    if (r == NULL) {
        return NULL;
    }
    if (key < r->data) {
        r->left = delete(r->left, key);
    } else if (key > r->data) {
        r->right = delete(r->right, key);
    } else {
        if (r->left == NULL) {            /* 子が右だけ、または子なし */
            Node *child = r->right;
            free(r);
            return child;
        }
        if (r->right == NULL) {           /* 子が左だけ */
            Node *child = r->left;
            free(r);
            return child;
        }
        /* 子が2つ: 右部分木の最小値（後継）で置き換え、後継を右部分木から削除する */
        Node *s = min_node(r->right);
        r->data = s->data;
        r->right = delete(r->right, key);
    }
    return r;
}

void inorder(const Node *r) {
    if (r != NULL) {
        inorder(r->left);
        printf("%d ", r->data);
        inorder(r->right);
    }
}

/* すべてのノードが lo < data < hi を満たし、BST の条件を守っているか */
int is_bst(const Node *r, long lo, long hi) {
    if (r == NULL) {
        return 1;
    }
    if (r->data <= lo || r->data >= hi) {
        return 0;
    }
    return is_bst(r->left, lo, r->data) && is_bst(r->right, r->data, hi);
}

void free_tree(Node *r) {
    if (r != NULL) {
        free_tree(r->left);
        free_tree(r->right);
        free(r);
    }
}

void show(const Node *r, const char *msg) {
    printf("%-12s: ", msg);
    inorder(r);
    printf("| BST: %s\n", is_bst(r, -1000000L, 1000000L) ? "OK" : "BROKEN");
}

int main(void) {
    int v[] = {50, 30, 70, 20, 40, 60, 80, 65};
    Node *root = NULL;
    for (int i = 0; i < 8; i++) {
        root = insert(root, v[i]);
    }
    show(root, "start");
    root = delete(root, 20);  show(root, "delete 20");
    root = delete(root, 60);  show(root, "delete 60");
    root = delete(root, 50);  show(root, "delete 50");
    free_tree(root);
    return 0;
}
