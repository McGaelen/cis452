//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#define MAX_LENGTH 80
#define MAX_ARGS 20

int main() {

	int i, status;
	char cmd[MAX_LENGTH]; // string containing user's input
	char *argv[MAX_ARGS]; // array of pointers to char arrays
	char *nl;			  // holds position of \n for trimming
	struct rusage *usage; // place to store child process usage


	while (1) {

		printf("shell$ ");
		fgets(cmd, MAX_LENGTH, stdin);
		nl = strchr(cmd, '\n');
		*nl = '\0';

		if (strcmp(cmd, "quit") == 0) 
		{
			exit(0);
		} 
		else 
		{
			// create array of command and parameters
			i = 0;
			argv[i] = strtok(cmd, " ");
			strcat(argv[i], "\0");

			while(argv[i] != NULL && i < MAX_ARGS) {
				//printf( " %s\n", argv[i] );
				argv[++i] = strtok(NULL, " ");
				strcat(argv[i], "\0");
			}

			int childpid = fork();
			//printf("CHILD: %d\n", childpid);

			if (childpid == 0) { // to debug child in gdb, do "set follow-fork-mode child" in gdb 
				//sleep(20); 	 // debug to check PID (use ps -A | grep shell)	
				execvp(argv[0], argv);
				printf("exec failed.\n");
				exit(1);

			} else {
				wait(&status);
				printf("\n");

				getrusage(RUSAGE_CHILDREN, usage);
				printf("User CPU usage: %ld\n", usage->ru_utime.tv_sec);
				printf("Involuntary context switches: %ld\n", usage->ru_nivcsw);
				//printf ("Child exited with status %i\n", status);
			}
		}
	}
}
