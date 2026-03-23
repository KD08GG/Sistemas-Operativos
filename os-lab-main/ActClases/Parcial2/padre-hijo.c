#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid_M;

    // Se crea el proceso hijo M
    pid_M = fork();

    if (pid_M == 0) {
        // --- HIJO M ---
        // Abre el archivo datos.txt para escribir (w) 
        FILE *archivo = fopen("datos.txt", "w");
        
        // Escribe 5 enteros en el archivo 
        for (int i = 1; i <= 5; i++) {
            fprintf(archivo, "%d\n", i);
        }
        
        fclose(archivo);
        // El hijo termina aquí para no ejecutar código del padre
        exit(0); 
    } 
    else if (pid_M > 0) {
        // --- PADRE Q ---
        // El padre espera 3 segundos como dice la nota 
        printf("Padre Q: Esperando 3 segundos...\n");
        sleep(3);

        // Abre el archivo para leer (r) 
        FILE *archivo = fopen("datos.txt", "r");
        int numero;

        printf("Datos leidos por el Padre Q:\n");
        // Lee del archivo e imprime en pantalla 
        while (fscanf(archivo, "%d", &numero) != EOF) {
            printf("%d\n", numero);
        }

        fclose(archivo);
    }

    return 0;
}