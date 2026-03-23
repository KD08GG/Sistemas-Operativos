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

    // Father process
    if( pid_A != 0 ) // es padre
    {
        execl("/workspaces/codigos-c-docker/os-lab-main/ActClases/Parcial3/HolaMundo", "HolaMundo", NULL);
        perror("excecl ha fallado.");
    }
    return 0;
}