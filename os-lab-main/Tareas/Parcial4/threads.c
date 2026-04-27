#include <stdio.h>
#include <pthread.h>

int counter = 0;
#define N_THREADS 2
// Declare the semaphore as a global variable so that the threads can use it.


void* increase_counter(void* arg) 
{
    int i;
    for ( int i=0; i<1000; i++) 
    {
        counter = counter + 1;
    }
    pthread_exit( NULL );
}

int main() 
{
    pthread_t thread_ids[N_THREADS];
    
    // Create the semaphore.
    
    // Create children.
    for( int i=0; i<N_THREADS; i++)
        pthread_create( &thread_ids[i], NULL, increase_counter, NULL );
    
    // Wait for the children to finish.
    for( int i=0; i<N_THREADS; i++)
        pthread_join( thread_ids[i], NULL );
    
    // Destroy the semaphore.

    printf("El valor final del contador es %d\n", counter);
    return 0;
}