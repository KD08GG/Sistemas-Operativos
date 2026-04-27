#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t pidL= -1, pidD= -1;
    int status;

    pidL = fork();
    if (pidL == 0) {
        sleep(3);
        printf("termine mi ejecucion, mi pid es %d y el de mi padre es %d\n",getpid(), getppid());
        exit(0);
    }

    pidD = fork();
    if (pidD == 0) {
        sleep(1);
        printf("termine mi ejecucion, mi pid es %d y el de mi padre es %d\n",getpid(), getppid());
        exit(1);
    }

    wait(&status);
    printf("uno de mis hijos ha terminado su ejecucion %d\n", WEXITSTATUS(status));

    wait(&status);
    printf("uno de mis hijos ha terminado su ejecucion %d\n", WEXITSTATUS(status));

    return 0;
}