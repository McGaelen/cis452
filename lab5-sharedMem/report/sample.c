#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define FOO 4096

int main ()
{
   int shmId;
   struct shmid_ds stats;
   char *shmPtr;

   if ((shmId = shmget (0x7777, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
  	perror ("i can't get no..\n");
  	exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
  	perror ("can't attach\n");
  	exit (1);
   }

   printf ("value a: %lu\t value b: %lu\n", (unsigned long) shmPtr, (unsigned long) shmPtr + FOO);

   pause();
   
   shmctl(shmId, IPC_STAT, &stats);
   printf("Size of the shared memory segment: %lu\n", stats.shm_segsz);
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