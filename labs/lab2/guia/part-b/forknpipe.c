#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BFSZ 100
#define LOOP 5

int main() {
    char buff[BFSZ];
    char msg[]="Hello!\n";
    int ii, pfd[2];
    pipe(pfd);
    if (!fork()) { // child
        close(pfd[0]); // not going to read
        printf("I am PID[%d] (child)\n", getpid());
        for (ii=0; ii<LOOP; ii++)
            write(pfd[1], msg, sizeof(msg)-1);
        close(pfd[1]); // no more writing
    } else { // parent
        close(pfd[1]); // not going to write
        printf("I am PID[%d] (parent)\n", getpid());
        memset(buff, 0, sizeof(buff));
        while ((ii=read(pfd[0], buff, sizeof(buff)))) {
            printf("PARENT READ[%d]: \n%s\n", ii, buff);
            memset(buff, 0, sizeof(buff));
        }
        close(pfd[0]); // no more reading
    }
}
