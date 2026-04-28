#include "ascension_magios.h"
#include <stdlib.h> // Para usar rand, limpiar terminal, etc.
#include <time.h>   // Para obtener una semilla desde el reloj
#include <stdio.h>

const char* MSJ_MOVIMIENTOS = "      -W-          -S-            -A-           -D- \n     Arriba       Abajo        Izquierda      Derecha \n";
const char* MSJ_ELIMINACION = "\n\nELIMINADO...!!\n\nTe quedaste sin vidas\n";
const char* MSJ_JUEGO_GANADO = "\n En hora buena...\n\n tú ascención fue aprobada, pronto nos comunicaremos contigo\n";

/*
Pre condiciones: -
Post condiciones: devuelve true si el 'movimiento' es H, L, D, S, A ó W. False en caso contrario.
*/
bool es_opcion(char movimiento){
    return(movimiento == 'H' || movimiento == 'L' || movimiento == 'D' || movimiento == 'S' || movimiento == 'A' || movimiento == 'W');
}
/*
Pre condiciones: -
Post condiciones: guarda en 'movimiento' el caracter ingresado por el usuario, movimiento debe ser  H, L, D, S, A, W. en caso de nos er ninguna de las anterioeres opciones vuelve a pedir otro caracter. 
*/
void pedir_movimiento(char* movimiento){
    printf("%s\n", MSJ_MOVIMIENTOS);
    scanf(" %c", movimiento);
    while (!es_opcion(*movimiento) ){
        printf("El movimiento es invalido. \n+++\nLas opciones son: \nW -Para moverse hacia arriba \nS -Para moverse hacia abajo \nA -Para moverse hacia la izquierda \nD -Para moverse hacia la derecha \nL -Para activar la antorcha \nH -Para activar el hechizo revelador\n+++\n");
        scanf(" %c", movimiento);
    }
}

int main(){
    srand ( (unsigned)time(NULL));
    juego_t juego;

    inicializar_juego(&juego);
    int nivel = juego.nivel_actual;

    while ((estado_juego(juego) == 0) && ( nivel < 3)){
        mostrar_juego(juego);
        
        char movimiento = ' ';
        pedir_movimiento(&movimiento);
        realizar_jugada(&juego, movimiento);

        if (estado_nivel((juego).niveles[nivel], juego.homero) == 1){
            cambiar_nivel(&(juego));
            nivel = juego.nivel_actual;
            juego.camino_visible = true;
        }
    }


    if (estado_juego(juego) == -1){
        system("clear");
        printf("%s", MSJ_ELIMINACION);
    } else {
        system("clear");
        printf("%s", MSJ_JUEGO_GANADO);
    }
    
    return 0;
}