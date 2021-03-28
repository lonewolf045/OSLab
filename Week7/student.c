#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h> 
#include <stdlib.h>
#include <sys/sem.h>

#define NO_SEM	1

#define P(s) semop(s, &Pop, 1);
#define V(s) semop(s, &Vop, 1);

struct sembuf Pop;
struct sembuf Vop;

int shmid1,shmid2,n; 
int semid1,semid2;

int main(int argc, char *argv[]) {
    key_t mykey1,mykey2;

	union semun {
		int              val;    
		struct semid_ds *buf;    
		unsigned short  *array;  
		struct seminfo  *__buf;
	} setvalArg;

    setvalArg.val = 1;

	Pop.sem_num = 0;
	Pop.sem_op = -1;
	Pop.sem_flg = SEM_UNDO;

	Vop.sem_num = 0;
	Vop.sem_op = 1;
	Vop.sem_flg = SEM_UNDO;

    int status;
    pid_t pid=0;
    pid_t p1=0; 
    n = atoi(argv[1]);
    key_t shkey1,shkey2;

    mykey1 = ftok(".", 3);
	semid1 = semget(mykey1, NO_SEM, IPC_CREAT | 0777);
	status = semctl(semid1, 0, SETVAL, setvalArg);
    mykey2 = ftok(".", 4);
	semid2 = semget(mykey2, NO_SEM, IPC_CREAT | 0777);
	//status = semctl(semid2, 0, SETVAL, setvalArg);

    shkey1 = ftok("/home/yellowsubmarine/OSLab/Week7",1);
    shkey2 = ftok("/home/yellowsubmarine/OSLab/Week7",2);
    shmid1 =  shmget(shkey1, sizeof(int), IPC_CREAT | 0666);
    shmid2 =  shmget(shkey2, sizeof(int), IPC_CREAT | 0666);


    P(semid1);
    int * count = (int *)shmat(shmid1,NULL,0);
    int * roll = (int *)shmat(shmid2,NULL,0);
    *count += 1;
    *roll = n;
    V(semid2);  

    printf("Process:Initialised\n"); 
}