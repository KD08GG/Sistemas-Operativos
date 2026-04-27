#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NO 10

void *par( void *args )
{
    
        printf( "Es un numero par.\n" );
        pthread_exit( NULL );
}

void *impar( void *args )
{
    printf( "Es un numero impar.\n" );
    pthread_exit( NULL );
}

int main()
{
    pthread_t threads[THREADS_NO];
    int state, i;
    
    for(i=0; i < THREADS_NO; i++) 
    {
        if (i % 2 == 0) {
            pthread_create( &threads[i], NULL, par, NULL );
        } else {
            pthread_create( &threads[i], NULL, impar, NULL );
        }
        
        printf( "Main creo un hilo en la iteración %d.\n", i );
    }
    
    for (i=0; i < THREADS_NO; i++)
    {
        pthread_join( threads[i], NULL );
    }
    exit(0);
}