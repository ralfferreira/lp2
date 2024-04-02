#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define QTD_THREADS 5

typedef struct {
	long id;
	int count;
} param_t;

void* funcao_thread(void* p) {
	param_t* my_params = (param_t*) p;

	printf("[%ld] iniciando contagem (%d)...\n", my_params->id, my_params->count);
	for (int i = 0; i < my_params->count; i++) {
		printf("[%ld] %d\n", my_params->id, i);
		sleep(1);
	}
	printf("[%ld] finalizando contagem...\n", my_params->id);

//	return (void*)my_params->id;
	pthread_exit((void*)my_params->id);

}

int main(void) {
	pthread_t threads[QTD_THREADS];
	param_t params[QTD_THREADS];

	printf("[main] criando %d threads...\n", QTD_THREADS);

	for (long i = 0; i < QTD_THREADS; i++) {
		params[i].id = i;
		params[i].count = rand() % 10;
		pthread_create(&threads[i], NULL, funcao_thread, (void*)&params[i]);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		long retval;
		pthread_join(threads[i], (void*)&retval);
		printf("[main] finalizando thread %d recebi dela %ld\n", i, retval);
	}

	printf("[main] finalizando funcao main...\n");

	return 0;
}
