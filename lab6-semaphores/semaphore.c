#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

struct sembuf sem_wait = {0, -1, 0};
struct sembuf sem_signal = {0, 1, 0};

int main (int argc, char *argv[])
{
	long int i, loop, temp, *shmPtr;
	int shmId, semID, status;
	pid_t pid;

	// get value of loop variable (from command-line argument)
	loop = atoi(argv[1]);

	/* Shared Mem Setup -------------------------- */
	if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
		perror ("shmget()");
		exit(1);
	}
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
		perror ("shmat()");
		exit(1);
	}

	/* Semaphore setup ----------------------------- */
	semID = semget (IPC_PRIVATE, 1, 0600);
	if (semID < 0) {
		perror("semget()");
		exit(1);
	}

	status = semctl (semID, 0, SETVAL, 1);
	if (status < 0) {
		perror("semctl()");
		exit(1);
	}

	shmPtr[0] = 0;
	shmPtr[1] = 1;

	if ( !(pid = fork()) )
	{
		//semop wait
		if ( semop(semID, &sem_wait, 1) == -1 ) {
			perror("semop wait");
			exit(1);
		}

		for (i=0; i<loop; i++) {
			// swap the contents of shmPtr[0] and shmPtr[1]
			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;
		}
		//semop signal
		if ( semop(semID, &sem_signal, 1) < 0 ) {
			perror("semop signal");
			exit(1);
		}

		if (shmdt (shmPtr) < 0) {
			perror ("shmdt()");
			exit(1);
		}
			exit(0);
	}
	else
	{
		//semop wait
		if ( semop(semID, &sem_wait, 1) == -1 ) {
			perror("semop wait");
			exit(1);
		}
		for (i=0; i<loop; i++) {
			// swap the contents of shmPtr[1] and shmPtr[0]
			temp = shmPtr[0];
			shmPtr[0] = shmPtr[1];
			shmPtr[1] = temp;
		}
		//semop signal
		if ( semop(semID, &sem_signal, 1) == -1 ) {
			perror("semop signal");
			exit(1);
		}
	}
	wait (&status);
	printf ("shmPtr[0]: %li\tshmPtr[1]: %li\n", shmPtr[0], shmPtr[1]);

	if ( semctl (semID, 0, IPC_RMID) < 0 ) {
		perror("semctl RMID");
		exit(1);
	}
	if (shmdt (shmPtr) < 0) {
		perror ("shmdt()");
		exit (1);
	}
	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		perror ("shmctl()");
		exit(1);
	}

	return 0;
	}
