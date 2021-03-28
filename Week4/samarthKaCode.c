#include <stdio.h>
#include <unistd.h> /* for fork() */
#include <sys/wait.h> /* for wait() */
#include <sys/types.h> /* for wait() kill(2)*/
#include <sys/ipc.h> /* for shmget() shmctl() */
#include <sys/shm.h> /* for shmget() shmctl() */
#include <signal.h> /* for signal(2) kill(2) */
#include <errno.h> /* for perror */
#include <stdlib.h>

void printMatrix(int r, int c, int mat[][c]){
	int i, j;
       	for(i = 0; i < r; i++) {
		for(j = 0; j < c; j++) {
			printf("%d ,%d ,%d", mat[i][j],i,j);
		}
		printf("\n");
	}
}

int compute(int n, int m, int p, int A[][m], int B[][p],int i, int j) {
    int sum = 0;
    for(int k =0; k < m ; k++) {
        sum += A[i][k]*B[k][j];
    }
    return sum;
}

int main() {
	int ar, ac, br, bc; //variables for the dimensions of the matrices to be multiplied
	int i, j, k;
	int shmid;
	pid_t p;
	printf("Enter the number of rows in matrix a: ");
	scanf("%d", &ar);
	printf("Enter the number of columns in matrix a: ");
        scanf("%d", &ac);
	printf("Enter the number of rows in matrix b: ");
        scanf("%d", &br);
	printf("Enter the number of columns in matrix b: ");
        scanf("%d", &bc);
	if (ac != br) {
		printf("These matrices cannot be multiplied. Check your dimensions");
		return 0;
	}
	int a[ar][ac];
	int b[br][bc];
	printf("Enter the values in matrix a in row major order: \n");
	for(i = 0; i < ar; i++) {
	       for(j = 0; j < ac; j++) {
			scanf("%d", &a[i][j]);
		}
	}	
 	printf("Enter the values in matrix b in row major order: \n");
        for(i = 0; i < br; i++) {
               for(j = 0; j < bc; j++) {
                        scanf("%d", &b[i][j]);
                }
        }
	printMatrix(ar, ac, a);
	printMatrix(br, bc, b);
	
	
	shmid = shmget(IPC_PRIVATE,sizeof(int[ar][bc]),IPC_CREAT | 0777);
    	printf("Shared memory id: %d\n",shmid);
	

	for(i = 0; i < ar; i++) {
		for(j = 0; j < bc; j++) {
			p = fork();
			if(!p) {
				int (*child_c)[bc];
				child_c = shmat(shmid, NULL, 0);
				if (child_c == (void *)-1) {
            				/* shmat fails */
            				perror("shmat() failed at child: ");
            				exit(1);
          			}
				child_c[i][j] = compute(ar, ac, bc, a, b, i, j);
				shmdt(child_c);
				exit(0);
			} 
		}
	}
	
	int (*c)[bc];
	c = shmat(shmid, NULL, 0);
	printMatrix(ar, bc, c);	

	shmdt(c);
    	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
