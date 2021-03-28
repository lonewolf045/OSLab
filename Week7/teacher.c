#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <errno.h> 
#include <stdlib.h>

#define NO_SEM	1

#define P(s) semop(s, &Pop, 1);
#define V(s) semop(s, &Vop, 1);

struct sembuf Pop;
struct sembuf Vop;

int shmid1,shmid2,n;
int semid1,semid2; 

typedef void (*sighandler_t)(int);
void releaseSHM(int signum) {
        int status;
        printf("\n");
        //status = countStudents(n);
        int * count = (int *)shmat(shmid1,NULL,0);
        status = *count;
        if(status == 0) {
            fprintf(stderr,"No student attended the class till now!!\n");
        } else {
            fprintf(stdout,"Number of present students: %d\n",status);
        }
        status = shmctl(shmid1, IPC_RMID, NULL);
        status = shmctl(shmid2, IPC_RMID, NULL);
        if (status == 0) {
                fprintf(stderr, "Remove shared memory with id = %d.\n", shmid1);
        } else if (status == -1) {
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid1);
        } else {
                fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", shmid1);
        }
        fprintf(stdout, "Class Ending.\n");
        status = kill(0, SIGKILL);
        if (status == 0) {
                fprintf(stdout, "Class Ended.\n");
        } else if (status == -1) {
                perror("kill failed.\n");
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid1);
        } else {
                fprintf(stderr, "kill(2) returned wrong value.\n");
        }
}

int main(int argc, char *argv[]) {
    key_t mykey1,mykey2;

	union semun {
		int              val;    
		struct semid_ds *buf;    
		unsigned short  *array;  
		struct seminfo  *__buf;
	} setvalArg;

    setvalArg.val = 0;

	Pop.sem_num = 0;
	Pop.sem_op = -1;
	Pop.sem_flg = SEM_UNDO;

	Vop.sem_num = 0;
	Vop.sem_op = 1;
	Vop.sem_flg = SEM_UNDO;

    int status;
    n = atoi(argv[1]);
    sighandler_t shandler;
    shandler =  signal(SIGINT, releaseSHM);
    
    mykey1 = ftok(".", 3);
	semid1 = semget(mykey1, NO_SEM, IPC_CREAT | 0777);
	//status = semctl(semid1, 0, SETVAL, setvalArg);
    mykey2 = ftok(".", 4);
	semid2 = semget(mykey2, NO_SEM, IPC_CREAT | 0777);
	status = semctl(semid2, 0, SETVAL, setvalArg);
    
    key_t shkey1,shkey2;
    shkey1 = ftok("/home/yellowsubmarine/OSLab/Week7",1);
    shkey2 = ftok("/home/yellowsubmarine/OSLab/Week7",2);
    shmid1 =  shmget(shkey1, sizeof(int), IPC_CREAT | 0666);
    shmid2 =  shmget(shkey2, sizeof(int), IPC_CREAT | 0666);
    
    int * count = (int *)shmat(shmid1,NULL,0);
    int * roll = (int *)shmat(shmid2,NULL,0);
    *count = 0;
    *roll = -1;

    int lroll[n];
    for(int i = 0; i < n; i++) {
        lroll[i] = -1;
    }

    printf("The space for %d students have been allocated at %d.\n",n,shmid1);
    while(1) {
        P(semid2);
        if(*count <= n) {
            lroll[*roll - 1] = *roll;
            printf("%d has marked the attendance.\n",*roll);
            *roll = -1;
            
        } else {
            *count -= 1;
            *roll = -1;
        }
        V(semid1);
    } 
}