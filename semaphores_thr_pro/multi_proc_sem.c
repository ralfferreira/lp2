#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3
#define SEM_FILE "mys"

sem_t *semaphore;

void child_process(int id) {
	long count = 0;
	sem_wait(semaphore);
	printf("[child-%d] enters critical section\n", id);
	while (count < 9999999999)
		count++;
	printf("[child-%d] leaves critical section\n", id);
	sem_post(semaphore);
	exit(0);
}

int main(void) {
	semaphore = sem_open(SEM_FILE, O_CREAT | O_EXCL, 0644, 1);

	if (semaphore == SEM_FAILED) {
		perror("sem_open error!");
		exit(EXIT_FAILURE);
	}

	pid_t cpid, ppid;

	ppid = getpid();

	for (int i = 0; i < NUM_CHILDREN; ++i) {
		if (ppid == getpid()) {
			cpid = fork();
			if (cpid == 0) {
				child_process(i);
			} else if (cpid < 0) {
				perror("fork error!");
				exit(EXIT_FAILURE);
			}
		}
	}

	for(int i = 0; i < NUM_CHILDREN; ++i) {
		wait(NULL);
	}

	sem_unlink(SEM_FILE);
	sem_close(semaphore);

	return 0;
}
