#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t empty, full;
int shared_mem;

void* producer(void* p) {
	srand(time(NULL));
	while(1) {
		int rnd = rand() % 256;
		sem_wait(&empty);
		printf("[prod] %d\n", rnd);
		shared_mem = rnd;
		sleep(1);
		sem_post(&full);
	}

}

void* consumer(void* p) {
	while(1) {
		sem_wait(&full);
		int cons = shared_mem;
		printf("[cons] %d\n", cons);
		sleep(1);
		sem_post(&empty);
	}
}

int main(void) {
	pthread_t prod, cons;

	sem_init(&empty, 0, 1);
	sem_init(&full, 0, 0);

	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&cons, NULL, consumer, NULL);

	pthread_join(prod, NULL);
	pthread_join(cons, NULL);

	return 0;
}
