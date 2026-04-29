/* Escribe un programa que utilice concurrencia para imprimir
números pares e impares en orden
○ La variable global counter incrementará cuando se imprima en pantalla
○ Hilo 1: imprime números impares
○ Hilo 2: imprime números pares y, si counter es mayor que 100, mata al
proceso
○ Hilo principal: crea a hilo1 e hilo 2, y espera a que terminen
○ Usa variables de condición para alternar los números
(recomendación: crea una variable even_turn que cambie de 0 a 1
dependiendo de qué hilo hizo la impresión)
● Ejemplo de ejecución:
○ Hilo impares: 1
○ Hilo pares: 2
○ Hilo impares: 3
○ …
○ Hilo pares: 100 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int counter = 0; // variable global incrementa
int even_turn = 0;

pthread_mutex_t mutex;
pthread_cond_t cond; 

// Hilo 1 numeros impares 
void* hilo_impares(void* arg)
{
    while( 1 )
    {
        pthread_mutex_lock( &mutex );
        while( even_turn == 1 )
        {
            pthread_cond_wait( &cond, &mutex );
        }
        if( counter > 100 )
        {
            pthread_mutex_unlock( &mutex );
            break;
        }
        counter++;
        printf("Hilo impares: %d\n", counter);
        // pasa turno a hilo par
        even_turn = 1;
        pthread_cond_signal( &cond );

        pthread_mutex_unlock( &mutex );
    }

    pthread_exit( NULL );
}

// Hilo 2 numeros pares
void* hilo_pares(void* arg)
{
    while( 1 )
    {
        pthread_mutex_lock( &mutex );
        while( even_turn == 0 )
        {
            pthread_cond_wait( &cond, &mutex );
        }
        counter++;
        printf("Hilo pares: %d\n", counter);

        if( counter > 100 ) // counter > 100 - mata proceso
        {
            pthread_mutex_unlock( &mutex );
            exit(0);
        }
        even_turn = 0;
        pthread_cond_signal( &cond );

        pthread_mutex_unlock( &mutex );
    }
    pthread_exit( NULL );
}

int main()
{
    pthread_t hilo_1, hilo_2;
    pthread_mutex_init( &mutex, 0 );
    pthread_cond_init( &cond, 0 );

    pthread_create( &hilo_1, NULL, hilo_impares, NULL );
    pthread_create( &hilo_2, NULL, hilo_pares, NULL );

    pthread_join( hilo_1, NULL );
    pthread_join( hilo_2, NULL );

    pthread_mutex_destroy( &mutex );
    pthread_cond_destroy( &cond );

    return 0;
}