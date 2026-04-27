//Escribir un programa que implemente un sistema de productor-consumidor utilizando hilos
//Arreglo global (5 casillas): Se inicializa en 0s
//Hilo principal:
    //● Crea un hilo secundario.
    //● Después entra a un for para ir leyendo los valores que ingresa el usuario.
    //● Al terminar de leer los valores del usuario, espera a que termine el hilo secundario e imprime "Hemos terminado.".
//Hilo secundario:
    //● Verifica constantemente si algún valor del arreglo es distinto de 0 (i.e. está en un bucle infinito que recorre al arreglo una y otra vez para buscar si alguna casilla tiene algo distinto a 0).
    //● Si detecta un valor distinto a cero lo imprime, y pone 0 en esa posición del arreglo.
    //● Al encontrar un valor en la última casilla, lo imprime y termina su ejecución.

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

//arreglo global
int arreglo[5] = {0, 0, 0, 0, 0};

//hilo consumidor 
void *consumidor(void *arg) {
    int i;
    while (1) {
        for (i = 0; i < 5; i++) {
            if (arreglo[i] != 0) {
                printf("\nConsumidor ha consumido: %d\n", arreglo[i]);
                arreglo[i] = 0;
                //si el valor consumido es el de la última casilla, termina el hilo
                if (i == 4) {
                    pthread_exit(NULL);
                }
            }
        }
    }
}

int main () {
    pthread_t hilo_consumidor;
    int i, valor;

    //crea el hilo consumidor
    pthread_create(&hilo_consumidor, NULL, consumidor, NULL);

    //lee los 5 valores del usuario
    for (i = 0; i < 5; i++) {
        printf("Ingrese un valor %d: ", i);
        scanf("\n%d", &valor);
        arreglo[i] = valor;
    }

    //espera a que termine el hilo consumidor
    pthread_join(hilo_consumidor, NULL);
    printf("\nHemos terminado.\n");
    return 0;
}