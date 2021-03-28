#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h> 
#include <stdlib.h>

int shmid,n; 

int countStudents(int n) {
    int *sm = (int *)shmat(shmid,NULL,0);
    int count = 0;
    for (int i = 0 ; i < n; i++) {
        if(sm[i] != -1) {
            fprintf(stdout,"%d is present.The attendance was marked by process id %d.\n",i+1,sm[i]);
            count ++;
        }
    }
    shmdt(sm);
    return count;
}

void populateWithNeg() {
    int *sm = (int *)shmat(shmid,NULL,0);
    for (int i = 0 ; i < n; i++) {
        sm[i] = -1;
    }
    shmdt(sm);
}

typedef void (*sighandler_t)(int);
void releaseSHM(int signum) {
        int status;
        printf("\n");
        status = countStudents(n);
        if(status == 0) {
            fprintf(stderr,"No student attended the class till now!!\n");
        } else {
            fprintf(stdout,"Number of present students: %d\n",status);
        }
        status = shmctl(shmid, IPC_RMID, NULL);
        if (status == 0) {
                fprintf(stderr, "Remove shared memory with id = %d.\n", shmid);
        } else if (status == -1) {
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
        } else {
                fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", shmid);
        }
        fprintf(stdout, "Class Ending.\n");
        status = kill(0, SIGKILL);
        if (status == 0) {
                fprintf(stdout, "Class Ended.\n");
        } else if (status == -1) {
                perror("kill failed.\n");
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
        } else {
                fprintf(stderr, "kill(2) returned wrong value.\n");
        }
}

int main(int argc, char *argv[]) {
    int status;
    n = atoi(argv[1]);
    sighandler_t shandler;
    shandler =  signal(SIGINT, releaseSHM);
    key_t shkey;
    shkey = ftok(".",1);
    shmid =  shmget(shkey, sizeof(int[n]), IPC_CREAT | 0666);
    populateWithNeg(n);       
    printf("The space for %d students have been allocated at %d.\n",n,shmid);
    while(1) {
            
    } 
}