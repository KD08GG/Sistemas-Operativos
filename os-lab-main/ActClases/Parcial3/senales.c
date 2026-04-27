#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void signal_handler_sigusr2(int signum) 
{
    int i;
    for (i = 0; i <= 10; i++)
    {
        printf("Señal SIGUSR2 recibida.: %d\n", i);
    }
}

int main() 
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) 
    {
        // Proceso hijo
        signal(SIGUSR2, signal_handler_sigusr2);
        printf("M: Proceso hijo en espera de señales.\n");
        pause();
        return 0;
    } 
    
    // Proceso padre
    sleep(1);
    printf("Q: Proceso padre enviando señal al proceso hijo.\n");
    kill(pid, SIGUSR2);
    wait(&status);

    return 0;
}