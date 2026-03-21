#include <stdio.h>

/*
PRE:-
POST: Devuelve el equivalente en Fahrenheit
*/
double cambio_de_temperatura_a_fahrenheit (double temperatura_en_celcius) {
    double temperatura_en_fahrenheit = 0;
    temperatura_en_fahrenheit = temperatura_en_celcius*9/5+32;
    return temperatura_en_fahrenheit;
}

int main () {
    double numero_temperatura_en_celcius = 0;
    double numero_temperatura_en_fahrenheit = 0;

    printf("Indique la temperatura en Celcius:");
    scanf("%lf", &numero_temperatura_en_celcius);

    numero_temperatura_en_fahrenheit = cambio_de_temperatura_a_fahrenheit (numero_temperatura_en_celcius);
    printf("Es equivalente a %.3f Fahrenheit.\n", numero_temperatura_en_fahrenheit);

    return 0;
}
