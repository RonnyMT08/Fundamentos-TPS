#include "ascension_magios.h"
#include "utiles.h"
#include <stdio.h>
#include <stdlib.h> // Para usar rand, limpiar terminal, etc.
#include <time.h>   // Para obtener una semilla desde el reloj

const char HOMERO = 'H';
const char PARED = 'P';
const char CAMINO = 'C';

/*
* Pre condiciones: -
* Post condiciones: devuelve un numero aleatorio entre 20 y 10 inclusive.
*/
int numero_aleatorio(int min, int max){
    srand ( (unsigned)time(NULL));
    int numero = rand() % (max - min + 1) + min;
    return numero;
}

/*
* Pre condiciones: -
* Post condiciones: crea un mapa vacio.
*/
void crear_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            mapa[i][j] = 'p';
        }
    }   
}

/*
* Pre condiciones: -
* Post condiciones: agrega paredes al mapa.
*/
void agregar_paredes(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t paredes[MAX_PAREDES], int tope_paredes){
    //agregar paredes
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if ( (i == 0 && (j >= 0 && j < MAX_COLUMNAS)) || (i == MAX_FILAS-1 && (j >= 0 && j < MAX_COLUMNAS))){
                mapa[i][j] = PARED;
            } else if ( (i > 0 && i < MAX_FILAS) && (j == 0 || j == MAX_COLUMNAS-1)){
                mapa[i][j] = PARED; 
            }
        }
    }
    for (int i = 0; i < tope_paredes; i++){
        int fil_pared = paredes[i].fil;
        int col_pared = paredes[i].col;
        mapa[fil_pared][col_pared] = PARED;
    }
}

/*
* Pre condiciones: -
* Post condiciones: agrega camino al mapa.
*/
void agregar_camino(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino){
    //agregar camino
    for (int i = 0; i < tope_camino; i++){
        int fil_camino = camino[i].fil;
        int col_camino = camino[i].col;
        if(mapa[fil_camino][col_camino] != PARED){
            mapa[fil_camino][col_camino] = '-';
        }
    }
    
}

/*
* Pre condiciones: -
* Post condiciones: actualiza la posicion inical de homero.
*/
void determinar_posicion_inical_homero(int *fil, int *col){
    int min = 1;
    int max_fil = MAX_FILAS - 2;
    int max_col = MAX_COLUMNAS - 2;
    (*fil) = numero_aleatorio(min, max_fil);
    (*col) = numero_aleatorio(min, max_col);
}

void agregar_homero(char mapa[MAX_FILAS][MAX_COLUMNAS], personaje_t homero){
    int fil_homero = homero.posicion.fil;
    int col_homero = homero.posicion.col;

    mapa[fil_homero][col_homero] = HOMERO;
}

void ver_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    //imprimir matriz
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

/////////////////////////////////////////////////////////
void inicializar_juego(juego_t *juego) {
    //personaje
    int homero_fil = (*juego).homero.posicion.fil;
    int homero_col = (*juego).homero.posicion.col;
    determinar_posicion_inical_homero(&homero_fil, &homero_col);
    
    (*juego).homero.posicion.fil = homero_fil;
    (*juego).homero.posicion.col = homero_col;

    (*juego).homero.recolecto_pergamino = false;
    (*juego).homero.hechizos_reveladores = 0;
    (*juego).homero.vidas_restantes = 5;
    (*juego).homero.antorchas = 0;

    // Inicializar niveles[1]

    for (int i = 0; i < MAX_NIVELES; i++){
        int nivel_actual = i + 1;
        (*juego).niveles[i].tope_paredes = 0;
        (*juego).niveles[i].tope_camino = 0;
        (*juego).niveles[i].pergamino.fil = -1;
        (*juego).niveles[i].pergamino.col = -1;
        (*juego).niveles[i].tope_herramientas = 0;
        (*juego).niveles[i].tope_obstaculos = 0;
        obtener_mapa((*juego).niveles[i].paredes, &(*juego).niveles[i].tope_paredes, (*juego).niveles[i].camino, &(*juego).niveles[i].tope_camino, nivel_actual);
    }

    // extra
    (*juego).tope_niveles = 3;
    (*juego).nivel_actual = 0;
}

void cambiar_nivel(juego_t* juego) {
    int siguiente_nivel = 1;
    (*juego).nivel_actual += siguiente_nivel;
}

void realizar_jugada(juego_t *juego, char movimiento){
    switch (movimiento){
        case 'w':
            (*juego).homero.posicion.fil -= 1;
            break;
        case 's':
            (*juego).homero.posicion.fil += 1;
            break;
        case 'a':
            (*juego).homero.posicion.col -= 1;
            break;
        case 'd':
            (*juego).homero.posicion.col += 1;
            break;   
        default:
            break;
    }
}

void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    int numero_vidas = juego.homero.vidas_restantes;
    int numero_echizos = juego.homero.hechizos_reveladores;
    int numero_antorchas = juego.homero.antorchas; 
    
    crear_mapa(mapa);
    agregar_paredes(mapa, juego.niveles[juego.nivel_actual].paredes, juego.niveles[juego.nivel_actual].tope_paredes);
    
    agregar_camino(mapa, juego.niveles[juego.nivel_actual].camino, juego.niveles[juego.nivel_actual].tope_camino);
    agregar_homero(mapa, juego.homero);
    system("clear");

    while (estado_juego(juego) == 0){
        char movimiento;

        printf("--------------------Welcome to The Game--------------------\n\n");
        ver_mapa(mapa);
        printf("-----------------------------------------------------------\n\n");
        printf("Vidas: %i,             Echizo: %i,            Antorchas: %i\n", numero_vidas, numero_echizos, numero_antorchas);
        printf("Arriba -w-     Abajo -s-     Izquierda -a-     Derecha -d- \n");
        scanf(" %c", &movimiento);

        realizar_jugada(&juego, movimiento);
        mostrar_juego(juego);
        system("clear");
    }
}  

int estado_nivel(nivel_t nivel){
    int pergamino_altar = 0; 
    if (pergamino_altar == 3){
        printf("ganado!!");
        return 1;
    } else {
        printf("jugando");
        return 0;
    }
}

int estado_juego(juego_t juego){
    int ganado = 0;
    if (ganado == 1){
        return 1;
    } else if(ganado == -1){
        return -1;
    } else {
        return 0;
    }

}