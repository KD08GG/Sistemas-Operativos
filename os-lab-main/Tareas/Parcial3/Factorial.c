#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int factorial (int num){
    int resultado = 1; 
    int i;

    for (i= 1; i <= num; i ++ ) { //inicio, condicion, incremento
        resultado = resultado * i;
    }
    return resultado;
}

int main () {

    int n = 5, m = 3; 
    pid_t pid_B = -1, pid_C = -1, pid_D = -1;
    int val_B, val_C, val_D;
    int status;

    pid_B = fork();
    if (pid_B == 0){
        exit (factorial (n-m));
    }

    pid_C = fork();
    if (pid_C == 0){
        exit (factorial (m));
    }

    pid_D = fork();
    if (pid_D == 0){
        exit (factorial (n));
    }

    waitpid(pid_B, &status, 0);
    val_B = WEXITSTATUS(status);
    printf("Hijo B retorna= %d\n", val_B);

    waitpid(pid_C, &status, 0);
    val_C = WEXITSTATUS(status);
    printf("Hijo C retorna= %d\n", val_C);


    waitpid(pid_D, &status, 0);
    val_D = WEXITSTATUS(status);
    printf("Hijo D retorna %d\n", val_D);

    printf("Resultado: %d\n", val_D / (val_C * val_B));  // n! / (m! * (n-m)!)

    return 0;
}