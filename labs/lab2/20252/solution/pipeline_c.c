#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BFSZ 400
#define READ 0
#define WRITE 1

// sn

void process(int rfdp, int wfdp, char* cmd) {
    char path[BFSZ];
    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "/bin/%s", cmd);
    dup2(rfdp, 0);
    dup2(wfdp, 1);
    execl(path, cmd, NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) return 1; // we need more commands
    int i, n = argc;
    char msg[BFSZ], buff[BFSZ];
    int** fdp = (int**)malloc(n*sizeof(int*));
    for (i=0; i<n; i++) {
        fdp[i] = (int*)malloc(2*sizeof(int));
        pipe(fdp[i]);
    }
    for (i=0; i<n-1; i++) if (fork()) break;
    // each process will WRITE on pipe[i] and will READ from pipe[(i+n-1)%n]
    close(fdp[i][READ]);
    close(fdp[(i+n-1)%n][WRITE]);
    for (int k=0; k<n; k++)
        if (k!=i && k!=((i+n-1)%n)) {
            close(fdp[k][WRITE]);
            close(fdp[k][READ]);
        }
    memset(msg, 0, BFSZ);
    memset(buff, 0, BFSZ);
    if (!i) { // process 0
        close(fdp[0][WRITE]);
        while (read(fdp[n-1][READ], msg, sizeof(msg))) {
            printf("%s", msg);
            fflush(stdout);
        }
        close(fdp[n-1][READ]);
    } else process(fdp[(i+n-1)%n][READ], fdp[i][WRITE], argv[i]);
    wait(NULL);
    return 0;
} 
