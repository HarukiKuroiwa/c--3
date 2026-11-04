// 05: 2つのスレッドで共有カウンタを 100万回ずつ増やす
#include <stdio.h>
#include <pthread.h>

#define N 1000000

int counter = 0;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    (void)arg;
    for (int i = 0; i < N; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("counter = %d (expected %d)\n", counter, 2 * N);
    return 0;
}
