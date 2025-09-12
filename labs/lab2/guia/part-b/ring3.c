#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define BFSZ 200
#define READ 0
#define WRITE 1

void strrev(char* s) {
    for (int i=0, n=strlen(s); i<n/2; i++) {
        char c = s[i];
        s[i] = s[n-i-1];
        s[n-i-1] = c;
    }
}

void strcap(char* s) {
    for (int i=0; s[i]; i++)
        s[i] -= (s[i]>='a' && s[i]<='z') ? 'a'-'A' : 0;
}

void process(int rfdp, int wfdp, char* msg, void(*fun)(char*)) {
    while(read(rfdp, msg, BFSZ)) {
        fun(msg);
        write(wfdp, msg, strlen(msg)+1);
    }
    close(wfdp);
    close(rfdp);
}

int main() {
    int i;
    char msg[BFSZ], buff[BFSZ];
    int fdp[3][2];
    for (i=0; i<3; i++) pipe(fdp[i]);
    for (i=0; i<2; i++) if (fork()) break;
    // each process will WRITE on pipe[i] and will READ from pipe[(i+n-1)%n]
    close(fdp[i][READ]);
    close(fdp[(i+2)%3][WRITE]);
    for (int k=0; k<3; k++)
        if (k!=i && k!=((i+2)%3)) {
            close(fdp[k][WRITE]);
            close(fdp[k][READ]);
        }
    memset(msg, 0, BFSZ);
    memset(buff, 0, BFSZ);
    if (!i) { // process 0
        printf("Please input a string: ");
        fgets(msg, sizeof(msg), stdin); msg[strlen(msg)-1]=0; // remove '\n'
        write(fdp[i][WRITE], msg, strlen(msg)+1);
        close(fdp[i][WRITE]);
        memset(msg, 0, BFSZ);
        printf("Processed string is: ");
        while (read(fdp[2][READ], msg, sizeof(msg))) {
            waitpid(-1, NULL, 0);
            printf("%s\n", msg);
        }
        close(fdp[2][READ]);
    } else if (i==1) // reverse
        process(fdp[(i+2)%3][READ], fdp[i][WRITE], msg, strrev);
    else // capitalize
        process(fdp[(i+2)%3][READ], fdp[i][WRITE], msg, strcap);
    wait(NULL);
    return 0;
}
