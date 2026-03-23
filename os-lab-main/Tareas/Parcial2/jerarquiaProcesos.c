#include <stdio.h> //printf
#include <stdlib.h> //exit 
#include <unistd.h> //fork y sleep
#include <sys/types.h> //tipo de dato pid_t = num especial para identificar procesos

int main() {
    pid_t pid; //variable llamada pid: se guarda numero que dira si somos padre o hijo despues de  la clonacion

    // Programa clonado: padre e hijo 
    pid = fork();

    if (pid < 0) { // si pid es negativo 
        // Error al crear
        perror("Error en fork");
        return 1;
    }

    if (pid == 0) { //si pid es 0, entra el hijo M (Solo lo ejecuta el hijo)
        // HIJO (M) 
        printf("Hijo (M): Escribir en datos.txt\n");
        
        FILE *archivo = fopen("datos.txt", "w"); //hijo abre archivo modo escritura 
        if (archivo == NULL) {
            perror("Error al abrir archivo en hijo");
            exit(1);
        }

        // Escribe 5 enteros en el archivo
        for (int i = 1; i <= 5; i++) {
            fprintf(archivo, "%d\n", i * 10); //en cada vuelta multiplica el numero * 10 y lo escribe: 10, 20,30,40,50
        }

        fclose(archivo); //cierra archivo para que padre lo lea despues
        printf("Hijo (M): Datos guardados.\n");
        exit(0); //el hijo termina su vida aqui para no hacer lo del padre 
    } 
    else {
        // PADRE (Q) -solo lo hace el padre 
        // Sleep(3): si no espera, intentaria leer el archivo antes de que el hijo termine de escribir 
        printf("Padre (Q): Espera 3 segundos...\n");
        sleep(3);

        printf("Padre (Q): Lee datos del archivo\n");
        FILE *archivo = fopen("datos.txt", "r"); //abre archivo en modo lectura 
        if (archivo == NULL) {
            perror("Error al abrir archivo");
            return 1;
        }
        // Datos leidos
        int numero;
        while (fscanf(archivo, "%d", &numero) != EOF) { //lee archivo numero por numero hasta llegar al final End Of File
            printf("Valor: %d\n", numero); //muestra lo que encontro
        }

        fclose(archivo);
        printf("Padre (Q): Proceso completado.\n");
    }

    return 0;
}