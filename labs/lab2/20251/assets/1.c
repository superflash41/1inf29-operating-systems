#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define N 8

int main(void) {
    pid_t pid_padre = getpid();
    pid_t pids[N]; // Almacenar los PIDs de los hijos
    pid_t child;
    int i, status;

    for (i = 0; i < N; ++i) {
        child = fork();
        if (child < 0) {
            perror("fork");
            exit(1);
        }

        if (child == 0) {
            // Proceso hijo
            if (i == 0) {
                // Primer hijo
                // Esperamos un poco para asegurarnos que los demás hijos se creen
                sleep(1);

                // Eliminamos a los demás hijos
                for (int j = 1; j < N; ++j) {
                    kill(pids[j], SIGKILL);
                }

                // Eliminamos al padre
                kill(pid_padre, SIGKILL);

                printf("Primer hijo (%d): Eliminé a mis hermanos y al padre.\n", getpid());
            }

            // Todos los hijos esperan indefinidamente
            pause();
            exit(0);
        } else {
            // Proceso padre guarda el PID del hijo
            pids[i] = child;
        }
    }

    // Solo el padre hace wait si no ha sido eliminado
    if (getpid() == pid_padre) {
        for (i = 0; i < N; ++i) {
            wait(&status);
        }
        printf("Padre ha terminado.\n");
    }

    return 0;
}


