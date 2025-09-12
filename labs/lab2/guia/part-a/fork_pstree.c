#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child;
    if ((child = fork())) {
        fprintf(stderr, "Fork returned %d, I am the father with ID = %d\n", child, getpid());
        sleep(3); // waits 3 seconds
    } else {
        fprintf(stderr, "Fork returned %d, I am the child with ID = %d\n", child, getpid());
        pid_t parent = getppid();
        // insert the parent id into the command 
        char cmd[200];
        snprintf(cmd, sizeof(cmd), "pstree %d > process.txt", parent);
        system(cmd);
        // show the contents of the file
        snprintf(cmd, sizeof(cmd), "echo -n 'Output: ' && cat process.txt");
        system(cmd);
    }
    return 0;
}
