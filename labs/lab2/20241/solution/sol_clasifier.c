#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

// Saymon N. - 20211866

#define READ 0
#define WRITE 1

int main(int na, char *argv[]) {
    int n, i;
    if (na != 2) {
        fprintf(stderr, "Use: %s <n>\n", argv[0]);
        exit(1);
    }
    n = atoi(argv[1]);
    int fdp[3][2];
    for (i=0; i<3; i++) pipe(fdp[i]);
    for (i=0; i<3; i++) // each process will read from pipe[i]
        if (!fork()) {
            int j, r, mypid = getpid(), size=0;
            for (j=0; j<3; j++) // close unused pipe ends
                if (j != i) close(fdp[j][READ]);
            srand(mypid);			
            for (j=0; j<n; j++) {
                r = rand()%528;
                if (r%2) write(fdp[0][WRITE], &r, sizeof(r)); // to pipe 0
                else write(fdp[1][WRITE], &r, sizeof(r)); // to pipe 1
                if (r%10==1 || r%10==2) write(fdp[2][WRITE], &r, sizeof(r)); // to pipe 2
                printf("%d %d\n", mypid, r);
            }    
            for (j=0; j<3; j++) close(fdp[j][WRITE]);
            int* a = (int*)malloc(n*sizeof(int)); // array to get numbers
            while (read(fdp[i][READ], &r, sizeof(r))) a[size++] = r;
            close(fdp[i][READ]);
            if (!size) printf("--> PID[%d]: empty set!\n", mypid);
            else for (j=0; j<size; j++)
                printf("--> PID[%d]: %d\n", mypid, a[j]);
            free(a);
            exit(0);    
        }	
    for (i=0; i<3; i++) {
        close(fdp[i][READ]);
        close(fdp[i][WRITE]);
    }
    for (i=0; i<3; i++) waitpid(-1, NULL, 0);
    exit(0);	
}	

/* explanation:
* each process gets a value of i: 0, 1 or 2
* the value a process gets serves to identify the pipe from which it can read.
* when a process finishes writing all its numbers, it closes its pipe ends
*/
