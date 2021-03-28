#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

void printMatrix(int r, int c, int mat[r][c]){
	int i, j;
       	for(i = 0; i < r; i++) {
		for(j = 0; j < c; j++) {
			printf("%d ,%d ,%d", mat[i][j],i,j);
		}
		printf("\n");
	}
}

int compute(int n, int m, int p,int A[n][m],int B[m][p],int i, int j) {
    int sum = 0;
    for(int k =0; k < m ; k++) {
        sum += A[i][k] * B[k][j];
    }
    return sum;
}

int shmid;

int main() {
    int status;
    //pid_t pid = 0;
    pid_t p1 = 0;
    int n,m,p;
    printf("Enter n: ");
    scanf("%d",&n);
    printf("Enter m: ");
    scanf("%d",&m);
    printf("Enter p: ");
    scanf("%d",&p);
    int A[n][m] , B[m][p] , C[n][p];
    printf("Enter A: \n");
    for(int i = 0 ; i < n; i++) {
        for(int j = 0; j < m; j++) {
            scanf("%d",&A[i][j]);
        }
    }
    printf("Enter B: \n");
    for(int i = 0 ; i < m; i++) {
        for(int j = 0; j < p; j++) {
            scanf("%d",&B[i][j]);
        }
    }
    shmid = shmget(IPC_PRIVATE,sizeof(int[n][p]),IPC_CREAT | 0777);
    printf("Shared memory id: %d\n",shmid);

    for(int i = 0 ; i < n; i++) {
        for(int j = 0; j < p; j++) {
            if(fork() == 0) {
                int *sm = (int *)shmat(shmid,NULL,0);
                int k;
                k = compute(n ,m ,p ,A,B,i,j);
                sm += (i*p) + j;
                *sm = k;
                shmdt(sm);
                exit(0);
            }
        }
    }
    sleep(1);
    wait(&status);
    int *sm = (int *)shmat(shmid,NULL,0);
    printf("The multiplied array is: \n");
    for(int i =0; i < n; i++) {
        for(int j = 0; j < p; j++) {
            printf("%d ", *(sm + (i*p) + j));
        }
        printf("\n");
    }
    shmdt(sm);
    shmctl(shmid, IPC_RMID, NULL);
}