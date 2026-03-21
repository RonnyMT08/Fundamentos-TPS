#include <stdio.h>

const float PI = 3.14159;

float area_del_circulo(float radio){
    float area_calculada = 0;
    area_calculada = PI*radio*radio;
    return area_calculada;
}
int main() {
    float radio_dado = 0;
    float numero_area_del_circulo = 0;

    printf("Ingrese el radio:");
    scanf("%f", &radio_dado);

    numero_area_del_circulo = area_del_circulo(radio_dado);
    printf("El área del círculo es %f\n", numero_area_del_circulo);

    return 0;
}
