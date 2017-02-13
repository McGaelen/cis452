/*
	CIS 452 Lab4 - Blocking Multi-threaded Server
	Gaelen McIntee and Emily Wang
	2/7/17
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define MAX_NAME_LENGTH 50

void * getFile(void *arg);
void quit(int signum);

int requests = 0;	//keep track of how many requests we get

int main(void)
{
	pthread_t thread1;
	int status;
	char input[MAX_NAME_LENGTH];

	srand(time(NULL));
	signal(SIGINT, quit);

	while (1) //parent stays in loop until ^C
	{
		//ask for which file we want
		fprintf(stderr, "Which file do you want to access?  ");
		fgets(input, MAX_NAME_LENGTH, stdin);
		fprintf(stderr, "Searching for file %s...\n", input);

		//Make new thread to execute getFile with input as it's argument
		if ((status = pthread_create (&thread1, NULL,  getFile, input)) != 0) {
			fprintf (stderr, "thread create error %d\n", status);
			exit(1);
		}
	}
}

void * getFile(void *arg)
{
	char filename[MAX_NAME_LENGTH];
	strcpy(filename, (char *)arg);    //casting arg as a char * to save it in filename
	int chance = rand() % 100;		 //choose rand number between 0 and 99

	*( filename + (strlen(filename)-1) ) = '\0';  //strip off newline from filename (replace with null)

	if (chance >= 20) //chance is between 20 and 99
	{
		sleep(1);
		fprintf(stderr, "Found file %s in disk cache.\n", filename);
	}
	else if (chance < 20)  //chance is between 0 and 19
	{
		sleep( (rand()%4) + 7 ); //sleep 7-10 seconds randomly
		fprintf(stderr, "Found file %s in hard drive.\n", filename);
	}

	requests++;
	return NULL;
}

void quit(int signum)
{
	fprintf(stderr, " recieved. %i requests serviced. Exiting...\n", requests);
	exit(0);
}
