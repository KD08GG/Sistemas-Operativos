//Programa que verifica si un número entero se encuentra en un arreglo de 12 casillas.
//Hilo principal:
    //● Divide el arreglo de entrada en 3 sub arreglos. E.g. {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12} se vuelve {1, 2, 3, 4} {5, 6, 7, 8} {9, 10, 11, 12}
    //● Crea 3 hilos secundarios.
    //● Espera a que los hilos terminen.
    //● Verifica la variable global found_flag para ver si algún hijo encontró el número, e imprime si el número fue encontrado o no.
//Cada hilo secundario:
    //● Toma uno de los 3 sub arreglos.
    //● Verifica si el número aparece.
    //● Si el número aparece modifica la variable global found_flag, le pone 1 y finaliza su ejecución.
//Usar una variable global para cada subarreglo.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <pthread.h>

//variable global
int found_flag = 0;

//subarreglo global
int sub1[4];
int sub2[4];
int sub3[4];

int numero;
int m;

//hilo 
void *busca1(void *arg)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (sub1[i] == numero)
        {
            found_flag = 1;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

//hilo 2
void *busca2(void *arg)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (sub2[i] == numero)
        {
            found_flag = 1;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

//hilo 3
void *busca3(void *arg)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (sub3[i] == numero)
        {
            found_flag = 1;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t t1, t2, t3;
    int arreglo[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int i;

    printf("Ingresa el numero a buscar: ");
    scanf("%d", &numero);

    //dividir arreglo en 3 sub
    for (i = 0; i < 4; i++) sub1[i] = arreglo[i];
    for (i = 0; i < 4; i++) sub2[i] = arreglo[i + 4];
    for (i = 0; i < 4; i++) sub3[i] = arreglo[i + 8];

    
    pthread_create(&t1, NULL, busca1, NULL);
    pthread_create(&t2, NULL, busca2, NULL);
    pthread_create(&t3, NULL, busca3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    //revisar si encontro el num 
    if (found_flag == 1)
        printf("El numero %d fue encontrado.\n", numero);
    else
        printf("El numero %d no fue encontrado.\n", numero);

    return 0;
}