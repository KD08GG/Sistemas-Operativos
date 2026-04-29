#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NO_ITEMS 10
int buffer = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_consumer, cond_producer;

void* producer(void* arg)
{
    for ( int i=0; i<NO_ITEMS; i++ ) 
    {
        pthread_mutex_lock( &mutex );
        while( buffer != 0 ) // Esperar hasta que el buffer esté libre
        {
            // Libera el mutex y espera a que se cumpla la condición (que llegue la señal)
            pthread_cond_wait( &cond_producer, &mutex );
        }
        buffer = 1;
        printf("Productor: escribí un dato en el buffer.\n");
        
        // Notifica que hay elementos en el buffer
        pthread_cond_signal( &cond_consumer );
        pthread_mutex_unlock( &mutex );
    }

    pthread_exit( NULL );
}

void* consumer(void* arg)
{
    for ( int i=0; i<NO_ITEMS; i++ ) 
    {
        pthread_mutex_lock( &mutex );
        // ---------- Región crítica
        while (buffer == 0) // Esperar hasta que haya información en el buffer
        {
            // Libera el mutex y espera a que se cumpla la condición (que llegue la señal)
            pthread_cond_wait( &cond_consumer, &mutex );
        }
        printf("Consumidor: leí un dato del buffer.\n");
        buffer = 0;
        
        // Avisa que el buffer está limpio.
        pthread_cond_signal( &cond_producer ); 
        pthread_mutex_unlock( &mutex );
        
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t thread_1, thread_2;
    
    pthread_mutex_init( &mutex, 0 );
    pthread_cond_init( &cond_consumer, 0 );
    pthread_cond_init( &cond_producer, 0 );
    
    // Creamos los hilos
    pthread_create( &thread_1, NULL, producer, NULL );
    pthread_create( &thread_2, NULL, consumer, NULL );
    
    pthread_join( thread_1, NULL);
    pthread_join( thread_2, NULL);

    pthread_mutex_destroy( &mutex );
    pthread_cond_destroy( &cond_consumer );
    pthread_cond_destroy( &cond_producer );
    
    return 0;
}