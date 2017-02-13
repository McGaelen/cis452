/*
	CIS 452 Lab 5 - writer.c
	Gaelen McIntee and Emily Wang
	?/?/????
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h> //debug

#define SHM_SIZE 4096

void quit(int signum);

int shmId, status;
key_t shmKey;
char *shmAddr;

int main(int argc, char *argv[])
{

	char input[SHM_SIZE];
	int turn;
	signal(SIGINT, quit);

	// Set Up –––––––––––––––––––––––––––
	shmKey = ftok("writer", 'A');
	if (shmKey == -1) {
		perror("ftok()");
		exit(1);
	}

	shmId = shmget(shmKey, SHM_SIZE, IPC_CREAT|IPC_EXCL|0600);
	if (shmId < 0) {
		perror("shmget()");
		exit(1);
	}

	shmAddr = shmat(shmId, NULL, 0);
	if (shmAddr == (void *) -1) {
		perror("shmat()");
		exit(1);
	}


	while (1)
	{
		// Lockstep Synchronization goes here... idk how to do it tho

		turn = 0;
		while(turn != 0)
			;
		// Critical Section ––––––––––––––––––––
		fprintf(stderr, "Enter a string: ");
		fgets(input, SHM_SIZE, stdin);

		strcpy(shmAddr, input);
		// End Critical Section –––––––––––––––––

		turn = 1;
		//fprintf(stderr, "Shared mem: %s", shmAddr);  // debug
		//sleep(10); //for checking ipcs at runtime
	}

}

void quit(int signum)
{
	fprintf(stderr, " received.  Exiting...\n");

	status = shmdt(shmAddr);
	if (status < 0) {
		perror("shmdt()");
		exit(1);
	}

	status = shmctl(shmId, IPC_RMID, 0);
	if (status < 0) {
		perror("shmctl()");
		exit(1);
   }

   exit(0);
}
