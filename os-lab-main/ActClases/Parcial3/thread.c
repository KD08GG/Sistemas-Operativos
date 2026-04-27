#include <stdio.h>
#include <pthread.h>

void* my_thread_function(void* arg) {
    printf("Hola desde el hilo!\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, my_thread_function, NULL);
    pthread_join(thread_id, NULL);
    printf("Hilo ha terminado.\n");
    return 0;
}