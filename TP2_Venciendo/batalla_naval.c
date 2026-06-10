// gcc *.c -g oponente.o -o batalla_naval -std=c99 -Wall -Wconversion -Werror -lm
// valgrind --leak-check=full --show-leak-kinds=all ./batalla_naval barcos.csv reportes.csv
#include "oponente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MIN_FILAS 1
#define MIN_COLUMNAS 1
#define MAX_FILAS 10
#define MAX_COLUMNAS 10
#define MAXIMO_LARGO_BARCO 5
#define MAX_CATEGORIAS 50
#define MAX_VALORES 6
#define CANT_CATEGORIAS 6

#define EMOJI_NUBES "\U0001f32b\uFE0F"
#define EMOJI_AGUA "\U0001f7e6"
#define EMOJI_BARCO "\U0001f6a2"
#define EMOJI_ATACADO "\u274C"
#define EMOJI_BARCO_ELIMINADO "\U0001f525"

#define ARCH_REPORTS "reportes.csv"
#define LECTURA "r"
#define ESCRITURA "w"
#define FORMATO_LECTURA_BARCOS "%i;%i;%c;%i"
#define FORMATO_ESCRITURA_REPORTES "%s :%i\n"

const int CANT_ARGS_MIN = 2;
const int CANT_ARGS_MAX = 3;
const int VALOR_NULO = 0;

const int EXITO = 0;
const int ERROR = -1;
const int ERROR_APERTURA = 3; 
const int ERROR_ARGS = 4;

const int MIN_LARGO_BARCO = 1;
const int MAX_LARGO_BARCO = 5;

const char BARCO = 'B';
const char TOCADO = 'T';
const char VACIO = '-';
const char AGUA = 'A';
const char HUNDIDO = 'H';

const char ESTE = 'E';
const char OESTE = 'O';
const char NORTE = 'N';
const char SUR = 'S';

const int ESTADO_JUGANDO = -1;
const int ESTADO_TERMINADO = 0;

const char CATEGORIAS[CANT_CATEGORIAS][MAX_CATEGORIAS] = {"Balas aliadas acertadas", "Balas aliadas erradas", "Balas enemigas acertadas", "Balas enemigas erradas", "Barcos enemigos hundidos", "Barcos aliados sobrevivientes"};

typedef struct posiciones {
    int fila;
    int columna; 
    char direccion;
    int largo;
} posicion_t;

typedef struct balas {
    int valores_categoria[MAX_VALORES];
} balas_t;

/* 
    Precondiciones: Los valores de 'posiciones' deben estar inicializados.
    Postcondiciones: Verifica que los datos de los barcos obtenidos del archivo 'barcos.csv'
        devuelve EXITO si los datos son correctos. ERROR si algun dato de barco esta escrito de manera erronea.
*/ 
int validar_posiciones_archivo(posicion_t posiciones[CANT_BARCOS]){
    bool posiciones_validas = true;
    int i = 0;
    while (i < CANT_BARCOS && posiciones_validas){
        if( !(posiciones[i].fila >= MIN_FILAS && posiciones[i].fila <= MAX_FILAS && posiciones[i].columna >= MIN_COLUMNAS && posiciones[i].columna <= MAX_COLUMNAS)){
            posiciones_validas = false;
        } 
        if( !(posiciones[i].direccion == ESTE || posiciones[i].direccion == OESTE || posiciones[i].direccion == NORTE || posiciones[i].direccion == SUR )){
            posiciones_validas = false;
        }
        if( !(posiciones[i].largo >= MIN_LARGO_BARCO && posiciones[i].largo <= MAX_LARGO_BARCO)){
            posiciones_validas = false;
        }
        i++;
    }
    if (!posiciones_validas){
        return ERROR;
    }
    return EXITO;
}

