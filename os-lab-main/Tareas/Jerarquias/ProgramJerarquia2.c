#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void imprimir(char nombre) {
    printf("Proceso %c: Mi ID es %d, el ID de mi padre es %d\n", nombre, getpid(), getppid());
}

int main() {
    // R es el proceso principal
    
    // --- Rama de T ---
    if (fork() == 0) {
        // Estoy en T
        if (fork() == 0) {
            // Estoy en V (hijo de T)
            imprimir('V');
            exit(0);
        }
        sleep(1); // Espero a que V imprima
        imprimir('T');
        exit(0);
    }

    // --- Rama de W ---
    if (fork() == 0) {
        // Estoy en W
        if (fork() == 0) {
            // Estoy en S (hijo de W)
            imprimir('S');
            exit(0);
        }
        if (fork() == 0) {
            // Estoy en L (hijo de W)
            if (fork() == 0) {
                // Estoy en P (hijo de L)
                imprimir('P');
                exit(0);
            }
            sleep(1); // Espero a que P imprima
            imprimir('L');
            exit(0);
        }
        sleep(2); // Espero a que S, L y P impriman
        imprimir('W');
        exit(0);
    }

    // --- Proceso R (El padre original) ---
    sleep(4); // Se espera a que todas las ramas terminen
    imprimir('R');

    return 0;
}