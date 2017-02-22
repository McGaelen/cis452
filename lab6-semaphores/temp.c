#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SIZE 16

int main (int argc, char* argv[])
{
   int status;
   long int i, loop, temp, *shmPtr;
   int shmId;
   pid_t pid;
   struct sembuf *sbuf[1];


   loop = atoi(argv[1]);
   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("shmget()\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("shmId()\n");
      exit (1);
   }
   if (semctl(shmId, 0, 0, 1)< 0){
	perror("Cannot set semaphore value \n");
	} // initialize semaphore
   
   shmPtr[0] = 0;
   shmPtr[1] = 1;

  
   if (!(pid = fork())) {
     semop(shmId, &sbuf, 1); //wait?
     //sbuf[0].sem_op = 0; //wait
      for (i=0; i<loop; i++) {
		temp = shmPtr[0];
	      	shmPtr[0] = shmPtr[1];
	      	shmPtr[1] = temp;
               // swap the contents of shmPtr[0] and shmPtr[1]
      }
    
      if (shmdt (shmPtr) < 0) {
         perror ("shmdt()");
         exit (1);
      }
	semop(shmId, sbuf, 1); //signal?
	//sbuf[1].sem_flg = 0; //signal
   }
   else{
      semop(shmId, &sbuf, 1);
//       sbuf[0].sem_op = 0; //wait
	for (i=0; i<loop; i++) {
	
	      temp = shmPtr[0];
	      shmPtr[0] = shmPtr[1];
	      shmPtr[1] = temp;	      
	
               // swap the contents of shmPtr[1] and shmPtr[0]
      }
	semop(shmId, &sbuf, 1);
//     sbuf[1].sem_flg = 0; //signal
	}

  // wait (&status);
   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);


   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }

   return 0;
}
