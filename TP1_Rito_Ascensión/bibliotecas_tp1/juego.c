#include "ascension_magios.h"
#include <stdlib.h> 
#include <time.h> 
#include <stdio.h>

const int MINIMO_VIDAS = 0;
const int JUEGO_JUGANDO = 0;
const int NIVEL_ACTUAL_GANADO = 1;
const int NIVEL_ACTUAL_JUGANDO = 0;
const int ESTADO_JUEGO_GANADO = 1;
const int ESTADO_JUEGO_PERDIDO = -1;

const char MOVERSE_ARRIBA = 'W';
const char MOVERSE_ABAJO = 'S';
const char MOVERSE_DERECHA = 'D';
const char MOVERSE_IZQUIERDA = 'A';

const char ACTIVAR_ANTORCHA = 'L';
const char ACTIVAR_HECHIZO_REVELADOR = 'H';

const char* MSJ_EMOJI_DIRECCIONES = "Movimientos: \u2B06\uFE0F (W)     \u2B07\uFE0F (S)    \u2B05\uFE0F (A)     \u27A1\uFE0F (D)";
const char* MSJ_ELIMINACION = "\
########  ######## ########  ########  ####  ######  ######## ######## \n\
##     ## ##       ##     ## ##     ##  ##  ##    ##    ##    ##       \n\
##     ## ##       ##     ## ##     ##  ##  ##          ##    ##       \n\
########  ######   ########  ##     ##  ##   ######     ##    ######   \n\
##        ##       ##   ##   ##     ##  ##        ##    ##    ##       \n\
##        ##       ##    ##  ##     ##  ##  ##    ##    ##    ##       \n\
##        ######## ##     ## ########  ####  ######     ##    ######## \n";

 
const char* MSJ_JUEGO_GANADO = "\
 ######      ###    ##    ##    ###     ######  ######## ######## \n\
##    ##    ## ##   ###   ##   ## ##   ##    ##    ##    ##       \n\
##         ##   ##  ####  ##  ##   ##  ##          ##    ##       \n\
##   #### ##     ## ## ## ## ##     ##  ######     ##    ######   \n\
##    ##  ######### ##  #### #########       ##    ##    ##       \n\
##    ##  ##     ## ##   ### ##     ## ##    ##    ##    ##       \n\
 ######   ##     ## ##    ## ##     ##  ######     ##    ######## \n";

const char* MSL_MOVIMIENTO_INVALIDO = "\u26A0\uFE0F  El movimiento es invalido. \U0001f4dd Verifique los movimientos validos";

/*
Pre condiciones: -
Post condiciones: devuelve true si el 'movimiento' es H, L, D, S, A ó W. False en caso contrario.
*/
bool es_opcion(char movimiento){
    return(movimiento == ACTIVAR_HECHIZO_REVELADOR || movimiento == ACTIVAR_ANTORCHA || movimiento == MOVERSE_DERECHA || movimiento == MOVERSE_ABAJO || movimiento == MOVERSE_IZQUIERDA || movimiento == MOVERSE_ARRIBA
    );
}

/*
Pre condiciones: -
Post condiciones: guarda en 'movimiento' el caracter ingresado por el usuario, movimiento debe ser  H, L, D, S, A, W. en caso de nos er ninguna de las anterioeres opciones vuelve a pedir otro caracter. 
*/
void pedir_movimiento(juego_t juego, char* movimiento){
    printf("%s \n", MSJ_EMOJI_DIRECCIONES);
    scanf(" %c", movimiento);
    while (!es_opcion(*movimiento) ){
        mostrar_juego(juego);
        printf("%s \n%s\n",MSJ_EMOJI_DIRECCIONES, MSL_MOVIMIENTO_INVALIDO); 
        scanf(" %c", movimiento);
    }
}

int main(){
    srand ( (unsigned)time(NULL));
    
    juego_t juego;
    inicializar_juego(&juego);
    
    int indice_nivel = juego.nivel_actual - 1;
    while ((estado_juego(juego) == JUEGO_JUGANDO)){
        mostrar_juego(juego);
        
        char movimiento = ' ';
        pedir_movimiento(juego, &movimiento);
        realizar_jugada(&juego, movimiento);

        if (estado_nivel((juego).niveles[indice_nivel], juego.homero) == NIVEL_ACTUAL_GANADO){
            cambiar_nivel(&(juego));
        }
        indice_nivel = juego.nivel_actual - 1;  
    }

    if (estado_juego(juego) == ESTADO_JUEGO_PERDIDO){
        system("clear");
        printf("%s", MSJ_ELIMINACION);
    } else if (estado_juego(juego) == ESTADO_JUEGO_GANADO && juego.homero.vidas_restantes >= MINIMO_VIDAS){
        system("clear");
        printf("%s", MSJ_JUEGO_GANADO);
    }
    return 0;
}