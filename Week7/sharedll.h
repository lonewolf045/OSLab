#ifndef sharedll_h__
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
#define sharedll_h__

struct SLL {
    int head;
    int count;
};

struct SNode {
    int data;
    int nextId;
};

extern int shlget(key_t key);
extern void addNodeToList(int data, key_t key);
extern void printLL(key_t key);
extern int deleteNodeFromList(int num,key_t key);

#endif // sharedll_h__