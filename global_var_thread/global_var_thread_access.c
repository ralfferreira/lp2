#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD_THREADS 1000
#define QTD_INC 1000

int counter;

void* increment(void* p) {
    for (int i = 0; i < QTD_INC; i++) {
        counter++;
    }
}

int main(void) {
    pthread_t threads[QTD_THREADS];

    counter = 0;

    printf("Criando %d threads para incrementar variavel global\n", QTD_THREADS);

    for (int i = 0; i < QTD_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    for (int i = 0; i < QTD_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Valor final de counter: %d\n", counter);

    return 0;
}