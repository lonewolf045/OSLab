#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include<signal.h>

#define NO_SEM	1

int semid;

typedef void (*sighandler_t)(int);
void releaseSHM(int signum) {
        int status;
        printf("\n");
        status = semctl(semid, 0,IPC_RMID, NULL);
        if (status == 0) {
                fprintf(stderr, "Remove semaphore memory with id = %d.\n", semid);
        } else if (status == -1) {
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", semid);
        } else {
                fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", semid);
        }
        fprintf(stdout, "Process Ending.\n");
        
		status = kill(0, SIGKILL);
        if (status == 0) {
                fprintf(stdout, "Class Ended.\n");
        } else if (status == -1) {
                perror("kill failed.\n");
                fprintf(stderr, "Cannot remove shared memory with id");
        } else {
                fprintf(stderr, "kill(2) returned wrong value.\n");
        }
}

int main() {
	key_t mykey;
	
	int status;

	sighandler_t shandler;
    shandler =  signal(SIGINT, releaseSHM);

	union semun {
		int val;
		struct semid_ds *buf;
		unsigned short *array;
		struct seminfo *__buf;
	} setvalArg;

	setvalArg.val = 3;

	mykey = ftok(".", 2);
	
    semid = semget(mykey, NO_SEM, IPC_CREAT | 0777);
	
    status = semctl(semid, 0, SETVAL, setvalArg);
	printf("Press ctrl+C after ending all the student processes.\n");
	while(1) {

	}
}