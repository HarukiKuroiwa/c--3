// set3: 4つの窓口が同時に座席を販売する
#include <stdio.h>
#include <pthread.h>

#define SEATS   100000
#define THREADS 4

typedef struct {
    int id;
    int sold;                 /* この窓口が売った枚数 */
} Counter;                    /* 販売窓口 */

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

int seats = SEATS;            /* 残り座席数（全窓口で共有） */

/* 空席があれば1枚売る。売れたら1、満席なら0を返す */
int sell() {
    pthread_mutex_lock(&lock);
    if (seats > 0) {
        seats--;
        pthread_mutex_unlock(&lock);
        return 1;
    }
    pthread_mutex_unlock(&lock);
    return 0;
}

/* 満席になるまで売り続ける */
void *window(void *arg) {
    Counter *c = arg;
    while (sell()) {
        c->sold++;
    }
    return NULL;
}

int main(void) {
    Counter c[THREADS];
    pthread_t t[THREADS];

    for (int i = 0; i < THREADS; i++) {
        c[i].id = i;
        c[i].sold = 0;
        pthread_create(&t[i], NULL, window, &c[i]);
    }

    int total = 0;
    for (int i = 0; i < THREADS; i++) {
        pthread_join(t[i], NULL);
        total += c[i].sold;
    }

    printf("sold  = %d (expected %d)\n", total, SEATS);
    printf("seats = %d (expected 0)\n", seats);
    return 0;
}
