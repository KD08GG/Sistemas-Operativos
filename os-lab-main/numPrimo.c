#include <stdio.h>
#include <stdbool.h>

// Función que verifica si un número es primo
bool esPrimo(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;

    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0)
            return false;
    }
    return true;
}

int main() {
    int numeroBase = 20; 

    int candidato = numeroBase + 1;

    while (!esPrimo(candidato)) {
        candidato++;
    }

    printf("El primer numero primo mayor que %d es: %d\n", numeroBase, candidato);

    return 0;
}
