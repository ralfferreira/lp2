#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#define QTD_THREADS 10
#define INC_LIMIT 10

long var_global;
pthread_mutex_t mutex;

void* incrementa_global(void* p) {
	long id = (long)p;

	printf("[%ld] incrementando var global...\n", id);

	for (int i = 0; i < INC_LIMIT; i++) {
		pthread_mutex_lock(&mutex); //PROTOCOLO DE ENTRADA - SECAO CRITICA
		var_global += ((var_global * 2) + (var_global+1));
		pthread_mutex_unlock(&mutex); //PROTOCOLO DE SAIDA - SECAO CRITICA
	}

}

int main(void) {
	pthread_t threads[QTD_THREADS];

	var_global = 1;

	pthread_mutex_init(&mutex, NULL);

	printf("[main] criando %d threads...\n", QTD_THREADS);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, incrementa_global, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("[main] valor final de var global: %ld\n", var_global);

	return 0;

}

