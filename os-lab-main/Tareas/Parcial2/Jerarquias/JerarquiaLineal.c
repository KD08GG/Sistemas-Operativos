#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    int i, n = 5;

    for (i = 1; i < n; i++) {
        pid = fork();
        if (pid != 0) { // Si es el padre, sale del ciclo
            break;
        }
    }
    // Cada proceso imprime quién es
    printf("Proceso PID: %d, Padre PPID: %d\n", getpid(), getppid());
    sleep(1); // Para que no se amontone la salida
    return 0;
}