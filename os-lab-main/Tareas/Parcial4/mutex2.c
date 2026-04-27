#include <stdio.h>
#include <pthread.h>

// recursos que compartenn
// current_i: entero que empieza en 0
// buffer_index: entero que empieza en 0
// buffer: arreglo entero de tamaño 1000

int current_i = 0;
int buffer_index = 0;
int buffer[1000];

pthread_mutex_t mutex; // proteje region critica 

void *llenar_buffer(void *arg) {
    // lee y escribe current_i y buffer_index
    pthread_mutex_lock(&mutex);
    buffer[buffer_index] = current_i;
    buffer_index += 1;
    current_i += 1;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t hilos[1000];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 1000; i++) {
        pthread_create(&hilos[i], NULL, llenar_buffer, NULL);
    }

    for (int i = 0; i < 1000; i++) {
        pthread_join(hilos[i], NULL);
    }

    // imprimir buffer donde el valor es 0
    printf("casillas buffer donde el valor es 0:\n");
    for (int i = 0; i < 1000; i++) {
        if (buffer[i] == 0) {
            printf("buffer[%d] = %d\n", i, buffer[i]);
        }
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}