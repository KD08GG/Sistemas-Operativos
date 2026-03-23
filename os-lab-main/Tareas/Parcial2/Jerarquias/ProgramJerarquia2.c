#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void imprimir(char nombre) {
    printf("Proceso %c: Mi ID es %d, el de mi padre es %d\n", nombre, getpid(), getppid());
}

int main() {
    // Definimos las variables como en tus clases
    pid_t pid_T, pid_W, pid_V, pid_S, pid_L, pid_P;

    // --- RAMA IZQUIERDA ---
    pid_T = fork(); // Guardamos el resultado en pid_T
    if (pid_T == 0) {
        pid_V = fork(); // T guarda el PID de su hijo V
        if (pid_V == 0) {
            imprimir('V');
            exit(0);
        }
        sleep(1); 
        imprimir('T');
        exit(0);
    }

    // --- RAMA DERECHA ---
    pid_W = fork(); // Guardamos el resultado en pid_W
    if (pid_W == 0) {
        // Hijo S
        pid_S = fork();
        if (pid_S == 0) {
            imprimir('S');
            exit(0);
        }

        // Hijo L
        pid_L = fork();
        if (pid_L == 0) {
            pid_P = fork();
            if (pid_P == 0) {
                imprimir('P');
                exit(0);
            }
            sleep(1);
            imprimir('L');
            exit(0);
        }

        sleep(2);
        imprimir('W');
        exit(0);
    }

    // El proceso R espera a que todos terminen
    sleep(4); 
    imprimir('R');

    return 0;
}