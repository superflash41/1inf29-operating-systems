#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
int main(void) {
    int i,child,status;

 
    child = fork();
    if (child < 0) {
            perror("fork");
            exit(1);
    }

    if (child == 0) {
            // Proceso hijo 1
            srand(time(NULL));
            int x, mypid=getpid();
            for(x=0;x<10;x++) {
                printf("%d %d\n",mypid,rand() % 501);
            }
            exit(0);  
    }
    
    child = fork();
    if (child < 0) {
            perror("fork");
            exit(1);
    }
    if (child == 0) {
            // Proceso hijo 2
            srand(time(NULL)+10);
            int x, mypid=getpid();
            for(x=0;x<10;x++) {
                printf("%d %d\n",mypid,rand() % 501);
            }
            exit(0);
    }
    // Solo el padre hace wait si no ha sido eliminado
    for (i = 0; i < 2; ++i) {
            wait(&status);
    }
    return 0;
}


