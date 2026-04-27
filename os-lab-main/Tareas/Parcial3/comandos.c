#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid_A = -1, pid_B = -1, pid_C = -1, pid_D = -1; ;
    int status;

    pid_A = fork(); // hijo A
    if (pid_A == 0)
    {
        printf("A: mi pid es %d y el pid de mi padre es %d\n", getpid(), getppid());

        pid_D = fork(); //subhijo D
        if (pid_D == 0) // D ejecuta a ls
        {
            printf("D, mi pid es %d y el pid de mi padre es %d\n", getpid(), getppid());

            execl("/bin/ls", "ls", NULL); //se ejecuta comand ls
            perror("execl falló");
            exit(0);
        }

    
        waitpid(pid_D, &status, 0); // A espera a D y borra el folder
        execl("/bin/rm", "rm", "new_folder", NULL);
        perror("execl falló");
        exit(0);
    }

    pid_B = fork();
    if (pid_B == 0) // B ejecuta mkdir - new folder
    {
        printf("B: mi pid es %d y el pid de mi padre es %d\n", getpid(), getppid());
        execl("/bin/mkdir", "mkdir", "new_folder", NULL);
        perror("execl falló");
        exit(0);
    }

    pid_C = fork(); // C ejecuta ls
    if (pid_C == 0)
    {
        printf("C: mi pid es %dy  el pid de mi padre es %d\n", getpid(), getppid());
        execl("/bin/ls", "ls", NULL);
        perror("execl falló");
        exit(0);
    }

    waitpid(pid_A, &status, 0); //M espera a A
    execl("/bin/ls", "ls", NULL);

    waitpid(pid_C, &status, 0); //M ejecuta a C
    execl("/bin/mkdir", "mkdir", "new_folder", NULL);

    waitpid(pid_B, &status, 0); //M ejecuta a B 
    printf("Soy M, mi pid = %d\n", getpid()); //imprimer soy M y su pit...
    execl("/bin/ls", "ls", NULL); //ejecuta ls

    return 0;
}
