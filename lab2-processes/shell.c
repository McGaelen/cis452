#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#define MAX_LENGTH 80
#define MAX_ARGS 20
#define DELIMS " \t\r\n"

int main() {

	int i, status;
	long oldswitches = 0, newswitches; // for finding # of context switches
	char cmd[MAX_LENGTH];	// string containing user's input
	char *argv[MAX_ARGS];	// array of pointers to char arrays
	struct rusage usage;	// place to store child process usage

	while(1){
		printf("shell$ ");
		fgets(cmd, MAX_LENGTH, stdin);	// show prompt

		if (strcmp(cmd, "quit\n") == 0)	
			exit(0);

		i = 0;		// build array of arguments
		argv[i] = strtok(cmd, DELIMS);
		while (argv[i] != NULL)
		{
			argv[++i] = strtok (NULL, DELIMS);
		}


		int pid = fork();

		if (pid == 0) 	  // execute commands in argv if we're the child
		{
			execvp(*argv, argv);
			printf("exec failed.\n"); 
			exit(1);
		}
		else if (pid < 0)  // if fork() failed
		{
			printf("Error\n");
			exit(1);
		}
		else			  // parent waits
		{
			wait(&status);
		//	printf("Child exited\n");
		}

		getrusage(RUSAGE_CHILDREN, &usage);		// populates usage struct
		printf("user CPU time used: %ld microseconds\n", usage.ru_utime.tv_usec);

		// subtract old switches from new switches to get value from each individual child
		newswitches = usage.ru_nivcsw;
		printf("involuntary context switches: %ld\n", (newswitches - oldswitches));
		oldswitches = newswitches;
	}

	return 0;
}
