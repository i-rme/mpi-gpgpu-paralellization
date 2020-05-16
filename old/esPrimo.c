#include <stdio.h>
#include <stdlib.h>

int esPrimo(int numero);

int main()
{
    int numero = 567;//Metido a la fuerza, cambiar por los numeros aleatorios que se generen
    //printf("Dime un número: \n");
    //scanf("%d", &numero);
    if (esPrimo(numero)) {
        printf("Es primo");
    } else {
        printf("No es primo");
    }
    return 0;
}


int esPrimo(int numero)
{
    //Si es un uno o un cero
    if(numero == 0 || numero == 1){
        return 0;
    }
    //Si es cuatro (caso especial)
    if (numero == 4){
        return 0;
    }
    //Resto de numeros
    for (int x = 2; x < numero / 2; x++){
        if(numero % x == 0) return 0;
    }
    return 1;
}