/* 
    Precondiciones: los valores de 'posicion' deben estar inicialzados.
    Postcondiciones: Guarda los valores en 'barco_jugador' de las coordenadas del barco según su largo(2,3,4,5) y direccion(ESTE, OESTE, NORTE, SUR) dados por el parametro 'posicion'.
*/
int guardar_direccion_posicion(barco_t* barco_jugador, posicion_t posicion){
    bool direcciones_posiciones_validas = true;
    int posicion_fila = posicion.fila -1;
    int posicion_columna = posicion.columna -1;

    for (int i = 1; i < posicion.largo; i++){
        if (posicion.direccion == ESTE){
            (*barco_jugador).posiciones[i].fila = posicion_fila;
            (*barco_jugador).posiciones[i].columna = (posicion_columna) + i;
        } else if (posicion.direccion == OESTE){
            (*barco_jugador).posiciones[i].fila = posicion_fila;
            (*barco_jugador).posiciones[i].columna = (posicion_columna) - i;
        } else if (posicion.direccion == NORTE){
            (*barco_jugador).posiciones[i].fila = (posicion_fila) - i;
            (*barco_jugador).posiciones[i].columna = posicion_columna;
        } else if (posicion.direccion == SUR){
            (*barco_jugador).posiciones[i].fila = (posicion_fila) + i;
            (*barco_jugador).posiciones[i].columna = posicion_columna;
        }

        if ( (*barco_jugador).posiciones[i].fila < 0 || (*barco_jugador).posiciones[i].fila >= MAX_FILAS || (*barco_jugador).posiciones[i].columna < 0 || (*barco_jugador).posiciones[i].columna >= MAX_COLUMNAS  ){
            direcciones_posiciones_validas = false;
        }
    }

    if(!direcciones_posiciones_validas){
        return ERROR;
    }
    return EXITO;
}

/*
    Precondiciones: Los valores de 'posiciones' deben estar inicializados.
    Postcondiciones: Guarda los valores en 'barco_jugador' de las coordenadas de cada barco dada por el parametro 'posicion' con la primera coordenada(fila,columna), largo(2,3,4,5) y dirección(ESTE, OESTE, NORTE, SUR).
*/
int guardar_barco(posicion_t posiciones[CANT_BARCOS], barco_t barcos_jugador[CANT_BARCOS]){
    bool barco_guardado = true;
    for (int i = 0; i < CANT_BARCOS; i++){
        barcos_jugador[i].largo = posiciones[i].largo;
        barcos_jugador[i].posiciones[0].fila = posiciones[i].fila - 1;
        barcos_jugador[i].posiciones[0].columna = posiciones[i].columna - 1;
        int guardar = guardar_direccion_posicion(&barcos_jugador[i], posiciones[i]);
        if (guardar == ERROR){
            barco_guardado = false;
        }
    }
    if (!barco_guardado){
        return ERROR;
    }
    return EXITO;
}

/*
    Precondiciones: El archivo 'arch_pos_barc' debe estar abierto en modo LECTURA.
    Postcondiciones: Guarda en 'barcos_jugador' todas las posiciones de los barcos recibidos por en el 'archivo_pos_barcos'. Devuelve EXITO si se guardo correctamente las posiciones, ERROR en caso contrario
*/
int guardar_barcos_jugador(barco_t barcos_jugador[CANT_BARCOS], FILE* arch_pos_barc){
    
    bool posiciones_barcos_validos = true;
    posicion_t posiciones[CANT_BARCOS];
    int linea = 0;
    int leidos = fscanf(arch_pos_barc, FORMATO_LECTURA_BARCOS, &posiciones[linea].fila, &posiciones[linea].columna, &posiciones[linea].direccion, &posiciones[linea].largo);
    if (leidos == EOF){
        printf("Error el archivo de bascos.csv se encuentra vacio\n");
        fclose(arch_pos_barc);
        return ERROR_APERTURA;
    }

    for (int i = 1; i < CANT_BARCOS; i++){
        leidos = fscanf(arch_pos_barc, FORMATO_LECTURA_BARCOS, &posiciones[i].fila, &posiciones[i].columna, &posiciones[i].direccion, &posiciones[i].largo);
    
        if (leidos != 4){
            printf("Error en la cantidad de barcos en el archivo de barcos.csv (se necesita 5 lineas con los barcos)\n");
            fclose(arch_pos_barc);
            posiciones_barcos_validos = false;
        }
    }

    if (!posiciones_barcos_validos){
        return ERROR;
    }

    int archivos_validos;
    archivos_validos = validar_posiciones_archivo(posiciones);
    if (archivos_validos != EXITO ){
        printf("Error en el contenido del archivo barcos.csv\n");
        return ERROR;
    }

    int barcos_guardados = guardar_barco(posiciones, barcos_jugador);
    if (barcos_guardados == ERROR){
        return ERROR;
    }
    return EXITO;
}

