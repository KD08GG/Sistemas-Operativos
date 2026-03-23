#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h> // Para exit() 
#include <errno.h> // Para manejo errores 

//Funcion para verificar errores
void verificarError(int resultado) {
    if (resultado < 0) { // Regresa -1 error
        if (errno == EAGAIN) {
            printf("Error: Archivo bloqueado.\n"); 
        } else if (errno == EACCES) {
            printf("Error: Problema de permisos para acceder al archivo.\n");
        } else if (errno == EBADF) {
            printf("Error: Mal descriptor de archivo.\n"); 
            // Mensaje usando perror
            perror("Error detectado"); 
        }
        exit(1); 
    }
}


//Arreglo de nùmeros flotantes 
int main() {
    printf("\nArreglo de nùmeros flotantes \n");
    float arreglo[] = {10.0, 20.0, 30.0, 40.0};
    int fd, n, m;

    // Crea archivo y escribe arreglo completo 
    fd = creat("datos.txt", 0777); 
    verificarError(fd);
    n = write(fd, arreglo, sizeof(arreglo)); 
    verificarError(n);
    close(fd); 

    // Leer arreglo y mostrar
    float buffer_lectura[4];
    fd = open("datos.txt", 0); 
    verificarError(fd);
    m = read(fd, buffer_lectura, sizeof(buffer_lectura)); 
    verificarError(m);
    
    for(int i = 0; i < 4; i++) {
        printf("Leido datos.txt: %.2f\n", buffer_lectura[i]);
    }
    close(fd);


    //Arreglo con 5 números flotantes uno por uno
    printf("\nArreglo con 5 números flotantes uno por uno\n");
    float valores[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    fd = creat("datos2.txt", 0777); 
    verificarError(fd);

    for(int i = 0; i < 5; i++) {
        n = write(fd, &valores[i], sizeof(float)); // Escribir uno por uno
        printf("Escritura %d: n = %d\n", i+1, n);
        verificarError(n);
    }
    close(fd);

    //Ejercicio Opcional - 0 a la izquierda
    printf("\nEjercicio Opcional\n");
    int entrada[] = {1, 2, 0, 4, 9, 0, 3, 5, 1, 3};
    int salida[10];
    int pos = 0;
    for(int i = 0; i < 10; i++) if(entrada[i] == 0) salida[pos++] = 0;
    for(int i = 0; i < 10; i++) if(entrada[i] != 0) salida[pos++] = entrada[i];
    
    printf("Resultado ceros a la izquierda: ");
    for(int i = 0; i < 10; i++) printf("%d ", salida[i]);
    printf("\n");

    return 0;
}