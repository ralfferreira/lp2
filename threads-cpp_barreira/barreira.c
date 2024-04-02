#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define QTD_THREADS 8
#define QTD_IMGS 3

pthread_barrier_t barreira_render;
pthread_barrier_t barreira_print;

int img_parts[QTD_THREADS];

void* render_img_part(void* p) {
	int count = 0;
	long id = (long)p;
	while (count < QTD_IMGS) {
		printf("[%ld] renderizando...\n", id);
		time_t t;
		srand(time(&t)+id);
		int my_render = rand() % 255;
		img_parts[id] = my_render;
		sleep((int)id + 1);
		printf("[%ld] chegou nas barreiras...\n", id);
		pthread_barrier_wait(&barreira_render);
		pthread_barrier_wait(&barreira_print);
		printf("[%ld] passou das barreiras...\n", id);
		count++;
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];
	int count = 0;

	pthread_barrier_init(&barreira_render, NULL, QTD_THREADS + 1);
	pthread_barrier_init(&barreira_print, NULL, QTD_THREADS + 1);
	//darei wait na main tambem

	printf("[main] Renderizando %d imagens, divididas em %d partes...\n",
		QTD_IMGS, QTD_THREADS);

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, render_img_part, (void*)i);
	}

	while (count < QTD_IMGS) {
		pthread_barrier_wait(&barreira_render);
		printf("Exibindo quadro #%d: \n", count);
		for (int i = 0; i < QTD_THREADS; i++) {
			printf("[%d] %d\n", i, img_parts[i]);
		}
		pthread_barrier_wait(&barreira_print);
		count++;
	}

	return 0;


}
