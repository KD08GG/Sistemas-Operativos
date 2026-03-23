#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() 
{
    pid_t pid = -1;
    int x = 5, y = 10;
    //int x = x + 10, y = y + 6;

    pid = fork(); 

    // Código del hijo
    if(pid == 0)
    {
        //printf("Soy el hijo y mi pid = %d\n", getpid());
        printf("Soy el hijo (PID: %d). Valores: x=%d, y=%d\n", getpid(), x, y);
        exit(0);
    }

    // Código del padre (el hijo terminó en exit)
    //printf("Soy el padre y mi pid = %d\n", getpid());
    printf("Soy el padre (PID: %d). Valores: x=%d, y=%d\n", getpid(), x, y);

    return 0;
}