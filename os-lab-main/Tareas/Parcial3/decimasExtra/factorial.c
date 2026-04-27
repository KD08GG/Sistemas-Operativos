//Programa que calcule el factorial de cualquier número en tres partes.
//Usa 3 variables globales.
//Hilo principal:
    //● Pide un número divisible entre 3.
    //● En cada una de las 3 variables globales, almacena un tercio del número. E.g. si el número es 9, almacena 3, 6, y 9
    //● Crea 3 hilos secundarios.
    //● Espera a que todos los hilos terminen.
    //● Imprime el resultado de multiplicar las 3 variables globales. (E.g. el resultado de 9! es 362,880).
//Hilos secundarios:
    //● Calculan el factorial de la parte que les tocó.
//Por ejemplo: Para calcular el factorial de 9
//El primer hilo calcula 1*2*3
//El segundo hilo calcula 4*5*6
//El tercer hilo calcula 7*8*9

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

//variables globales guardar resultados
int resultado1 = 1;
int resultado2 = 1;
int resultado3 = 1;

//variables globales guardar rango
int inicio1, fin1;
int inicio2, fin2;
int inicio3, fin3;

//hilo 1
void *hilo1 (void *arg) {
    int i;
    for (i = inicio1; i <= fin1; i++) {
        resultado1 *= i;
    }
    pthread_exit(NULL);
}

//hilo 2
void *hilo2 (void *arg) {
    int i;
    for (i = inicio2; i <= fin2; i++) {
        resultado2 *= i;
    }
    pthread_exit(NULL);
}

//hilo 3
void *hilo3 (void *arg) {
    int i;
    for (i = inicio3; i <= fin3; i++) {
        resultado3 *= i;
    }
    pthread_exit(NULL);
}

int main () {
    pthread_t thread1, thread2, thread3;
    int n;
    
    printf("Ingrese un número divisible entre 3: ");
    scanf("%d", &n);

    int tercio = n / 3; // 9/3 = 3
    //asigna rangos
    inicio1 = 1;
    fin1 = tercio; // = 3
    inicio2 = tercio + 1; // = 4
    fin2 = tercio * 2; // = 6
    inicio3 = tercio * 2 + 1; // = 7
    fin3 = n; // = 9

    //crear hilos
    pthread_create(&thread1, NULL, hilo1, NULL);    
    pthread_create(&thread2, NULL, hilo2, NULL);
    pthread_create(&thread3, NULL, hilo3, NULL);

    //esperar a que terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("El factorial de %d es: %d\n", n, resultado1 * resultado2 * resultado3);

    return 0;
}