/*
    Precondiciones: -
    Postcondiciones: Abre en modo lectura el 'archivo_barcos', guarda los datos en 'barcos_jugador', Devuelve ERROR_APERTURA si no se pudo abrir el archivo, ERROR si no se pudo guardar los barcos. EXITO en caso contrario. 
*/
int procesar_archivos(barco_t barcos_jugador[CANT_BARCOS], char* archivo_barcos ){
    FILE* arch_pos_barc = fopen(archivo_barcos, LECTURA);
    if (!arch_pos_barc){
        printf("Error al abrir el archivo de posiciones de los barcos(posiblemente no exista)\n");
        return ERROR_APERTURA;
    }

    if (guardar_barcos_jugador(barcos_jugador, arch_pos_barc) != EXITO){
        return ERROR;
        fclose(arch_pos_barc);
    }
    fclose(arch_pos_barc);

    return EXITO;
}

/*
    Precondicones: -
    Postcondiciones: Inicializa todos los valores de las categorias del reporte con un mismo valor inicial = 0.
*/
void inicializar_reporte(balas_t* reporte_balas){
    for (int i = 0; i < MAX_VALORES; i++){
        (*reporte_balas).valores_categoria[i] = VALOR_NULO;
    }
}

/*
    Precondiciones: -
    Postcondiciones: Inicializa todos los valores del 'tablero_jugador' y 'tablero_oponente' con VACIO(-).
*/
void inicializar_tableros(char tablero_jugador[MAX_FILAS][MAX_COLUMNAS], char tablero_oponente[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            tablero_jugador[i][j] = VACIO;
            tablero_oponente[i][j] = VACIO; 
        }
    }
}

/*
    Precondiciones: Los valores de 'barco_jugador' deben estar inicializadados.
    Postcondiciones: Carga todas las coordenadas de los BARCOS dados por 'barco_jugador' en el 'jugador_tablero' segun la coordenada(fila, columna) correspondiente.
*/
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

/*
    Precondiciones: los valores de 'jugador_tablero' deben estar reviamente cargados. 
    Postcondiciones: Imprime por pantalla el tablero del jugador con sus respectivos emojis(NUBES, BARCO, IMPACTO, AGUA) en cada posicion.
*/
void imprimir_tablero_jugador(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS]){
    printf("============MIS FLOTAS=============\n");
    printf("   01 02 03 04 05 06 07 08 09 10\n");
    for (int i = 0; i < MAX_FILAS; i++){
        if(i < 9){
            printf("0%i ", i+1);
        } else {
            printf("%i ", i+1);
        }
        for (int j = 0; j < MAX_COLUMNAS; j++){
            if(jugador_tablero[i][j] == BARCO){
                printf("%s ", EMOJI_BARCO);
            } else if (jugador_tablero[i][j] == TOCADO){
                printf("%s ",EMOJI_ATACADO);
            } else if( jugador_tablero[i][j] == AGUA){
                printf("%s ",EMOJI_AGUA);
            } else {
                printf("%s  ",EMOJI_NUBES);
            }
        }
        printf("\n");
    }
}


