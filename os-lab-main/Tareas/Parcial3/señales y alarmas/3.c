//M espera una alarma que escribe "Alarma recibida" despues de 3 segundos. Despues envia la señal SIGUSR2 a A. Espera a que hijos terminen e imprime "Mis hijos han terminado"
//A espera la señal SIGUSR1 y luego envia la señal SIGUSR2 a C. Espera a que C termine. 
//C espera a recibir una señal. 
//Debe haber 2 manejadores: SIGUSR1 y SIGUSR2 escriben "Señal recibida" SU ID, y el ID del su padre. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void sigusr_handler1(int sig) {
    printf("Recibí la señal SIGUSR1: %d, ID del proceso: %d, ID del padre: %d\n", sig, getpid(), getppid());
}
void sigusr_handler2(int sig) {
    printf("Recibí la señal SIGUSR2: %d, ID del proceso: %d, ID del padre: %d\n", sig, getpid(), getppid());
}
void alarm_handler(int sig) {
    printf("Alarma recibida\n");
}

int main() {
    pid_t pid_A=-1, pid_C=-1;
    int status;

    pid_A = fork();
    if (pid_A == 0) { // hijo A

        pid_C = fork();
        if (pid_C == 0) { // hijo C
            signal(SIGUSR2, sigusr_handler2);
            pause(); // espera recibir una senal
            exit(0);
        }

        signal(SIGUSR1, sigusr_handler1);

        pause(); // espera señal 1
        kill(pid_C, SIGUSR2); // envía señal 2 a C
        exit(0);
    }

    // padre M
    signal(SIGALRM, alarm_handler);
    alarm(3); // alarma de 3 segundos
    pause(); 

    kill(pid_A, SIGUSR1); // envia SIGUSR1 a A

    waitpid(pid_A, &status, 0); // espera que A termine
    waitpid(pid_C, &status, 0); // espera quw C termine

    printf("Mis hijos han terminado\n");
    return 0;
}

