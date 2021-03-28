#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#include <stdlib.h>


#define NO_SEM	1

#define P(s) semop(s, &Pop, 1);
#define V(s) semop(s, &Vop, 1);



struct sembuf Pop;
struct sembuf Vop;



int main(int argc, char *argv[]) {
    printf("First run helper program if you haven't done that. till now!!!\n");
	key_t mykey;

	int semid,n;

    if(argc !=  1) {
        n = atoi(argv[1]);
    } else {
        n = -1;
    }
		
    Pop.sem_num = 0;
	Pop.sem_op = -1;
	Pop.sem_flg = SEM_UNDO;

	Vop.sem_num = 0;
	Vop.sem_op = 1;
	Vop.sem_flg = SEM_UNDO;

	mykey = ftok(".", 2);
	semid = semget(mykey, NO_SEM, 0777);
    
    P(semid);
    while(1) {
            fprintf(stdout, "I am Student %d.\n",n);
            sleep(1); 
    }
    V(semid);

    return 0;
}