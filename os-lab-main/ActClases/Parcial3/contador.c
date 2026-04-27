#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int counter;

void *increase_counter( void *args )
{
    while(1)
    {
        counter++;
        sleep(1);
    }
    pthread_exit( NULL );
}

int main()
{
    pthread_t thread_1; // ID del hilo
    int state; // Valor de retorno de la creación del hilo
    counter = 0;    // La cuenta empieza en 0
    
    state = pthread_create( &thread_1, NULL, &increase_counter, NULL ); // Crear al hilo, call_return debe dar 0 o hay error

    printf("Contando. Presiona cualquier tecla para ver la cuenta.\n");
    getchar();
    printf("El contador llegó a: %d.\n", counter);
    
    exit(0); // Terminar el proceso también mata a todos sus hilos secundarios
}