/*
    Precondiciones: Los valores de 'oponente_tablero' deben estar previamente cargados.
    Postcondiciones: Imprime por pantalla el tablero del oponente con sus respectivos emojis(NUBES, BARCO, IMPACTO, AGUA) en cada posicion.
*/
void imprimir_tablero_oponente(char oponente_tablero[MAX_FILAS][MAX_COLUMNAS]){
    printf("========FLOTAS DEL OPONENTE=========\n");
    printf("   01 02 03 04 05 06 07 08 09 10\n");
    for (int i = 0; i < MAX_FILAS; i++){
        if (i < 9){
            printf("0%i ", i+1);
        } else {
            printf("%i ", i+1);
        }
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if( oponente_tablero[i][j] == BARCO){
                printf("%s ", EMOJI_ATACADO);
            } else if (oponente_tablero[i][j] == AGUA){
                printf("%s ",EMOJI_AGUA);
            } else if (oponente_tablero[i][j] == 'T'){
                printf("%s ", EMOJI_ATACADO);
            } else if (oponente_tablero[i][j] == 'H'){
                printf("%s ", EMOJI_BARCO_ELIMINADO);
            } else {
                printf("%s  ",EMOJI_NUBES);
            }
        }
        printf("\n");
    }
}

/*
    Precondiciones: -
    Postcondiciones: Pide y carga en 'fila_diaparo' y 'columna_disparo' los valores que ingresa el jugador, los mismos deben estar entre 1 y 10.
*/
void pedir_jugada( int* fila_disparo, int* columna_disparo){
    int fila = 0;
    int columna = 0; 
    printf("Ingrese las coordendas de su\npróximo disparo <fila>;<columna>:");
    scanf("%d;%d", &fila, &columna);
    while (! ((fila >= MIN_FILAS && fila <= MAX_FILAS) && (columna >= MIN_COLUMNAS && columna <= MAX_COLUMNAS)) ){
        printf("Por favor ingrese las coordendas\npor ejemplo '1;1':\n");
        scanf("%d;%d", &fila, &columna);
    }
    (*fila_disparo) = fila-1;
    (*columna_disparo) = columna-1;
}

/*
    Precondiciones: Los valores de 'jugador_tablero' y 'oponente_tablero' deben estar previamente inicializados.
    Postcondiciones: imprime por pantalla los tableros del jugador y del oponente.
*/
void imprimir_tableros(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS], char oponente_tablero[MAX_FILAS][MAX_COLUMNAS]){
    printf("==BIENVENIDO A LA BATALLA NAVAL==\n");
    imprimir_tablero_jugador(jugador_tablero);
    printf("=================================\n");
    imprimir_tablero_oponente(oponente_tablero);
    printf("=================================\n");
}

/*
    Precondiciones: El valor de 'impacto_disparo' debe ser AGUA, TOCADO ó HUNDIDO.
    Postcondiciones: Actualiza el valor en 'reporte_balas' segun el 'impacto_disparo' (AGUA, TOCADO, HUNDIDO) del disparo realizado por el jugador. 
*/
void guardar_disparo(char impacto_disparo, balas_t* reporte_balas, int* barcos_hundidos_oponente){
    if (impacto_disparo == AGUA){
        (*reporte_balas).valores_categoria[1]++ ; 
    } else if (impacto_disparo == TOCADO) {
        (*reporte_balas).valores_categoria[0]++ ;
    } else if (impacto_disparo == HUNDIDO){
        (*barcos_hundidos_oponente)++;
        (*reporte_balas).valores_categoria[0]++ ;
        (*reporte_balas).valores_categoria[4]++ ;
    }
}

