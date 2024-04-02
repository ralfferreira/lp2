#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t empty, full; //split binary semaphore
int shared_var; //buffer

void* producer(void* p) {
	time_t t;
	srand(time(&t)+1);

	while(1) {
		sem_wait(&empty); //protocolo de entrada
		int data = rand() % 256;
		printf("[producer] generated: %d\n", data);
		shared_var = data;
		sleep(1);
		sem_post(&full); //protocolo de saida
	}
}

void* consumer(void* p) {
	time_t t;
	srand(time(&t)+2);

	while(1) {
		sem_wait(&full);
		int data = shared_var;
		printf("[consumer] received: %d\n", data);
		sleep(1);
		sem_post(&empty);
	}

}

int main(void) {
	pthread_t prod, cons;

	sem_init(&empty, 0, 1);
	sem_init(&full, 0, 0);

	pthread_create(&prod, 0, producer, 0);
	pthread_create(&cons, 0, consumer, 0);

	pthread_join(prod, 0);
	pthread_join(cons, 0);

	return 0;
}
