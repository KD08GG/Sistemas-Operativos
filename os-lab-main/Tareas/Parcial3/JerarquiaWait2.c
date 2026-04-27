#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pidB= -1, pidC= -1, pidE = -1;
    int status;

    pidB = fork();
    if (pidB == 0) {
        sleep(3);
        printf("termina ejecucion B, mi pid es %d y el de mi padre es %d\n",getpid(), getppid());
        exit(1);
    }

    pidC = fork();
    if (pidC == 0) {
        sleep(1);
        printf("termina ejecucion C, mi pid es %d y el de mi padre es %d\n",getpid(), getppid());
        exit(2);
    }

    pidE = fork();
    if (pidE == 0) {
        //sleep(0);
        printf("termina ejecucion E, mi pid es %d y el de mi padre es %d\n",getpid(), getppid());
        exit(3);
    }

    waitpid(pidB, &status, 0);
    printf("Hijo B terminó con valor: %d\n", WEXITSTATUS(status));

    waitpid(pidC, &status, 0);
    printf("Hijo C terminó con valor: %d\n", WEXITSTATUS(status));

    waitpid(pidE, &status, 0);
    printf("Hijo E terminó con valor: %d\n", WEXITSTATUS(status));

    return 0;
}