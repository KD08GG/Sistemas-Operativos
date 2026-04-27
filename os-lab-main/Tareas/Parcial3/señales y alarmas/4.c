//A: manda una alarma de 5 segundos y espera a que B termine
//B: Duerme un segundo,  mata al proceso C usando SIGKILL, y espera a recibir una señal.
//C: Imprime "Esperando una señal" de manera indefinida
//SIGALRM iprime Alarma recibida, e inmediatamente usa la llamada al sistema exit (1). 
// secuencia A - B - C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void alarm_handler(int sig) {
    printf("Alarma recibida\n");
    exit(1);
}

int main() {
    pid_t pid_B, pid_C;
    int status;

    pid_B = fork();
    if (pid_B == 0) { // hijo B

        pid_C = fork();
        if (pid_C == 0) { // hijo C
            while(1) {
                printf("Esperando una señal\n");
                sleep(1);
            }
            exit(0);
        }


        sleep(1); // duerme un segundo
        kill(pid_C, SIGKILL); // mata al proceso C
        waitpid(pid_C, &status, 0); 
        pause(); // espera a recibir una señal
        exit(0);
    }

    // padre A
    signal(SIGALRM, alarm_handler);
    alarm(5); // lanza alarma de 5 segundos

    waitpid(pid_B, &status, 0); // espera a que B termine

    return 0;
}