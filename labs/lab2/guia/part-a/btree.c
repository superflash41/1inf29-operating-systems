#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int final;

void maketree(int x) {
    char s[60];
    sprintf(s, "I am process %d with %d and ppid %d\n", x, getpid(), getppid());
    write(1, s, strlen(s));
    if (x>=final) return;
    if (!fork()) { maketree(2*x); sleep(1); exit(0); }
    if (!fork()) { maketree(2*x+1); sleep(1); exit(0); }
    // wait(NULL);
    // wait(NULL);
}

int main(int narg, char** argv) {
    int n = atoi(argv[1]);
    final = 1<<(n-1);
    maketree(1);
    // only parent reaches here
    char cmd[200];
    snprintf(cmd, sizeof(cmd), "pstree -c %d > btree.txt", getpid());
    system(cmd);
    return EXIT_SUCCESS;
}

/* explanation:
* children sleep before dying
*/
