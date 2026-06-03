// gcc *.c -g oponente.o -o batalla_naval -std=c99 -Wall -Wconversion -Werror -lm
// ./batalla_naval barcos.csv reportes.csv
#include "oponente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_FILAS 10
#define MAX_COLUMNAS 10

#define ARCH_REPORTS "reportes.csv"
#define LECTURA "r"
#define ESCRITURA "w"
#define FORMATO_LECTURA_BARCOS "%i;%i;%c;%i"

#define FORMATO_LECTURA_SOBRES  "%i;%c\n"
#define FORMATO_ESCRITURA_FIGUS "%i;%s;%s\n"

const int CANT_ARGS_MIN = 2;
const int CANT_ARGS_MAX = 3;

const int EXITO = 0;
const int ERROR = -1;
const int ERROR_APERTURA = 3; 
const int ERROR_ARGS = 4;

const char BARCO = 'B';
const char VACIO = '-';
const char AGUA = 'A';

const char ESTE = 'E';
const char OESTE = 'O';
const char NORTE = 'N';
const char SUR = 'S';

typedef struct posiciones {
    int fila;
    int columna; 
    char direccion;
    int largo;
} posicion_t;

void posicionar_largo_bar(barco_t *barco, posicion_t posicion){
    (*barco).largo = posicion.largo;
    (*barco).posiciones[0].fila = posicion.fila;
    (*barco).posiciones[0].columna = posicion.columna;
    for (int i = 1; i < posicion.largo; i++){
        if (posicion.direccion == ESTE){
            (*barco).posiciones[i].fila = posicion.fila;
            (*barco).posiciones[i].columna = posicion.columna+i;
        } else if (posicion.direccion == OESTE){
            (*barco).posiciones[i].fila = posicion.fila;
            (*barco).posiciones[i].columna = posicion.columna-i;
        } else if (posicion.direccion == NORTE){
            (*barco).posiciones[i].fila = posicion.fila-i;
            (*barco).posiciones[i].columna = posicion.columna;
        } else if (posicion.direccion == SUR){
            (*barco).posiciones[i].fila = posicion.fila+i;
            (*barco).posiciones[i].columna = posicion.columna;
        }
    }
}

void guardar_barco(posicion_t posiciones[CANT_BARCOS], barco_t barcos_jugador[CANT_BARCOS]){
    for (int i = 0; i < CANT_BARCOS; i++){
        posicionar_largo_bar(&barcos_jugador[i], posiciones[i]);
    }
}

int guardar_barcos_jugador(barco_t barcos_jugador[CANT_BARCOS], char* ruta){
    FILE* arch_pos_barc = fopen(ruta, LECTURA);
    if (!arch_pos_barc){
        printf("Error al abrir el archivo de posiciones de los barcos\n");
        return ERROR_APERTURA;
    }
    posicion_t posiciones[CANT_BARCOS];
    for (int i = 0; i < CANT_BARCOS; i++){
        int leidos = fscanf(arch_pos_barc, FORMATO_LECTURA_BARCOS,
            &posiciones[i].fila,
            &posiciones[i].columna,
            &posiciones[i].direccion,
            &posiciones[i].largo);
        if (leidos != 4){
            printf("Error de formato en el archivo de barcos\n");
            fclose(arch_pos_barc);
            return ERROR;
        }
    }
    fclose(arch_pos_barc);
    guardar_barco(posiciones, barcos_jugador);
    return EXITO;
}

void posicionar_barcos(char usuario_tablero[MAX_FILAS][MAX_COLUMNAS], barco_t barco_usuario[CANT_BARCOS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            usuario_tablero[i][j] = VACIO;
        }
    }
    for (int barco = 0; barco < CANT_BARCOS; barco++){
        for (int largo = 0; largo < barco_usuario[barco].largo; largo++){
            int fila = barco_usuario[barco].posiciones[largo].fila;
            int columna = barco_usuario[barco].posiciones[largo].columna;
            if (fila >= 0 && fila < MAX_FILAS && columna >= 0 && columna < MAX_COLUMNAS){
                usuario_tablero[fila][columna] = BARCO;
            }
        }
    }
}

void imprimir_tablero(char usuario_tablero[MAX_FILAS][MAX_COLUMNAS]){
    printf("BIENVENIDO A LA BATALLA NAVAL\n");
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            printf("%c ", usuario_tablero[i][j]);
        }
        printf("\n");
    }
}

void realizar_jugada( int* fila_disparo, int* col_disparo){
    printf("Ingrese las coordendas de su proximo disparo:");
    scanf("%i;%i", fila_disparo, col_disparo);
}

void imprimir_tablero_oponente(char enemigo_tablero[MAX_FILAS][MAX_COLUMNAS],int fila_disparo,int col_disparo){
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if(i == fila_disparo && j == col_disparo){
                if (enemigo_tablero[i][j] != BARCO){
                    enemigo_tablero[i][j] = AGUA; 
                }
                printf("%c ",enemigo_tablero[i][j]);
            } else {
                enemigo_tablero[i][j] = VACIO;
                printf("%c ",enemigo_tablero[i][j]);
            }
        }
        printf("\n");
    }
}


int main(int argc, char* argv[]){
    char usuario_tablero[MAX_FILAS][MAX_COLUMNAS];
    char oponente_tablero[MAX_FILAS][MAX_COLUMNAS];
    barco_t barcos_jugador[CANT_BARCOS];
    for (int i = 0; i < CANT_BARCOS; i++) {
        barcos_jugador[i].posiciones = malloc(sizeof(coordenada_t) * MAX_FILAS);
        if (!barcos_jugador[i].posiciones){
            printf("Error al reservar memoria para las posiciones de los barcos\n");
            return ERROR;
        }
    }
    // barco_usuario_t barco_usuario[CANT_BARCOS];

    if (argc != CANT_ARGS_MAX){
        printf ("Error en la cantidad de argumentos");
        return ERROR_ARGS;
    }
    if (guardar_barcos_jugador(barcos_jugador, argv[1]) != 0){
        return ERROR;
    }
    printf("inicialización correcta\n");
    posicionar_barcos(usuario_tablero, barcos_jugador);

    imprimir_tablero(usuario_tablero);

    int fila_disparo;
    int col_disparo;
    realizar_jugada(&fila_disparo, &col_disparo);
    system("clear");
    imprimir_tablero_oponente(oponente_tablero, fila_disparo, col_disparo);


    for (int i = 0; i < CANT_BARCOS; i++) {
        free(barcos_jugador[i].posiciones);
    }
    return EXITO;
}