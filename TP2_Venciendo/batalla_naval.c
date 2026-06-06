// gcc *.c -g oponente.o -o batalla_naval -std=c99 -Wall -Wconversion -Werror -lm
// valgrind --leak-check=full --show-leak-kinds=all ./batalla_naval barcos.csv reportes.csv
#include "oponente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_FILAS 10
#define MAX_COLUMNAS 10
#define MAXIMO_JUGADAS 100 

#define EMOJI_NUBES "\U0001f32b\uFE0F"
#define EMOJI_AGUA "\U0001f7e6"
#define EMOJI_BARCO "\U0001f6a2"
#define EMOJI_ATACADO "\u274C"
#define EMOJI_BARCO_ELIMINADO "\U0001f525"

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
const char BARCO_IMPACTADO = 'H';
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

void posicionar_largo_bar(barco_t* barco, posicion_t posicion){
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

int guardar_barcos_jugador(barco_t barcos_jugador[CANT_BARCOS], char* archivo_barcos){
    FILE* arch_pos_barc = fopen(archivo_barcos, LECTURA);
    if (!arch_pos_barc){
        printf("Error al abrir el archivo de posiciones de los barcos\n");
        return ERROR_APERTURA;
    }
    posicion_t posiciones[CANT_BARCOS];
    int linea = 0;
    int leidos = fscanf(arch_pos_barc, FORMATO_LECTURA_BARCOS,
        &posiciones[linea].fila,
        &posiciones[linea].columna,
        &posiciones[linea].direccion,
        &posiciones[linea].largo);
    if (leidos == EOF){
        printf("Error el archivo de bascos.csv se encuentra vacio\n");
        fclose(arch_pos_barc);
        return ERROR_APERTURA;
    }

    for (int i = 1; i < CANT_BARCOS; i++){
        leidos = fscanf(arch_pos_barc, FORMATO_LECTURA_BARCOS,
            &posiciones[i].fila,
            &posiciones[i].columna,
            &posiciones[i].direccion,
            &posiciones[i].largo);
        
        if (leidos != 4){
            printf("Error en la cantidad de barcos en el archivo de barcos.csv (se necesita 5 lineas con los barcos)\n");
            fclose(arch_pos_barc);
            return ERROR;
        }
    }
    fclose(arch_pos_barc);
    guardar_barco(posiciones, barcos_jugador);
    return EXITO;
}

int procesar_archivos(barco_t barcos_jugador[CANT_BARCOS], char* archivo_barcos ){
    if (guardar_barcos_jugador(barcos_jugador, archivo_barcos) != 0){
        return ERROR;
    }
    return EXITO;
}

void inicializar_tableros(char tablero_jugador[MAX_FILAS][MAX_COLUMNAS], char tablero_oponente[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            tablero_jugador[i][j] = VACIO;
            tablero_oponente[i][j] = VACIO; 
        }
    }
}

void posicionar_barcos(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS], barco_t barco_jugador[CANT_BARCOS]){
    for (int barco = 0; barco < CANT_BARCOS; barco++){
        for (int largo = 0; largo < barco_jugador[barco].largo; largo++){
            int fila = barco_jugador[barco].posiciones[largo].fila;
            int columna = barco_jugador[barco].posiciones[largo].columna;
            if (fila >= 0 && fila < MAX_FILAS && columna >= 0 && columna < MAX_COLUMNAS){
                jugador_tablero[fila][columna] = BARCO;
            }
        }
    }
}

void imprimir_tablero(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS]){
    printf("==BIENVENIDO A LA BATALLA NAVAL==\n");
    printf("============MIS FLOTAS=============\n");
    printf("   01 02 03 04 05 06 07 08 09 10\n");
    for (int i = 0; i < MAX_FILAS; i++){
        if(i < 9){
            printf("0%i ", i+1);
        } else {
            printf("%i ", i+1);
        }
        for (int j = 0; j < MAX_COLUMNAS; j++){
            if( i > 0 && jugador_tablero[i][j] == BARCO){
                printf("%s ", EMOJI_BARCO);
            } else if (jugador_tablero[i][j] == BARCO_IMPACTADO){
                printf("%s ",EMOJI_ATACADO);
            } else if( jugador_tablero[i][j] == AGUA){
                printf("%s ",EMOJI_AGUA);
            } else {
                printf("%s  ",EMOJI_NUBES);
            }
        }
        printf("\n");
    }
    printf("=================================\n");

}



