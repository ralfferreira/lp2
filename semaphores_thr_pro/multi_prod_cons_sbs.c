#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8
#define QTD_PROD 1
#define QTD_CONS 5

int buffer[BUFFER_SIZE];

sem_t empty, full;
sem_t mutex_d, mutex_f; //mutex_d - producers; mutex_f - consumers;
int front, rear;


void* producer(void* p) {
	long id = (long)p;
	srand(time(NULL)+(int)id);
	while(1) {
		int _prod = rand() % 256;
		sem_wait(&empty);
		sem_wait(&mutex_d);
		buffer[rear] = _prod;
		printf("[prod-%ld] %d\n", id, _prod);
		rear = (rear+1) % BUFFER_SIZE;
		sem_post(&mutex_d);
		sem_post(&full);
		usleep(200000); //100ms
	}
}

void* consumer(void* p) {
	long id = (long)p;
	while(1) {
		sem_wait(&full);
		sem_wait(&mutex_f);
		int _cons = buffer[front];
		printf("[cons-%ld] %d\n", id, _cons);
		front = (front+1) % BUFFER_SIZE;
		sem_post(&mutex_f);
		sem_post(&empty);
		usleep(100000); //200ms
	}
}

int main(void) {
	pthread_t prod[QTD_PROD], cons[QTD_CONS];

	front = 0;
	rear = 0;

	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);

	sem_init(&mutex_d, 0, 1);
	sem_init(&mutex_f, 0, 1);

	for (long i = 0; i < QTD_PROD; i++) {
		pthread_create(&prod[i], 0, producer, (void*)i);
	}

	for (long i = 0; i < QTD_CONS; i++) {
		pthread_create(&cons[i], 0, consumer, (void*)i);
	}

	for (int i = 0; i < QTD_PROD; i++) {
		pthread_join(prod[i], NULL);
	}

	for (int i = 0; i < QTD_CONS; i++) {
		pthread_join(cons[i], NULL);
	}

	return 0;
}
