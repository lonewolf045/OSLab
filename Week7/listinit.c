#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "sharedll.h"

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("Too few parameters passed");
        exit(0);
    }
    for(int i = 1; i < argc; i++) {
        int nkey = atoi(argv[i]);
        key_t key = ftok(".",nkey);
        int id = shlget(key);
        printf("The list with key, %x, has bee created with id %d.\n",key,id);
    }
}