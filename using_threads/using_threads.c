#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD_THREADS 3
#define COUNTER_LIMIT 100

void* contador(void* p) {    
    long index = (long)p;

    int seed = time(NULL) + index;

    srand(seed);

    long limit = rand() % COUNTER_LIMIT + 10;

    printf("[%ld] LIMIT = %ld\n", index, limit);

    for (int i = 0; i < limit; i++) {
        printf("[%ld] Conta: %d\n", index, i);
        usleep(300000);
    }

    pthread_exit((void*)limit);
}

int main(void) {
    pthread_t threads[QTD_THREADS];
    void* thread_results[QTD_THREADS];

    for (long i = 0; i < QTD_THREADS; i++) {
        pthread_create(&threads[i], NULL, contador, (void*)i);
    }

    for (int i = 0; i < QTD_THREADS; i++) {
        pthread_join(threads[i], &thread_results[i]);
    }

    for (int i = 0; i < QTD_THREADS; i++) {
        printf("[main] recebi: %ld da thread [#%d]\n", (long)thread_results[i], i);
    }

    return 0;

}