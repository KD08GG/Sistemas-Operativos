#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
int main()
{
    pid_t pid_A, pid_B, pid_M;
    int status;
    pid_A = fork();
    // Child process
    if( pid_A == 0 )
    {
        sleep(10);
        return 10;
    }
    else
    {
        pid_B = fork();

        if( pid_B == 0 )
        {
            return 0;
        }
        else
        {
            pid_M = fork();
            if( pid_M == 0 )
            {
                sleep(3);
                return 3;
            }
        }
    }
    
    waitpid(pid_M, &status, 0); 
    printf("El proceso M ha terminado con el status: %d\n", WEXITSTATUS(status));

    waitpid(pid_A, &status, 0);
    printf("El proceso A ha terminado con el status: %d\n", WEXITSTATUS(status));

    waitpid(pid_B, &status, 0);
    printf("El proceso B ha terminado con el status: %d\n", WEXITSTATUS(status));
   
}