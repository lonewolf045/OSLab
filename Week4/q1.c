#include <stdio.h>
#include <unistd.h> /* for fork() */
#include <sys/wait.h> /* for wait() */
#include <sys/types.h> /* for wait() kill(2)*/
#include <sys/ipc.h> /* for shmget() shmctl() */
#include <sys/shm.h> /* for shmget() shmctl() */

#include <signal.h> /* for signal(2) kill(2) */

#include <errno.h> /* for perror */

#include <stdlib.h>

int shmid;

int main() {
    int status;
    //pid_t pid = 0;
    pid_t p1 = 0;
    shmid = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT | 0777);
    printf("Shared memory id: %d\n",shmid);
    p1 = fork();
    if(p1 == 0) {
        int i;
        int *sm;
        sm = shmat(shmid,NULL,0);
        for(i = 0; i < 20; i += 2) {
            sleep(0.5);
            printf("The child process is reading: %d\n",*sm);
            getchar();
        }
        exit(0);
    } else {
        srand(0);
        int i;
        int *sm;
        sm = shmat(shmid,NULL,0);
        for(i = 0; i < 20; i += 2) {
            sleep(0.5);
            *sm = rand()%500;
            printf("The parent process is writing: %d\n",*sm);
            getchar();
        }
        wait(&status);
        shmctl(shmid, IPC_RMID, NULL);
        exit(0);
    }
}