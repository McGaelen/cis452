/* 
 * CIS 452 Lab3 Program - Signal Handler
 * Gaelen McIntee and Emily Wang
 * 2/2/17
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void sigusr1(int signum); // handles SIGUSR1
void sigusr2(int signum); // handles SIGUSR2
void quit(int signum); 	  // handles ^C
int childpid;

int main(void) 
{
	int signals[2] = {10, 12}; // 10 = SIGUSR1, 12 = SIGUSR2
	signal(SIGUSR1, sigusr1);
	signal(SIGUSR2, sigusr2);
	signal(SIGINT, quit);
	srand(time(NULL));		// seeds random numbers

	childpid = fork();
	if (childpid) // Only want parent to print this out
		fprintf(stderr, "Spawned child PID# %i\n", childpid);

	while (1)
	{
		if (childpid < 0)
		{
			fprintf(stderr, "Fork failed.\n");
		}
		else if (!childpid)
		{
			sleep( 1 + (rand()%5) ); // Sleep for 1 to 5 seconds
			kill( getppid(), signals[rand()%2] ); // Pick one of the 2 signals randomly
		}
		else 
		{	
			fprintf(stderr, "Waiting...\t\t");
			sleep(5);
		}
	}
}

void sigusr1(int signum)
{
	fprintf(stderr, "Received SIGUSR1.\n");
}

void sigusr2(int signum)
{
	fprintf(stderr, "Received SIGUSR2.\n");
}

void quit(int signum)
{
	if (childpid) // Only want one of the processes to print this
		fprintf(stderr, " received. That's it, I'm shutting you down...\n");
	exit(0);
}
