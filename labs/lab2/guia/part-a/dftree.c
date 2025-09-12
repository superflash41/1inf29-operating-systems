#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int n, char** s) {
    int depth = atoi(s[1]);
    depth--; // based on how the problem defines depth
    pid_t parent = getpid(), dad;
    for (int i=0; i<depth; i++) {
        dad = getpid();
        if (fork()) {
            wait(NULL); // wait for one subtree
            fork();
            wait(NULL); // wait for the other subtree
        }
        if (dad==getpid()) break;
    }
    printf("I am process %d and my parent is %d\n", getpid(), getppid());
    return EXIT_SUCCESS;
}

/*
* children sleep before dying
* and this allows the creation of the tree
* to be in depth first
*/
