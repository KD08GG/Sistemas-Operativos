#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pidA, pidB, pidC;
    int status;

    pidA = fork();
    if (pidA == 0) {
        sleep(3);
        printf("Hijo A terminando\n");
        exit(1);
    }

    pidB = fork();
    if (pidB == 0) {
        sleep(1);
        printf("Hijo B terminando\n");
        exit(2);
    }

    pidC = fork();
    if (pidC == 0) {
        sleep(5);
        printf("Hijo C terminando\n");
        exit(3);
    }

    // Padre espera en orden: B, A, C
    waitpid(pidB, &status, 0);
    printf("Padre: B termino con status %d\n", WEXITSTATUS(status));

    waitpid(pidA, &status, 0);
    printf("Padre: A termino con status %d\n", WEXITSTATUS(status));

    waitpid(pidC, &status, 0);
    printf("Padre: C termino con status %d\n", WEXITSTATUS(status));

    return 0;
}