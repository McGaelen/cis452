/*
	CIS 452 Lab 5 - reader.c
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
	int turn;
	signal (SIGINT, quit);

	// Set Up –––––––––––––––––––––––––––
	shmKey = ftok("writer", 'A'); //use executable for the writer as file
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

		// Critical Section ––––––––––––––––––––
		fprintf(stderr, "Shared mem: %s", shmAddr);
		// End Critical Section –––––––––––––––––s
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

	//reader does not mark shared mem for deletion... i think

	exit(0);
}
