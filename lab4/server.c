#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define MAX_NAME_LENGTH 50

void * getFile(void *);
void quit(int signum);

int main(void)
{
	pthread_t thread1;
	int status;
	char input[MAX_NAME_LENGTH];

	srand(time(NULL));
	signal(SIGINT, quit);

	while (1)
	{
		fprintf(stderr, "Which file do you want to access?  ");
		fgets(input, MAX_NAME_LENGTH, stdin);
		fprintf(stderr, "Searching for file %s...\n", input);

		if ((status = pthread_create (&thread1, NULL,  getFile, input)) != 0) {
			fprintf (stderr, "thread create error %d\n", status);
			exit(1);
		}

	}
}

void * getFile(void *args)
{
	char *filename = (char *) args;
	int chance = rand() % 100;

	*(filename + (strlen(filename)-1) ) = '\0';

	if (chance > 20)
	{
		sleep(1);
		fprintf(stderr, "Found file %s in disk cache.\n", filename);
	}
	else if (chance <= 20)
	{
		sleep( (rand()%4) + 7 );
		fprintf(stderr, "Found file %s in hard drive.\n", filename);
	}

	return NULL;
}

void quit()
{
	fprintf(stderr, " recieved, exiting...\n");
	exit(0);
}
