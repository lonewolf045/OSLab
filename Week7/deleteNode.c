#include<stdio.h>
#include <stdlib.h>
#include "sharedll.h"

int main(int argc, char *argv[]) {
    if(argc%2 == 0 || argc == 1) {
        printf("Too few or incorrect parameters parameters passed");
        exit(0);
    }
    for(int i = 1; i < argc; i+=2) {
        int nkey = atoi(argv[i]);
        key_t key = ftok(".",nkey);
        int n = atoi(argv[i+1]);
        int i = deleteNodeFromList(n,key);
        printf("%d\n",i);
    }
}