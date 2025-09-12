#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

// Saymon N. - 20211866

#define READ 0
#define WRITE 1
#define CMDSZ 100

int cmp(const void* a, const void* b) { return *((int*)b) - *((int*)a); }
void close_fdp(int* fdp) { close(fdp[READ]); close(fdp[WRITE]); }

int main(int na, char** argv) {
    int i, j, r, n = atoi(argv[1]);
    srand(time(NULL));
    int** fdp = (int**)malloc(n*sizeof(int*));
    for (i=0; i<n; i++) {
        fdp[i] = (int*)malloc(2*sizeof(int));
        pipe(fdp[i]);
    }
    pid_t parent = getpid();
    for (i=0; i<n-1; i++) if (fork()) break;
    if (getpid()==parent) { // only the parent executes this
        printf("I am the parent PID[%d] and I have %d children!\n", parent, n-1);
        r = rand()%n; if (!r) r++;
        write(fdp[r][WRITE], &parent, sizeof(parent));
        for (i=0; i<n; i++) close_fdp(fdp[i]);
        pause();
    } else {
        close(fdp[i][WRITE]); // children do not write on their pipe
        int pid, mypid = getpid();
        if (read(fdp[i][READ], &pid, sizeof(pid)) && pid==parent) { // i am the killer
            for (j=0, pid=i; j<n; j++)
                if (j!=i) write(fdp[j][WRITE], &pid, sizeof(pid));
            printf("I am PID[%d] and I am the killer!\n", mypid);
            int* a = (int*)malloc(n*sizeof(int)), size=0;
            a[size++] = parent;
            while (size < n-1)
                if (read(fdp[i][READ], &pid, sizeof(pid)) && pid)
                    a[size++] = pid;
            for (j=0; j<n; j++) close_fdp(fdp[j]); // ready to kill
            qsort(a, size, sizeof(int), cmp);
            char cmd[CMDSZ];
            snprintf(cmd, sizeof(cmd), "pstree -p %d", parent);
            system(cmd);
            for (j=0; j<size; j++) {
                kill(a[j], SIGTERM);
                printf("PID[%d] was killed by PID[%d]\n", a[j], mypid);
                waitpid(a[j], NULL, 0);
                //sleep(1);
                //snprintf(cmd, sizeof(cmd), "pstree -p %d", parent);
                //system(cmd);
            }
            free(a);
        } else { // i am a victim
            write(fdp[pid][WRITE], &mypid, sizeof(mypid));
            for (j=0; j<n; j++) close_fdp(fdp[j]);  
            pause(); // ready to die
        }
    }
    for (i=0; i<n; i++) free(fdp[i]);
    free(fdp);
    return 0;
}
