#include <stdio.h>

// Funcion para calcular factorial
long calcularFactorial(int n) { // long usa más bits: 64 bits
    long resultado = 1;
    
    for (int i = 1; i <= n; i++) {
        resultado *= i; // multiplica resultado actual por el siguiente numero
    }
    return resultado;
}

int main() {
    int num;
    printf("Introduce numero entero: ");
    scanf("%d", &num);

    if (num < 0) { 
        printf("No se permiten numeros negativos, intenta otra vez.\n");
    } else {
        printf("El factorial de %d es: %ld\n", num, calcularFactorial(num));
    }

    return 0;
}