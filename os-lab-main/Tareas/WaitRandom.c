#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int numero_aleatorio() { //nuumero aleatorio entre 1 y 100
    return (rand() % 100) + 1;
}

int main() {
    pid_t pid_A = -1, pid_B = -1, pid_M = -1;
    int status;
    int correctos = 0;

    srand(time(NULL));   // Semilla rand(), antes de los forks

    pid_A = fork(); //hijo A
    if (pid_A == 0) {
        printf("Soy A, mi pid = %d, numero = %d\n", getpid(), numero_aleatorio());
        exit(0);
    }

    pid_B = fork(); //hijo b
    if (pid_B == 0) {
        printf("Soy B, mi pid = %d, numero = %d\n", getpid(), numero_aleatorio());
        exit(0);
    }

    pid_M = fork(); // hijo m 
    if (pid_M == 0) {
        printf("Soy M, mi pid = %d, numero = %d\n", getpid(), numero_aleatorio());
        exit(0);
    }

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        correctos++;

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        correctos++;

    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        correctos++;

    printf("Hijos terminados: %d\n", correctos);

    return 0;
}