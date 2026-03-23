#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Hijo N: escribe 10 enteros pares a N.txt
void proceso_N() {
    FILE *f = fopen("N.txt", "w");
    int i;
    for (i = 2; i <= 20; i += 2)
        fprintf(f, "%d\n", i);
    fclose(f);
    exit(0);
}

// Hijo M: escribe 10 enteros impares a M.txt
void proceso_M() {
    FILE *f = fopen("M.txt", "w");
    int i;
    for (i = 1; i <= 19; i += 2)
        fprintf(f, "%d\n", i);
    fclose(f);
    exit(0);
}

// Padre R: espera a sus hijos, lee los archivos y suma elemento por elemento
int main() {
    pid_t pid_N = -1, pid_M = -1;
    int status;

    pid_N = fork();
    if (pid_N == 0)
        proceso_N();

    pid_M = fork();
    if (pid_M == 0)
        proceso_M();

    // Padre espera a ambos hijos
    wait(&status);
    wait(&status);

    // Leer archivos y sumar
    FILE *fn = fopen("N.txt", "r");
    FILE *fm = fopen("M.txt", "r");

    int par, impar;
    while (fscanf(fm, "%d", &impar) == 1 && fscanf(fn, "%d", &par) == 1)
        printf("%d + %d = %d\n", impar, par, impar + par);

    fclose(fn);
    fclose(fm);

    return 0;
}