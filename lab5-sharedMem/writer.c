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
#include <unistd.h>

#define SHM_SIZE 4096
#define ME 0
#define OTHER 1

void quit(int signum);

int shmId, status;
key_t shmKey;
char *shmAddr;

int main(int argc, char *argv[])
{

	char input[SHM_SIZE];
	int *turn;		// flags for keeping track of the current turn
	char *text;		// the message that will be passed to readers
	signal(SIGINT, quit);

	// Set Up –––––––––––––––––––––––––––
	shmKey = ftok("writer", 'A'); // use executable for the writer as file key
	if (shmKey == -1) {
		perror("ftok()");
		exit(1);
	}

	// create a shm of size SHM_SIZE plus the size of 2 ints (for flags)
	shmId = shmget(shmKey, ( SHM_SIZE+(sizeof(int)*2) ), IPC_CREAT|IPC_EXCL|0600);
	if (shmId < 0) {
		perror("shmget()");
		exit(1);
	}

	shmAddr = shmat(shmId, NULL, 0);
	if (shmAddr == (void *) -1) {
		perror("shmat()");
		exit(1);
	}

	turn = (int *)shmAddr;				//Put turn array at beginning of shm
	text = shmAddr + (sizeof(int)*2);	//Put text after turn array
	turn[ME] = 1;		// writer goes first

	while (1)
	{
		// The writer is blocked when there are any readers in the critical Section
		// (when turn[OTEHR] > 0) or it's not the writer's turn.
		while(turn[OTHER] > 0 || turn[ME] == 0)
			;

		// Critical Section ––––––––––––––––––––
		fprintf(stderr, "Enter a string: ");
		fgets(input, SHM_SIZE, stdin);
		strcpy(text, input);
		// End Critical Section ––––––––––––––––

		//fprintf(stderr, "Turns: Me:%i  Other:%i\n", turn[ME], turn[OTHER]);  // debug
		//sleep(3); //for checking ipcs at runtime
		turn[ME] = 0;
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
