#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "sharedll.h"

int main() {
    key_t mykey = ftok(".",1);
    int shlid = shlget(mykey);
    addNodeToList(5,mykey);
    addNodeToList(4,mykey);
    addNodeToList(3,mykey);
    printf("%x\n",mykey);
    printLL(mykey);
} 