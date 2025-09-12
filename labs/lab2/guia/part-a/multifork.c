#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define D1 "PID INDUK** ** pid (%5.5d) ** **************************\n"
#define D2 "val1(%5.5d) ** val2(%5.5d) ** val3(%5.5d) ** val4(%5.5d)\n"

int main() {
    pid_t val1, val2, val3, val4, parent=getpid();
    printf(D1, parent);
    fflush(stdout);
    val1 = fork();
    // waitpid(-1, NULL, 0);
    val2 = fork();
    // waitpid(-1, NULL, 0);
    val3 = fork();
    // waitpid(-1, NULL, 0);
    val4 = fork();
    // waitpid(-1, NULL, 0);
    if (parent==getpid()) {
        char cmd[200];
        snprintf(cmd, sizeof(cmd), "pstree %d > multifork.txt", parent);
        system(cmd);
    } else sleep(1);
    printf(D2, (int)val1, (int)val2, (int)val3, (int)val4);
    waitpid(-1, NULL, 0);
    return 0;
}

/* explanation:
 * waitpid makes parents wait for their children's status.
 * we comment them in order to get to see the full tree in an instant.
 * then we wait for the children with sleep() and use waitpid at the end
 * to avoid zombie processes being created.
 * in the example we see a multifork with just 3 processes. here is one with 4
 * to make clearer how is the tree actually being created.
*/
