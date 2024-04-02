#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 10
#define INC_LIMIT 1000000

long var_global;

void* incrementa_global(void* p) {
	long id = (long)p;

	printf("[%ld] incrementando var global...\n", id);

	for (int i = 0; i < INC_LIMIT; i++) {
		//var_global++; //incremento nao-atomico - problemas!
		//__sync_fetch_and_add(&var_global, 1); //incremento atomico
		__atomic_fetch_add(&var_global, 1, __ATOMIC_SEQ_CST); //incremento atomico
	}

}

int main(void) {
	pthread_t threads[QTD_THREADS];

	var_global = 0;

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

