#include <stdio.h>
#include <pthread.h>

int account_a = 1000000;
int account_b = 1000000;

pthread_mutex_t mutex;

void *transferir(void *arg) {
    for (int i = 0; i < 1000; i++) {
        // un hiloi a la vez
        pthread_mutex_lock(&mutex);
        account_a -= 1;
        account_b += 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t hilos[1000];

    // inicializar mutex 
    pthread_mutex_init(&mutex, NULL);

    // crea 1000 hilos
    for (int i = 0; i < 1000; i++) {
        pthread_create(&hilos[i], NULL, transferir, NULL);
    }

    // espera a que termine
    for (int i = 0; i < 1000; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("account_a = %d\n", account_a);
    printf("account_b = %d\n", account_b);

    // destruye mutex 
    pthread_mutex_destroy(&mutex);

    return 0;
}