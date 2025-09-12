#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int ii=0;
    pid_t parent = getpid();
    if (!fork()) ii++;
    // waitpid(-1, NULL, 0);
    if (!fork()) ii++;
    // waitpid(-1, NULL, 0);
    if (!fork()) ii++;
    // waitpid(-1, NULL, 0);
    if (!fork()) ii++;
    // waitpid(-1, NULL, 0);
    if (parent==getpid()) {
        char cmd[200];
        snprintf(cmd, sizeof(cmd), "pstree %d > isengfork.txt", parent);
        system(cmd);
    } else sleep(1);
    printf("Result = %3.3d \n", ii);
    waitpid(-1, NULL, 0);
    return 0;
}

/* explanation:
 * this process generates the same tree as the previous one
 * but what differs from multifork.c is that this one modifies
 * the value of a variable ii only when we are one level deeper in the tree
*/
