#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "sharedll.h"

int main() {
    key_t mykey = ftok(".",1);
    int shlid = shlget(mykey);
    struct SLL * list = shmat(shlid,NULL,0);
    printf("%x\n",mykey);
    printf("%d\n",list->head);
    printLL(mykey);
} 