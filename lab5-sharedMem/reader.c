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
#include <unistd.h>

#define SHM_SIZE 4096
#define ME 1
#define OTHER 0

void quit(int signum);

int shmId, status;
key_t shmKey;
char *shmAddr;

int main(int argc, char *argv[])
{
	int *turn;		// flags for keeping track of the current turn
	char *text;		// the message that will be passed to readers
	signal(SIGINT, quit);

	// Set Up –––––––––––––––––––––––––––
	shmKey = ftok("writer", 'A'); // use executable for the writer as file key
	if (shmKey == -1) {
		perror("ftok()");
		exit(1);
	}

	shmId = shmget(shmKey, 0, 0);
	if (shmId < 0) {
		perror("shmget()");
		exit(1);
	}

	shmAddr = shmat(shmId, NULL, 0);  // no addr or flg because we expect
	if (shmAddr == (void *) -1) {
		perror("shmat()");
		exit(1);
	}

	turn = (int *)shmAddr;				// Put turn array at beginning of shm
	text = shmAddr + (sizeof(int)*2);	// Put text after turn array
	turn[ME] = 0;		// writer goes first

	while (1)
	{
		/*
		The value turn[ME] is incrememnted for every reader process that
		enters the CS. When a reader leaves the CS, turn[ME] is decrememnted.
		turn[ME] must be zero before turn[OTHER] can be set to 1.
	 	0 = Not in CS; >0 = In CS
		*/

		while(turn[OTHER] == 1)
			;

		// usleep prevents concurrency issues, ensures every reader actually
		// performs a decremement
		usleep(1);
		turn[ME]++;

		// Critical Section –––––––––––––––––––
		fprintf(stderr, "Read from shared mem: %s", text);
		// End Critical Section –––––––––––––––

		usleep(1);
		turn[ME]--;

		//fprintf(stderr, "Turns: Me:%i  Other:%i\n", turn[ME], turn[OTHER]);  //debug
		//sleep(3);  //debug

		// Block readers that have already finished from pre-emptively setting
		// the writer's turn
		while(turn[ME] > 0)
		  	;
		turn[OTHER] = 1;
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

	// reader does not mark shared mem for deletion... i think

	exit(0);
}
