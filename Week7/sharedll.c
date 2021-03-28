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

struct sembuf Pop = {.sem_num = 0,.sem_op = -1,.sem_flg = SEM_UNDO};
struct sembuf Vop = {.sem_num = 0,.sem_op = 1,.sem_flg = SEM_UNDO};

union semun {
	int              val;   
	struct semid_ds *buf;
    unsigned short  *array;
	struct seminfo  *__buf;
} setvalArg = {.val = 1};

struct SLL {
    int head;
    int count;
};

struct SNode {
    int data;
    int nextId;
};

int shlget(key_t key) {
    int shlid = shmget(key,sizeof(struct SLL),IPC_CREAT|0777);
    int semid = semget(key, NO_SEM, IPC_CREAT | 0777);
    semctl(semid, 0, SETVAL, setvalArg);
    struct SLL * list = shmat(shlid,NULL,0);
    if(list->count == 0) {
        list->head = -1;
    }
    shmdt(list);
    return shlid;
}

void addNodeToList(int data, key_t key) {
    int shlid = shmget(key,sizeof(struct SLL),IPC_CREAT|0777);
    int nodeid = shmget(IPC_PRIVATE,sizeof(struct SNode),IPC_CREAT|0777);
    int semid = semget(key, NO_SEM, IPC_CREAT | 0777);
    struct SNode * snode = shmat(nodeid,NULL,0);
    struct SLL * list = shmat(shlid,NULL,0);
    snode->data = data;
    snode->nextId = list->head;
    P(semid);
    list->head = nodeid;
    list->count += 1;
    getchar();
    V(semid);
    shmdt(snode);
    shmdt(list);
}

void printLL(key_t key) {
    int shlid = shmget(key,sizeof(struct SLL),IPC_CREAT|0777);
    struct SLL * list = shmat(shlid,NULL,0);
    struct SNode * snode = shmat(list->head,NULL,0);
    while(list->head != -1) {
        printf("%d \n",snode->data);
        if(snode->nextId == -1) {
            break;
        }
        snode = shmat(snode->nextId,NULL,0);
    };
}

int deleteNodeFromList(int num,key_t key) {
    int shlid = shmget(key,sizeof(struct SLL),IPC_CREAT|0777);
    struct SLL * list = shmat(shlid,NULL,0);
    struct SNode * snode = shmat(list->head,NULL,0);
    int semid = semget(key, NO_SEM, IPC_CREAT | 0777);
    int previd = list->head;
    int currid;
    int flag = 0;
    if(snode->data == num) {
        P(semid);
        previd = list->head;
        list->head = snode->nextId;
        shmctl(previd, IPC_RMID, NULL);
        V(semid);
        return 0;    
    }
    int i = 1;
    while(snode->nextId != -1) {
        i++;
        currid = snode->nextId;
        snode = shmat(snode->nextId,NULL,0);
        if(snode->data == num) {
            flag = 1;
            break;
        }
        previd = currid;
    }
    if(flag == 0) {
        return -1;
    }
    P(semid);
    snode = shmat(currid,NULL,0);
    struct SNode * pnode = shmat(previd,NULL,0);
    pnode->nextId = snode->nextId;
    shmctl(currid,IPC_RMID,NULL);
    V(semid);
    return i;
}