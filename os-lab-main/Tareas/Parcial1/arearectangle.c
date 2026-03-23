#include <stdio.h>

int main() {
    float base, height, area;

    printf("--- Calculadora Area Rectangulo ---\n");

    printf("Base: ");
    scanf("%f", &base);

    printf("Altura: ");
    scanf("%f", &height);

    area = base * height;

    printf("Area del rectangulo: %.2f\n", area);

    return 0;
}