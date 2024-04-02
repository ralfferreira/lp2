#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8

int buffer[BUFFER_SIZE];

sem_t empty, full;
int front, rear;

void* producer(void* p) {
	srand(time(NULL));
	while(1) {
		int _prod = rand() % 256;
		sem_wait(&empty);
		buffer[rear] = _prod;
		printf("[prod] %d\n", _prod);
		rear = (rear+1) % BUFFER_SIZE;
		sem_post(&full);
		usleep(300000);
	}
}

void* consumer(void* p) {
	while(1) {
		sem_wait(&full);
		int _cons = buffer[front];
		printf("[cons] %d\n", _cons);
		front = (front+1) % BUFFER_SIZE;
		sem_post(&empty);
		usleep(100000);
	}
}

int main(void) {
	pthread_t prod, cons;

	front = 0;
	rear = 0;

	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);

	pthread_create(&prod, 0, producer, 0);
	pthread_create(&cons, 0, consumer, 0);

	pthread_join(prod, NULL);
	pthread_join(cons, NULL);

	return 0;
}
