#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
int main()
{
    pid_t pid_A;
    int status;
    pid_A = fork();

    // Child process
    if( pid_A == 0 ) // es hijo
    {
        execl("/workspaces/codigos-c-docker/os-lab-main/ActClases/Parcial3/HelloWorld", "HelloWorld", NULL);
        perror("excecl ha fallado.");
        return 0;
    }
    else
    {
        wait(&status);
        if( WEXITSTATUS(status) == 0 ) // es hijo 
        {
            printf("Mi hijo se ha ejecutado exitosamente.\n");
        }
    }
    return 0;
}