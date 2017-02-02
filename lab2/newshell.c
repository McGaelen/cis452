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
	char cmd[MAX_LENGTH];
	char *argv[MAX_ARGS]; 
	struct rusage usage;
	int oldSwitch = 0;
	//struct timeval = oldTime;

	while(1){
		printf("shell$ ");
		fgets(cmd, MAX_LENGTH, stdin);
	
		if (strcmp(cmd, "quit\n") == 0)
			exit(0);

		i = 0;
		argv[i] = strtok(cmd, DELIMS);
		while (argv[i] != NULL){
		//	argv[i] = strcat(argv[i], '\0');
			argv[++i] = strtok (NULL, DELIMS);
			
		}
		
		

		int pid = fork();
		
		if (pid == 0) {
			//	i = 0;
			//	argv = strtok(cmd, " ");

			//	while(argv != NULL){
			//		argv = strtok(NULL, " ");
			//	}
			execvp(*argv, argv);
				printf("exec failed.\n");
				exit(1);
			
		}
		else if (pid < 0){
			printf("Error\n");
			exit(1);
		}
		else{
			wait(&status);
		//	printf("Child exited\n");
		}
		getrusage(RUSAGE_SELF, &usage);

		printf("user CPU time used: %ld.%06ld\n", usage.ru_utime.tv_sec, usage.ru_stime.tv_usec);
		printf("involuntary context switches: %ld\n", usage.ru_nivcsw - oldSwitch);
		//oldTime = usage.ru_utime;
		oldSwitch = usage.ru_nivcsw;
		//printf("%d\n", oldSwitch);
	}
	
	return 0;
}