/*
    Precondiciones: Los valores de 'tablero_oponente' y 'posicion_disparo' deben estar previamente inicializados. impacto de disparo debe ser HUNDIDO, AGUA, TOCADO.
    Postcondiciones: guarda en las coordendas del tablero oponente el impacto de bala ocasionado, el minmo puede ser HUNDIDO, AGUA, TOCADA 
*/
void accionar_disparo_jugador(char tablero_oponente[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_disparo, char impacto_disparo, balas_t* reporte_balas, int* barcos_hundidos_oponente){

    int fila_disparo = posicion_disparo.fila;
    int columna_disparo = posicion_disparo.columna;

    for (int i = 0; i < MAX_FILAS; i++ ){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if ( fila_disparo == i && columna_disparo == j){
                tablero_oponente[i][j] = impacto_disparo;
            }
        }
    }
    guardar_disparo(impacto_disparo, &(*reporte_balas), &(*barcos_hundidos_oponente));

}
/*
    Precondiciones: Los valores de 'jugador_tablero' de estar previamente inicializado, el tope debe ser concodante con la cantidad de barcos, los valores de 'fila' y 'columna' deben estar entre 1 y 10
    Postcondiciones: Guarda en las coordendas del tablero del jugador el impacto de bala ocasionado por el oponente, el minmo puede ser AGUA, TOCADA 
*/
void estado_barco(barco_t barco_jugador[CANT_BARCOS], int *tope_barcos, int fila, int columna){
    bool encontrado = false;
    int i = 0;
    while (i < CANT_BARCOS && !encontrado){
        int largo = 0;
        while (largo < MAXIMO_LARGO_BARCO && !encontrado){
            if (barco_jugador[i].posiciones[largo].fila == fila && barco_jugador[i].posiciones[largo].columna == columna){
                barco_jugador[i].largo--;
                if (barco_jugador[i].largo == 0){
                    (*tope_barcos)--;
                }
                encontrado = true;
            }
            largo++;
        }
        i++;
    }
}


/*
    Precondiciones: Los valores de 'jugador_tablero' y 'disparo_oponente_jugador' deben estar previamente inicializados.
    Postcondiciones: Guarda en las coordendas del tablero del jugador el impacto de bala ocasionado por el oponente, el minmo puede ser AGUA, TOCADA 
*/
void accionar_disparo_oponente(barco_t barco_jugador[CANT_BARCOS], int *tope_barcos, char jugador_tablero[MAX_FILAS][MAX_COLUMNAS], coordenada_t disparo_oponente_jugador, balas_t* reporte_balas){
    int fila = disparo_oponente_jugador.fila;
    int columna = disparo_oponente_jugador.columna;

    if (jugador_tablero[fila][columna] != BARCO){
        if (jugador_tablero[fila][columna] != TOCADO){
            jugador_tablero[fila][columna] = AGUA;
        }
        (*reporte_balas).valores_categoria[3]++;
    } else {
        jugador_tablero[fila][columna] = TOCADO;
        (*reporte_balas).valores_categoria[2]++;

        estado_barco(barco_jugador, &(*tope_barcos), fila, columna);
    }
}

/*
    Precondiciones: El valor de 'barcos_hundidos_oponente' debe ser > 0 y <=5. 
    Postcondiciones: Devuelve ESTADO_TERMINADO si a el jugador o a él oponente no le quedan barcos en su tablero. ESTADO_JUGANDO en caso contrario.
*/
int estado_juego(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS], int barcos_hundidos_oponente){
    int cant_barcos_jugador = 0;
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j= 0; j < MAX_COLUMNAS; j++){
            if (jugador_tablero[i][j] == BARCO){
                cant_barcos_jugador ++;
            }
        }
    }
    if (cant_barcos_jugador == 0 || barcos_hundidos_oponente == CANT_BARCOS){
        return ESTADO_TERMINADO;
    }
    return ESTADO_JUGANDO;
}

/*
    Precondiciones: los valores de 'reporte_balas' debe estar inicializado, El archivo 'archivo_reportes' debe estar correctamente abierto en modo escritura.
    Postcondiciones: guarda en el archivo de reportes.csv las categorias (balas, barcos) y su respectivo valor, Devuelve EXITO si se guardaron correctamente los datos. ERROR en caso contrario.
*/
int procesar_reportes(FILE* archivo_reportes, balas_t reporte_balas){
    for (int i = 0; i < CANT_CATEGORIAS; i++){
        if (fprintf(archivo_reportes, FORMATO_ESCRITURA_REPORTES, CATEGORIAS[i], reporte_balas.valores_categoria[i]) < 0){
            return ERROR;
        }
    }
    return EXITO;
}

