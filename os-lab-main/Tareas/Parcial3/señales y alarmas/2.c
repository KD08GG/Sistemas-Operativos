//Lanza alarma dentro de 3 segundos y espera una señal.
//La alarma tiene un manejador para escribir "Transcurrieron 3 segundos"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void alarm_handler(int sig) { //manejador de la alarma
    printf("Transcurrieron 3 segundos\n");
}

int main() { //configura el manejador y lanza la alarma
    signal(SIGALRM, alarm_handler);
    alarm(3);
    pause(); //espera a que ocurra la alarma
    return 0;
}

