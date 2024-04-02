#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int global_var;
int lock;

void* incrementa(void* p) {
	long id = (long)p;

	while(1) {
		//protocolo de entrada
		while(__atomic_test_and_set(&lock, 1));
		//<await !(lock) lock = true;>
		//sc
		sleep(1);
		global_var++;
		printf("[thread-%ld] global_var = %d\n",
			id, global_var);
		//sc
		//protocolo de saida
		lock = 0;
	}
}

int main(void) {
	pthread_t th1, th2;

	global_var = 0;
	lock = 0;

	pthread_create(&th1, NULL, incrementa, (void*)1);
	pthread_create(&th2, NULL, incrementa, (void*)2);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	return 0;

}
