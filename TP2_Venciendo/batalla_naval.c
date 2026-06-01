// gcc *.c oponente.o -o batalla_naval -std=c99 -Wall -Wconversion -Werror -lm
// ./batalla_naval <archivo_barcos> <archivo_reporte>
#include "oponente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_FILAS 10
#define MAX_COLUMNAS 10

#define ARCH_REPORTS "reportes.csv"
#define LECTURA "r"
#define ESCRITURA "w"
#define FORMATO_LECTURA_BARCOS "%i;%i;%[^;];%i\n"

#define FORMATO_LECTURA_SOBRES  "%i;%c\n"
#define FORMATO_ESCRITURA_FIGUS "%i;%s;%s\n"

const int CANT_ARGS_MIN = 1;
const int CANT_ARGS_MAX = 3;

const int EXITO = 0;
const int ERROR = -1;
const int ERROR_APERTURA = 3; 
const int ERROR_ARGS = 4;

const char BARCO = 'B';
const char VACIO = '-';

typedef struct barco_usuario {
    int largo;
    int fila;
    int columna;
    char direccion;
} barco_usuario_t;

int guardar_barco_usuario(barco_usuario_t barco_usuario[CANT_BARCOS], char* ruta){
    FILE* arch_pos_barc = fopen(ruta, LECTURA);
    if (!arch_pos_barc){
        printf("Error al abrir el archivo de posiciones de los barcos\n");
        return ERROR_APERTURA;
    }
    /*Cargar los barcos al tablero*/
    
    for (int i = 0; i < CANT_BARCOS; i++){
        fscanf(arch_pos_barc,FORMATO_LECTURA_BARCOS, &barco_usuario[i].fila, &barco_usuario[i].columna, &barco_usuario[i].direccion, &barco_usuario[i].largo);
    }
    fclose(arch_pos_barc);
    return EXITO;
}

void posicionar_barcos(char usuario_tablero[MAX_FILAS][MAX_COLUMNAS], barco_usuario_t barco_usuario[CANT_BARCOS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            for(int b = 0; b < CANT_BARCOS; b++){   
                if (i == barco_usuario[b].fila && j == barco_usuario[b].columna){
                    usuario_tablero[i][j]=BARCO;
                    if (barco_usuario[b].direccion==)
                    {
                        /* code */
                    }
                    
                } else if (usuario_tablero[i][j] != BARCO ) {
                    usuario_tablero[i][j]=VACIO;
                }
            }
        }
    }   
}

void imprimir_tablero(char usuario_tablero[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            printf("%c ", usuario_tablero[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char* argv[]){
    char usuario_tablero[MAX_FILAS][MAX_COLUMNAS];
    // char oponente_tablero[MAX_FILAS][MAX_COLUMNAS];
    barco_usuario_t barco_usuario[CANT_BARCOS];

    if (argc < CANT_ARGS_MIN || argc > CANT_ARGS_MAX){
        printf ("Error en la cantidad de argumentos");
        return ERROR_ARGS;
    }
    if (guardar_barco_usuario(barco_usuario, argv[1]) != 0){
        return ERROR;
    }
    printf("inicialización correcta\n");
    posicionar_barcos(usuario_tablero, barco_usuario);

    imprimir_tablero(usuario_tablero);

    return EXITO;
}