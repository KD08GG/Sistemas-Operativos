#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Crea primer hijo (N)
    if (fork() == 0) {
        FILE *archivoN = fopen("N.txt", "w");
        for (int i = 1; i <= 10; i++) {
            fprintf(archivoN, "%d\n", i * 2); // Escribe 2, 4, 6 -
        }
        fclose(archivoN);
        exit(0); // El hijo N termina
    }

    // Crea segundo hijo (M)
    if (fork() == 0) {
        FILE *archivoM = fopen("M.txt", "w");
        for (int i = 0; i < 10; i++) {
            fprintf(archivoM, "%d\n", (i * 2) + 1); // Escribe 1, 3, 5
        }
        fclose(archivoM);
        exit(0); // El hijo M termina
    }

    // --- EL PADRE (R) ---
    // Sleep para dar 3 segundos a los hijos y asegurar que los archivos ya existan.
    sleep(3); 

    FILE *fN = fopen("N.txt", "r");
    FILE *fM = fopen("M.txt", "r");
    int numN, numM;

    printf("Ejecución del programa:\n");    
    for (int i = 0; i < 10; i++) {
        fscanf(fN, "%d", &numN);
        fscanf(fM, "%d", &numM);
        printf("%d + %d = %d\n", numM, numN, numM + numN);
    }

    fclose(fN);
    fclose(fM);

    return 0;
}