// R imprime su ID de manera infinita. 
// Cuando se ingresa CTRL+C. SIGNINT imprime "El usuario ha presionado CTRL+C" 
// Se pausa la ejecucion 3 segundos. 
// R continua imprimiendo su ID de manera infinita.

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigint_handler(int sig) {
    printf("\nEl usuario ha presionado CTRL+C\n");
    sleep(3);
}

int main() {
    signal(SIGINT, sigint_handler);
    while(1) {
        printf("ID de proceso: %d\n", getpid());
        sleep(1);
    }
    return 0;
}

