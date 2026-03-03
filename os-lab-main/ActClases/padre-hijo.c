#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void print_pid(char name)
{
    printf("Soy el %c.\n", name);
    printf("Mi ID es %d.\n", getpid());
    printf("El ID de mi padre es %d.\n\n", getppid());
}

int main()
{
    pid_t pid_a, pid_b = -1;

    // Create the first son
    pid_a = fork();

    if(pid_a == 0)
    {
        print_pid('A');
        exit(0);
    }

    // Create the second son
    pid_b = fork();

    if(pid_b == 0)
    {
        print_pid('B');
        exit(0);
    }

    print_pid('Z'); // El proceso original se identifica como 'P'

    return 0;
}