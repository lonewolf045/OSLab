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

int main(int argc, char *argv[]) {
    int status;
    pid_t pid=0;
    pid_t p1=0; 
    n = atoi(argv[1]);
    key_t shkey;
    shkey = ftok("/home/yellowsubmarine/OSLab/Week5",1);
    shmid =  shmget(shkey, sizeof(int[n]), IPC_CREAT | 0666);
    int *sm = (int *)shmat(shmid,NULL,0);
    sm[n-1] = getpid();     
    printf("Attendance Marked\n"); 
}