/*
    Precondiciones: -
    Postcondiciones: Muestra por pantalla el mensaje de GANASTE si es ganador, PERDISTE en caso contrario.
*/
void motrar_ganador(bool es_ganador){
    if (es_ganador){
        printf("================GANASTE==============\n");
    } else {
        printf("==============PERDISTE==============\n");
    }
    printf("===========JUEGO TERMINADO===========\n");
    printf("=====================================\n");
}


int main(int argc, char* argv[]){
    char jugador_tablero[MAX_FILAS][MAX_COLUMNAS];
    char oponente_tablero[MAX_FILAS][MAX_COLUMNAS];
    barco_t barcos_jugador[CANT_BARCOS];

    if (argc != CANT_ARGS_MAX){
        printf ("Error en la cantidad de argumentos ejemplo: ./batalla_naval <barcos.csv> <reportes.csv>\n");
        return ERROR_ARGS;
    }
    
    for (int i = 0; i < CANT_BARCOS; i++) {
        barcos_jugador[i].posiciones = malloc(sizeof(coordenada_t) * MAXIMO_LARGO_BARCO);
        if (!barcos_jugador[i].posiciones){
            printf("Error al reservar memoria para las posiciones de los barcos\n");
            for(int j = 0; j < i; j++){
                free(barcos_jugador[j].posiciones);
            }
            return ERROR;
        }
    }
    
    if (procesar_archivos(barcos_jugador, argv[1]) != 0){
        for (int i = 0; i < CANT_BARCOS; i++) {
            free(barcos_jugador[i].posiciones);
        }
        return ERROR;
    }

    oponente_t *oponente;
    oponente = oponente_crear(barcos_jugador);
    
    balas_t reporte_balas;
    inicializar_reporte(&reporte_balas);
    inicializar_tableros(jugador_tablero, oponente_tablero);
    posicionar_barcos(jugador_tablero, barcos_jugador);
    int barcos_hundidos_oponente = 0;
    int tope_barcos = 5;


    while (estado_juego(jugador_tablero, barcos_hundidos_oponente) != 0){
        system("clear");
        coordenada_t disparo_jugador_oponente;
        coordenada_t disparo_oponente_jugador;

        imprimir_tableros(jugador_tablero, oponente_tablero);
        pedir_jugada(&disparo_jugador_oponente.fila, &disparo_jugador_oponente.columna);

        char impacto_disparo = oponente_recibe_disparo(oponente, disparo_jugador_oponente);
        accionar_disparo_jugador(oponente_tablero, disparo_jugador_oponente, impacto_disparo, &reporte_balas, &barcos_hundidos_oponente);
        
        disparo_oponente_jugador = oponente_realiza_disparo(oponente);
        accionar_disparo_oponente(barcos_jugador, &tope_barcos, jugador_tablero, disparo_oponente_jugador, &reporte_balas);
    }
    
    reporte_balas.valores_categoria[5] = tope_barcos;

    FILE* archivo_reportes = fopen(argv[2], ESCRITURA);
    if(!archivo_reportes){
        printf("Error en abrir el archivo de reportes");
        for (int i = 0; i < CANT_BARCOS; i++) {
            free(barcos_jugador[i].posiciones);
        }   
        oponente_destruir(oponente);
        return ERROR;    
    }

    if (procesar_reportes(archivo_reportes, reporte_balas) == ERROR){
        fclose(archivo_reportes);
        for (int i = 0; i < CANT_BARCOS; i++) {
            free(barcos_jugador[i].posiciones);
        }   
        oponente_destruir(oponente);
        return ERROR;
    }
    fclose(archivo_reportes);
    system("clear");

    bool es_ganador = false;
    if (barcos_hundidos_oponente == CANT_BARCOS){
        es_ganador = true;
    }
    motrar_ganador(es_ganador);

    for (int i = 0; i < CANT_BARCOS; i++) {
        free(barcos_jugador[i].posiciones);
    }   
    oponente_destruir(oponente);
    return EXITO;
}