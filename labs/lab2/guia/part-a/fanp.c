#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int i, status;
    pid_t child, parent = getpid();
    for (i=0; i<4; i++)
        if ((child=fork())<=0) break;
        else fprintf(stderr, "This is iteration %d and process %d is born\n", i, child);
    fprintf(stderr, "I am process %d with parent %d\n\n", getpid(), getppid());
    if (getpid()==parent) { // only the original parent executes this
        // show pstree
        char cmd[200];
        snprintf(cmd, sizeof(cmd), "pstree %d > fanp.txt", parent);
        system(cmd);
        // children rest in peace
        for (i=0; i<4; i++) wait(&status);
        // parent rests in peace
    } else sleep(1);
    return 0;
}