void imprimir_tablero_oponente(char enemigo_tablero[MAX_FILAS][MAX_COLUMNAS]){
    printf("========FLOTAS DEL OPONENTEO========\n");
    printf("   01 02 03 04 05 06 07 08 09 10\n");
    for (int i = 0; i < MAX_FILAS; i++){
        if (i < 9){
            printf("0%i ", i+1);
        } else {
            printf("%i ", i+1);
        }
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if( enemigo_tablero[i][j] == BARCO){
                printf("%s ", EMOJI_ATACADO);
            } else if (enemigo_tablero[i][j] == AGUA){
                printf("%s ",EMOJI_AGUA);
            } else if (enemigo_tablero[i][j] == 'T'){
                printf("%s ", EMOJI_ATACADO);
            } else if (enemigo_tablero[i][j] == 'H'){
                printf("%s ", EMOJI_BARCO_ELIMINADO);
            } else {
                printf("%s  ",EMOJI_NUBES);
            }
        }
        printf("\n");
    }
    printf("=================================\n");
}

void pedir_jugada( int* fila_disparo, int* columna_disparo){
    int fila = 0;
    int columna = 0; 
    printf("Ingrese las coordendas de su\npróximo disparo <fila>;<columna>:");
    scanf("%i;%i", &fila, &columna);
    while (!((fila > 0 && fila <= 10) && (columna > 0 && columna <= 10))){
        printf("Por favor ingrese las coordendas\npor ejemplo '1;1':\n");
        scanf("%d;%d ", &fila, &columna);
    }
    (*fila_disparo) = fila;
    (*columna_disparo) = columna;
}

void imprimir_tableros(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS], char oponente_tablero[MAX_FILAS][MAX_COLUMNAS]){
    imprimir_tablero(jugador_tablero);
    imprimir_tablero_oponente(oponente_tablero);
}

void accionar_disparo(char tablero_oponente[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_disparo, char impacto){
    int fila_disparo = posicion_disparo.fila-1;
    int columna_disparo = posicion_disparo.columna-1;

    for (int i = 0; i < MAX_FILAS; i++ ){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if ( fila_disparo == i && columna_disparo == j){
                tablero_oponente[i][j] = impacto;
            }
        }
    }
}
void accionar_disparo_oponete(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_disparo_oponente){
    int fila_disparo = posicion_disparo_oponente.fila -1;
    int columna_disparo = posicion_disparo_oponente.columna -1;

    for (int i = 0; i < MAX_FILAS; i++ ){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if (fila_disparo == i && columna_disparo == j){
                if ( jugador_tablero[i][j] == BARCO){
                    jugador_tablero[i][j] = BARCO_IMPACTADO;
                } else if(jugador_tablero[i][j] == VACIO){
                    jugador_tablero[i][j] = AGUA;
                }
            }
        }
    }
}


int estado_juego(){
    int salida = 1;
    // printf("para salir: 0");
    // scanf("%i", &salida);
    return salida;
}

int main(int argc, char* argv[]){
    char jugador_tablero[MAX_FILAS][MAX_COLUMNAS];
    char oponente_tablero[MAX_FILAS][MAX_COLUMNAS];
    barco_t barcos_jugador[CANT_BARCOS];

    for (int i = 0; i < CANT_BARCOS; i++) {
        barcos_jugador[i].posiciones = malloc(sizeof(coordenada_t) * MAXIMO_JUGADAS);
        if (!barcos_jugador[i].posiciones){
            printf("Error al reservar memoria para las posiciones de los barcos\n");
            return ERROR;
        }
    }
    
    if (argc != CANT_ARGS_MAX){
        printf ("Error en la cantidad de argumentos");
        for (int i = 0; i < CANT_BARCOS; i++) {
            free(barcos_jugador[i].posiciones);
        }
        return ERROR_ARGS;
    }
    if (procesar_archivos(barcos_jugador, argv[1]) != 0){
        for (int i = 0; i < CANT_BARCOS; i++) {
            free(barcos_jugador[i].posiciones);
        }
        return ERROR;
    }
    oponente_t *oponente;
    oponente = oponente_crear(barcos_jugador);
    if (!oponente){
        printf("Error al reservar memoria para el oponente tda\n");
        for (int i = 0; i < CANT_BARCOS; i++) {
            free(barcos_jugador[i].posiciones);
        }
        return ERROR;
    }

    inicializar_tableros(jugador_tablero, oponente_tablero);
    posicionar_barcos(jugador_tablero, barcos_jugador);
    while (estado_juego() != 0){
        system("clear");
        coordenada_t posicion_disparo;
        coordenada_t posicion_disparo_oponente;

        imprimir_tableros(jugador_tablero, oponente_tablero);
        pedir_jugada(&posicion_disparo.fila, &posicion_disparo.columna);
        char impacto;
        impacto = oponente_recibe_disparo(oponente, posicion_disparo);
        accionar_disparo(oponente_tablero, posicion_disparo, impacto);
        posicion_disparo_oponente = oponente_realiza_disparo(oponente);
        accionar_disparo_oponete(jugador_tablero, posicion_disparo_oponente);
        estado_juego();
    }

    for (int i = 0; i < CANT_BARCOS; i++) {
        free(barcos_jugador[i].posiciones);
    }   
    oponente_destruir(oponente);
    return EXITO;
}