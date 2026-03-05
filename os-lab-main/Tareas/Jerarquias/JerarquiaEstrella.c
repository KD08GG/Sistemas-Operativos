#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    int i, n = 5;

    for (i = 1; i < n; i++) {
        pid = fork();
        if (pid == 0) { // Si es el hijo, sale del ciclo
            break;
        }
    }
    printf("Proceso PID: %d, Padre PPID: %d\n", getpid(), getppid());
    sleep(1);
    return 0;
}