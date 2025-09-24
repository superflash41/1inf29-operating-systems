#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

// sn

#define N 5 // Número de procesos hijos a crear

// Variable global para almacenar los PIDs de los hijos
pid_t pids[N];
// Bandera para indicar al hijo si ha sido seleccionado
volatile sig_atomic_t selected_to_kill = 0;

// Manejador de señales para el hijo elegido
void sig_handler(int signum) {
    if (signum == SIGUSR1) {
        selected_to_kill = 1;
    }
}

int main() {
    int i;
    srand(time(NULL));

    // Configuración del manejador de señales para todos los procesos antes de fork
    struct sigaction sa;
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    // Bucle para crear N procesos hijos
    for (i = 0; i < N; i++) {
        pids[i] = fork();

        if (pids[i] == -1) {
            perror("fork");
            return 1;
        }

        if (pids[i] == 0) { // Código del proceso hijo
            printf("Proceso hijo creado con PID: %d\n", getpid());
            
            // Los hijos esperan la señal para ser seleccionados
            while(!selected_to_kill) {
                pause(); // Pausa la ejecución hasta que se reciba una señal
            }
            
            printf("Proceso %d ha sido elegido para eliminar a los demás.\n", getpid());
            
            // Proceso "asesino" elimina a los demás hijos
            for(int j = 0; j < N; j++) {
                if (pids[j] != getpid()) {
                    printf("  -> Proceso %d eliminando a proceso %d\n", getpid(), pids[j]);
                    kill(pids[j], SIGKILL); // Envía la señal de terminación forzada
                }
            }
            
            printf("Proceso %d ha terminado su tarea y se autodestruye.\n", getpid());
            exit(0);
        }
    }

    // Código del proceso padre
    printf("Proceso padre con PID: %d\n", getpid());
    
    // El padre elige un hijo al azar para que sea el asesino
    int killer_index = rand() % N;
    pid_t killer_pid = pids[killer_index];
    
    printf("El padre ha elegido al proceso con PID: %d para ser el 'asesino'.\n", killer_pid);
    
    // El padre envía una señal SIGUSR1 al proceso hijo elegido
    kill(killer_pid, SIGUSR1);
    
    // El padre espera a que todos sus hijos terminen
    for (i = 0; i < N; i++) {
        wait(NULL);
    }
    
    printf("Todos los procesos hijos han terminado. El padre finaliza.\n");
    return 0;
}
