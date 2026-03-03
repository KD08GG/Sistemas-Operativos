#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    // Proceso hijo M
    pid = fork();

    if (pid < 0) {
        // Error al crear
        perror("Error en fork");
        return 1;
    }

    if (pid == 0) {
        // HIJO (M) 
        printf("Hijo (M): Escribir en datos.txt\n");
        
        FILE *archivo = fopen("datos.txt", "w");
        if (archivo == NULL) {
            perror("Error al abrir archivo en hijo");
            exit(1);
        }

        // Escribe 5 enteros en el archivo
        for (int i = 1; i <= 5; i++) {
            fprintf(archivo, "%d\n", i * 10); 
        }

        fclose(archivo);
        printf("Hijo (M): Datos guardados.\n");
        exit(0);
    } 
    else {


        // PADRE (Q)
        // Sleep(3) para esperar a que el hijo escriba - termine
        printf("Padre (Q): Espera 3 segundos...\n");
        sleep(3);

        printf("Padre (Q): Lee datos del archivo\n");
        FILE *archivo = fopen("datos.txt", "r");
        if (archivo == NULL) {
            perror("Error al abrir archivo");
            return 1;
        }
        // Datos leidos
        int numero;
        while (fscanf(archivo, "%d", &numero) != EOF) {
            printf("Valor: %d\n", numero);
        }

        fclose(archivo);
        printf("Padre (Q): Proceso completado.\n");
    }

    return 0;
}