#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int active_connections = 0;
int max_connections    = 0;

sem_t semaforo; // max 3 conexiones al mismo tiempo

pthread_mutex_t mutex;

void *usar_base_de_datos(void *arg) {
    sem_wait(&semaforo);

    active_connections++;
    printf("Conexiones activas: %d\n", active_connections);
    sleep(1);
    active_connections--;

    sem_post(&semaforo);
    return NULL;
}

int main() {
    pthread_t hilos[10];
    int ids[10];

    sem_init(&semaforo, 0, 3); // inicializa semaforo en 3 
    pthread_mutex_init(&mutex, NULL);

    // crea 10 hilos
    for (int i = 0; i < 10; i++) {
        ids[i] = i + 1;
        pthread_create(&hilos[i], NULL, usar_base_de_datos, &ids[i]);
    }

    // espera a que todos terminen
    for (int i = 0; i < 10; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("\nMaximo de conexiones simultaneas: %d\n", max_connections);

    sem_destroy(&semaforo); // destruye semaforo
    pthread_mutex_destroy(&mutex); // destruye mutex

    return 0;
}