#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int i, status;
    pid_t child, parent = getpid();
    for (i=0; i<3; i++) if ((child=fork())) break;
    fprintf(stderr, "Iteration %d\n", i);
    fprintf(stderr, "Received fork: %d\n", child);
    fprintf(stderr, "I am process %d with parent %d\n\n", getpid(), getppid());
    if (getpid()==parent) { // only the original parent executes this
        char cmd[200];
        snprintf(cmd, sizeof(cmd), "pstree %d > chainp.txt", parent);
        system(cmd);
    } else sleep(1); // children sleeping
    wait(&status);
    return 0;
}
