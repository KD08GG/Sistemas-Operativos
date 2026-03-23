#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) //rcive numeros 
{
    int suma = 0;

    for (int i = 1; i < argc; i++)
    {
        suma = suma + atoi(argv[i]); //se suma a la suma - convierte string a int 
    }

    return suma;
} 