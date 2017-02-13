#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

void sigUSR(int);
void sigUSR2(int);
void sigQuit(int);

int main(){
	pid_t pid, ppid;
	ppid = getpid();
//	printf("%d\n", ppid);
	signal(SIGHUP, sigUSR);
	signal(SIGHUP, sigUSR2);
	signal(SIGINT, sigQuit);
	srand(time(NULL));
	pid = fork();
	if (pid < 0){
		printf("Error spawning child\n");
		exit(1);
	}
	
	if (pid == 0){ //child
	while(1){	
		printf("waiting...\n");
             //	sleep(rand());
		sleep(3);
		if (rand() % 2 == 0){
			kill(ppid, SIGHUP);
		}
		else{
			kill(ppid, SIGHUP);
		
		}
	}
	}
	else{ //parent
		printf ("spawned child PID# %d\n", pid);

		while(1){
			signal(SIGHUP, sigUSR);
        		signal(SIGHUP, sigUSR2);
        		signal(SIGINT, sigQuit);
		}
		//signal(SIGTERM, sigQuit);
	}

	return 0;
}

void sigUSR(int sigNum){
	printf("       recieved a SIGUSR1 signal\n");
}

void sigUSR2(int sigNum){
	printf("       recieved a SIGUSR2 signal\n");
}

void sigQuit(int sigNum){
	printf("       ^C recieved. That's it, I'm shutting you down...\n");
	exit(0);
}
