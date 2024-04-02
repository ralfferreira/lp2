#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 100
#define INC_LIMIT 100000

long var_global;
char prosseguir;


void* incrementa_global(void* p) {
	long id = (long)p;

	printf("[%ld] incrementando var global...\n", id);

	for (int i = 0; i < INC_LIMIT; i++) {
//PROTOCOLO DE ENTRADA - SECAO CRITICA
		while(!(__sync_lock_test_and_set(&prosseguir, 0)));

//SECAO CRITICA - INICIO
		var_global++; //incremento nao-atomico - problemas!
//SECAO CRITICA - FIM

//PROTOCOLO DE SAIDA - SECAO CRITICA
		prosseguir = 1;

	}


}

int main(void) {
	pthread_t threads[QTD_THREADS];

	var_global = 0;
	prosseguir = 1;

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

