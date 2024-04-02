#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SEM_FILE "shm_____sem"
#define SHM_SIZE 1024

int main(void) {
	int shmid_sm, shmid_tu;
	key_t key_sm, key_tu;
	int *shared_memory;
	int *turn;
	sem_t *mutex;

	key_sm = ftok("/tmp/key_sm", 'a');
	if (key_sm == -1) {
		perror("ftok()");
		exit(1);
	}

        key_tu = ftok("/tmp/key_tu", 'a');
        if (key_tu == -1) {
                perror("ftok()");
                exit(1);
        }


	//aloca espaco de memoria compartilhada entre processos

	shmid_sm = shmget(key_sm, SHM_SIZE, IPC_CREAT | 0666);
	shmid_tu = shmget(key_tu, SHM_SIZE, IPC_CREAT | 0666);

	if (shmid_sm == -1) {
		perror("shmget()");
		exit(1);
	}

        if (shmid_tu == -1) {
                perror("shmget()");
                exit(1);
        }


	shared_memory = (int*) shmat(shmid_sm, NULL, 0);
	turn = (int*) shmat(shmid_tu, NULL, 0);

	if (shared_memory == (int*)-1) {
		perror("shmat()");
		exit(1);
	}

	if (turn == (int*)-1) {
		perror("shmat()");
		exit(1);
	}

	(*shared_memory) = 0;

	printf("[main] SHARED MEMORY = 0\n");

	mutex = sem_open(SEM_FILE, O_CREAT | O_EXCL, 0644, 1);
	*turn = 0;

	pid_t c_pid = fork();

	if (c_pid == -1) {
		perror("fork()");
		exit(1);
	}

	if (c_pid == 0) {
		while (1) {
			printf("[proc1] START\n");
			//PRCO1
			while(*turn != 1);
			sem_wait(mutex);
			//SECAO CRITICA START
			printf("[proc1] INC\n");
			(*shared_memory)++;
			sleep(1);
			printf("[proc1] SHARED MEMORY = %d\n", (*shared_memory));
			sem_post(mutex);
			printf("[proc1] END\n");
			*turn = 0;
			//SECAO CRITICA END

		}
	} else {
		while (1) {
		//PRC02
		while(*turn != 0);
		printf("[proc2] START\n");
		sem_wait(mutex);
		//SECAO CRITICA START
		printf("[proc2] INC\n");
		(*shared_memory)++;
		sleep(1);
		printf("[proc2] SHARED MEMORY = %d\n", (*shared_memory));
		sem_post(mutex);
		printf("[proc2] END\n");
		*turn = 1;
		//SECAO CRITICA END
		}

	}

	sem_unlink(SEM_FILE);
	sem_close(mutex);

	return 0;

}
