#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define BFSZ 100
#define READ 0
#define WRITE 1

int main() {
    char buff1[BFSZ], buff2[BFSZ];
    int p, ii, jj, fdp[4][2];
    pid_t mypid;
    memset(buff1, 0, BFSZ);
    memset(buff2, 0, BFSZ);
    for (ii=0; ii<4; ii++) pipe(fdp[ii]);
    // our process has 4 pipes
    ii = fork()?0:2;
    jj = fork()?0:1;
    // 4 processes, each with 4 pipe ends
    close(fdp[ii+jj][READ]); // EACH PROCESS WRITES ON PIPE[ii+jj]
    close(fdp[(ii+jj+3)%4][WRITE]); // EACH PROCESS READS FROM PIPE[(ii+jj+3)%4]
    close(fdp[(ii+jj+1)%4][WRITE]);
    close(fdp[(ii+jj+1)%4][READ]);
    close(fdp[(ii+jj+2)%4][WRITE]);
    close(fdp[(ii+jj+2)%4][READ]);
    // each process writes on buff1
    mypid = getpid();
    sprintf(buff1, "A message from PID[%d]\n", mypid);
    write(fdp[ii+jj][WRITE], buff1, sizeof(buff1));
    close(fdp[ii+jj][WRITE]);
    // each process reads on buff2
    while (read(fdp[(ii+jj+3)%4][READ], buff2, sizeof(buff2))) {
        waitpid(-1, NULL, 0);
        printf("PID[%d] is reading: \n%s\n", mypid, buff2);
    }
    close(fdp[(ii+jj+3)%4][READ]);
    return 0;
}
