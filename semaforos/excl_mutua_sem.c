#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define QTD_THREADS 5
#define VAR_LIMIT 60

int var_global = 0;
sem_t mutex;

void* incrementa(void* p) {
	long id = (long)p;
	while (1) {
		sem_wait(&mutex); //protocolo de entrada
		var_global++;
		printf("[%ld] alterou var_global: %d\n", id, var_global);
//		sleep(1);
		if (var_global == VAR_LIMIT)
			exit(0);
		sem_post(&mutex); //protocolo de saida
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	sem_init(&mutex, 0, 1);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], 0, incrementa, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}


	return 0;
}
