#include "ascension_magios.h"
#include <stdlib.h> // Para usar rand, limpiar terminal, etc.
#include <time.h>   // Para obtener una semilla desde el reloj

#include <stdio.h>

int main(){
    srand ( (unsigned)time(NULL));

    juego_t juego;

    inicializar_juego(&juego);
    mostrar_juego(juego);
    return 0;
}