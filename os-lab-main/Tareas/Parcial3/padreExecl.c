#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid_B = -1;
    int status;

    pid_B = fork();
    if (pid_B == 0) //hijo
    {
        execl("./sumaExecl", "suma", "10", "20", "30", NULL); //ejecuta suma - numeros como string
        perror("execl ha fallado.");
        exit(0);
    }

    waitpid(pid_B, &status, 0); //padre espera y retorma B
    printf("Hjo regreso: %d\n", WEXITSTATUS(status));

    return 0;
}