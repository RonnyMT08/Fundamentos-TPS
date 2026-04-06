#include "ascension_magios.h"
#include <stdio.h>

int main(){
    juego_t juego;
    inicializar_juego(&juego);
    mostrar_juego(juego);
    return 0;
}