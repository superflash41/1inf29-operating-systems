#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BFSZ 100
#define READ 0
#define WRITE 1

int main(int narg, char** argv) {
    int i, n = atoi(argv[1]);
    char msg[BFSZ];
    int** fdp = (int**)malloc(n*sizeof(int*));
    for (i=0; i<n; i++) {
        fdp[i] = (int*)malloc(2*sizeof(int));
        pipe(fdp[i]);
    }
    for (i=0; i<n-1; i++) if (fork()) break;
    // each process will WRITE on pipe[i]
    // and will READ from pipe[(i+n-1)%n]
    close(fdp[i][READ]);
    close(fdp[(i+n-1)%n][WRITE]);
    for (int k=0; k<n; k++)
        if (k!=i && k!=((i+n-1)%n)) {
            close(fdp[k][WRITE]);
            close(fdp[k][READ]);
        }
    memset(msg, 0, BFSZ);
    if (!i) { // process 0
        sprintf(msg, "E.T. phone home\n");
        write(fdp[i][WRITE], msg, sizeof(msg));
        close(fdp[i][WRITE]);
        printf("Message sent from process[%d]\n", getpid());
        memset(msg, 0, BFSZ);
        while (read(fdp[n-1][READ], msg, sizeof(msg))) {
            waitpid(-1, NULL, 0);
            printf("Message received at process[%d]: %s\n", getpid(), msg);
        }
        close(fdp[n-1][READ]);
    } else {
        while (read(fdp[(i+n-1)%n][READ], msg, sizeof(msg))) {
            write(fdp[i][WRITE], msg, sizeof(msg));
        }
        close(fdp[i][WRITE]);
        close(fdp[(i+n-1)%n][READ]);
        printf("Message sent from process[%d]\n", getpid());
    }
    return 0;
}
