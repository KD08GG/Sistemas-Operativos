/* Para lanzar un cohete al espacio se requiere activar dos llaves.
La segunda llave debe ser activada después de la primera. Usa
variables de condición para que un hilo active la primera llave
y, cuando esa llave esté activada, otro hilo active la segunda
llave. El main estará esperando a que los hilos terminen de
activar sus llaves para lanzar el cohete.
● Hay dos variables globales key_1 = 0 y key_2 = 0
● El primer hilo pone key_1 = 1 y manda la señal al otro hilo
● El segundo hilo implementa la variable de condición para esperar
que key_1 = 1y entonces pone key_2 = 1
● El main espera hasta que ambas llaves estén en 1, entonces lanza el
cohete */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int key_1 = 0;
int key_2 = 0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void *activate_key_1(void *arg) {
    sleep(1); // simula tiempo para activar llave 1
    pthread_mutex_lock(&mutex);
    key_1 = 1;
    printf("llave 1 activada\n");
    pthread_cond_signal(&cond); // señala al segundo hilo que la llave 1 está activada
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *activate_key_2(void *arg) {
    pthread_mutex_lock(&mutex);
    while (key_1 == 0) { // espera a que la llave 1 esté activada
        pthread_cond_wait(&cond, &mutex);
    }
    key_2 = 1;
    printf("llave 2 activada\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(){
    pthread_t thread1, thread2;

    // mutex y variable de condicion
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // crear hilos
    pthread_create(&thread1, NULL, activate_key_1, NULL);
    pthread_create(&thread2, NULL, activate_key_2, NULL);

    // espera 
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    if (key_1 == 1 && key_2 == 1) {
        printf("*se lanza cohete\n");
    }
    // destruir mutex y variabl condicion
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}