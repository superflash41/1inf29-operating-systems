#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int n, char** s) {
    if (n < 2) {
        fprintf(stderr, "depth of the tree not specified!\n ");
        exit(EXIT_FAILURE);
    }
    int depth = atoi(s[1]);
    depth--; // based on how the problem defines depth
    pid_t parent = getpid(), dad;
    for (int i=0; i<depth; i++) {
        dad = getpid();
        if (fork()) fork();
        if (dad==getpid()) break;
    }
    if (parent==getpid()) {
        char cmd[200];
        snprintf(cmd, sizeof(cmd), "pstree -c %d > tree.txt", parent);
        system(cmd);
    } else sleep(1);
    waitpid(-1, NULL, 0);
    return EXIT_SUCCESS;
}

/* explanation:
* each parent gets two children
*/
