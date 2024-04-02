#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t arrive_1, arrive_2;

void* worker_1(void* p) {
	while (1) {
		time_t t;
		srand(time(&t)+1);
		int rnd = (rand() % 8) + 1;
		printf("[worker 1]: job %d\n", rnd);
		sleep(rnd); //simulando execucao de job
		printf("[worker 1]: barrier in\n");
		//barreira
		sem_post(&arrive_1);
		sem_wait(&arrive_2);
		printf("[worker 1]: out of barrier\n");
	}


}

void* worker_2(void* p) {
	while (1) {
		time_t t;
		srand(time(&t)+2);
		int rnd = (rand() % 8) + 1;
		printf("[worker 2]: job %d\n", rnd);
		sleep(rnd); //simulando execucao de job
		printf("[worker 2]: barrier in\n");
		//barreira
		sem_post(&arrive_2);
		sem_wait(&arrive_1);
		printf("[worker 2]: out of barrier\n");
	}
}

int main(void) {
	pthread_t thread1, thread2;

	sem_init(&arrive_1, 0, 0);
	sem_init(&arrive_2, 0, 0);

	pthread_create(&thread1, NULL, worker_1, NULL);
	pthread_create(&thread2, NULL, worker_2, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}
