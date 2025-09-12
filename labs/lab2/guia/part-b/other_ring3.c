#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BFSZ 400
#define READ 0
#define WRITE 1

void process(int rfdp, int wfdp, char* cmd) {
    char path[BFSZ];
    memset(path, 0, sizeof(path));
    snprintf(path, sizeof(path), "/bin/%s", cmd);
    dup2(rfdp, 0);
    dup2(wfdp, 1);
    execl(path, cmd, NULL);
}

int main() {
    int i;
    char msg[BFSZ], buff[BFSZ];
    int fdp[4][2];
    for (i=0; i<4; i++) pipe(fdp[i]);
    for (i=0; i<3; i++) if (fork()) break;
    // each process will WRITE on pipe[i] and will READ from pipe[(i+n-1)%n]
    close(fdp[i][READ]);
    close(fdp[(i+5)%4][WRITE]);
    for (int k=0; k<4; k++)
        if (k!=i && k!=((i+5)%4)) {
            close(fdp[k][WRITE]);
            close(fdp[k][READ]);
        }
    memset(msg, 0, BFSZ);
    memset(buff, 0, BFSZ);
    if (!i) { // process 0
        close(fdp[0][WRITE]);
        while (read(fdp[1][READ], msg, sizeof(msg))) {
            printf("%s", msg);
            fflush(stdout);
        }
        close(fdp[1][READ]);
    } else if (i==1)
        process(fdp[(i+5)%4][READ], fdp[i][WRITE], "wc");
    else if (i==2) 
        process(fdp[(i+5)%4][READ], fdp[i][WRITE], "sort");
    else
        process(fdp[(i+5)%4][READ], fdp[i][WRITE], "ps");
    wait(NULL);
    return 0;
} 
