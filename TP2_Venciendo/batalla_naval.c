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
#define FORMATO_ESCRITURA_REPORTES "%s: %i\n"

const int CANT_ARGS_MAX = 3;
const int POS_ARCH_BARCOS = 1;
const int POS_ARCH_REPORTE = 2;
const int VALOR_NULO = 0;
const int CANT_INI_BARCOS_HUNDIDOS = 0;

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

const int LARGOS_ESPERADOS[CANT_BARCOS] = {2, 3, 3, 4, 5};

const char CATEGORIAS[CANT_CATEGORIAS][MAX_CATEGORIAS] = {"Balas aliadas acertadas", "Balas aliadas erradas", "Balas enemigas acertadas", "Balas enemigas erradas", "Barcos enemigos hundidos", "Barcos aliados sobrevivientes"};
const char* MSJ_BIENVENIDA = "==BIENVENIDO A LA BATALLA NAVAL==";
const char* MSJ_MITABLERO = "============MIS FLOTAS=============";
const char* MSJ_GUIA = "01 02 03 04 05 06 07 08 09 10";
const char* MSJ_OPONENTETABLERO = "========FLOTAS DEL OPONENTE=========";

const char* MSJ_GANASTE = "================GANASTE==============";
const char* MSJ_PERDISTE = "==============PERDISTE==============";
const char* MSJ_JUEGO_TERMINADO = "===========JUEGO TERMINADO===========";
const char* MSJ_DECORADOR = "=====================================";

typedef struct datos {
    int fila;
    int columna; 
    char direccion;
    int largo;
} datos_t;

typedef struct balas {
    int valores_categoria[MAX_VALORES];
} balas_t;


/*
    Precondiciones: los valores de 'barcos_jugador' deben esta incializados o creados previamente, 'num_barco' indica hasta que barco se libera la memoria. 'num' debe estar entre 0-9. 
    Postcondiciones: libera los espacios de memoria utilizados para las posiciones de los barcos.
*/
void liberar_espacio_barcos(barco_t barcos_jugador[CANT_BARCOS], int num_barco){
    for (int i = 0; i < num_barco; i++) {
        free(barcos_jugador[i].posiciones);
    }
}

/*
    Precondiciones: 'barcos_jugador' debe estar inicializado, 'num_barco' debe ser >= 0.
    Postcondiciones:  Devuelve 'true' si se reserva memoria dinamica para las coordenadas del barco. False en caso contrario
*/
bool reservar_espacio(barco_t barcos_jugador[CANT_BARCOS], int num_barco){
    bool reservado = true;
    barcos_jugador[num_barco].posiciones = malloc(sizeof(coordenada_t) * MAXIMO_LARGO_BARCO);
    if (!barcos_jugador[num_barco].posiciones){
        printf("Error al reservar memoria para las posiciones(coordenadas) de los barcos\n");
        liberar_espacio_barcos(barcos_jugador, num_barco);
        reservado = false;
        }
    return reservado;
}


/*
    Precondiciones: los valores de fila dada deben estar entre 0 y 9, dirección debe ser ESTE, OESTE, NORTE ó SUR, el desplazamiento debe estar entre 1-4.
    Postcondiciones: Carga los nuevos valores de 'fila', 'columna' segun la 'direccion' el 'desplazamiento' dado.
*/
void calcular_desplazamiento(int fila_dada, int columna_dada, char direccion, int desplazamiento, int* fila, int* columna){
    *fila = fila_dada;
    *columna = columna_dada;
    if (direccion == NORTE){
        *fila -= desplazamiento;
    } else if (direccion == SUR){
        *fila += desplazamiento;
    } else if (direccion == ESTE){
        *columna += desplazamiento;
    } else if (direccion == OESTE){
        *columna -= desplazamiento;
    }
}

/*
    Precondiciones: los valores de 'datos_archivo' deben estar inicializados.
    Postcondiciones: Devuelve 'true' si no existe superposicion en las posiciones de los 5 barcos. False en caso contrario.
*/
bool validar_superposicion(datos_t datos_archivo[CANT_BARCOS]){
    bool sin_superposicion = true;
    int filas[CANT_BARCOS * MAXIMO_LARGO_BARCO];
    int columnas[CANT_BARCOS * MAXIMO_LARGO_BARCO];
    int celdas_totales = 0;

    for (int i = 0; i < CANT_BARCOS; i++){
        for (int desplazamiento = 0; desplazamiento < datos_archivo[i].largo; desplazamiento++){
            calcular_desplazamiento(datos_archivo[i].fila, datos_archivo[i].columna, datos_archivo[i].direccion, desplazamiento, &filas[celdas_totales], &columnas[celdas_totales]);
            celdas_totales++;
        }
    }

    for (int celda1 = 0; celda1 < celdas_totales; celda1++){
        for (int celda2 = celda1 + 1; celda2 < celdas_totales; celda2++){
            if (filas[celda1] == filas[celda2] && columnas[celda1] == columnas[celda2]){
                sin_superposicion = false;
            }
        }
    }
    return sin_superposicion;
}

/* 
    Precondiciones: Los valores de 'largos_obtenidos' deben estar inicializados.
    Postcondiciones: Devuelve 'true' si el largo de los barcos coincide con los 'LARGOS_ESPERADOS'. False en caso contrario.
*/ 
bool validar_largo_barcos(int largos_obtenidos[CANT_BARCOS]){
    bool largos_validos = true;
    for (int i = 0; i < CANT_BARCOS; i++){
        
        for (int j = 0; j < CANT_BARCOS - 1 - i; j++){
            if (largos_obtenidos[j] > largos_obtenidos[j + 1]){
                int auxiliar = largos_obtenidos[j];
                largos_obtenidos[j] = largos_obtenidos[j + 1];
                largos_obtenidos[j + 1] = auxiliar;
            }
        }
    }
    for (int i = 0; i < CANT_BARCOS; i++){
        if (largos_obtenidos[i] != LARGOS_ESPERADOS[i]){
            largos_validos = false;
        }
    }
    return largos_validos;
}

/*
    Precondiciones: -
    Postcondiciones: Devuelve 'true' si los valores de 'fila_dada' y 'columna_dada'  deben estar entre 1-10. False en caso contrario.
*/
bool es_posicion(int fila_dada, int columna_dada){
    return (fila_dada >= MIN_FILAS && fila_dada <= MAX_FILAS && columna_dada >= MIN_COLUMNAS && columna_dada <= MAX_COLUMNAS);
}

/*
    Precondiciones: -
    Postcondiciones: Devuelve 'true' si el valor de 'direccion_dado' es ESTE, OESTE, NORTE ó SUR. False en caso contrario. 
*/
bool es_direccion(char direccion_dado){
    return (direccion_dado == ESTE || direccion_dado == OESTE || direccion_dado == NORTE || direccion_dado == SUR );
}

/*
    Precondiciones: -
    Postcondiciones: Devuelve 'true' si el valor de 'largo_dado' es >= 0 , <10.False en caso contrario. 
*/
bool es_largo(int largo_dado ){
    return (largo_dado >= MIN_LARGO_BARCO && largo_dado <= MAX_LARGO_BARCO);
}

/*
    Precondiciones: Los valores de 'datos_archivo' deben estar inicializados.
    Postcondiciones: Devuelve 'true' si cada una de las coordenadas de las posiciones de los barcos no se encuentran superpuestas. False en caso contrario.
*/ 
bool validar_posiciones_archivo(datos_t datos_archivo[CANT_BARCOS]){
    int largos_obtenidos[CANT_BARCOS];
    bool posiciones_validas = true;
    int i = 0;
    while (i < CANT_BARCOS && posiciones_validas){
        if( ! es_posicion(datos_archivo[i].fila, datos_archivo[i].columna)){
            posiciones_validas = false;
        } 
        if( ! es_direccion(datos_archivo[i].direccion)){
            posiciones_validas = false;
        }
        if( ! es_largo(datos_archivo[i].largo)){
            posiciones_validas = false;
        }
        largos_obtenidos[i] = datos_archivo[i].largo;
        i++;
    }

    if (!validar_largo_barcos(largos_obtenidos)){
        posiciones_validas = false;
    }

    if (!validar_superposicion(datos_archivo)){
        posiciones_validas = false;
    }

    return posiciones_validas;
}

/*
    Precondiciones: los valores de 'posicion' deben estar inicialzados.
    Postcondiciones: Guarda los valores en 'barco_jugador' de las coordenadas del barco según su largo(2,3,4,5) y direccion(ESTE, OESTE, NORTE, SUR) dados por el parametro 'posicion'.
*/
int guardar_direccion_posicion(barco_t* barco_jugador, datos_t posicion){
    bool direcciones_posiciones_validas = true;
    int posicion_fila = posicion.fila -1;
    int posicion_columna = posicion.columna -1;

    for (int i = 1; i < posicion.largo; i++){
        calcular_desplazamiento(posicion_fila, posicion_columna, posicion.direccion, i, &(*barco_jugador).posiciones[i].fila, &(*barco_jugador).posiciones[i].columna);

        if ( !es_posicion((*barco_jugador).posiciones[i].fila + 1, (*barco_jugador).posiciones[i].columna + 1)){
            direcciones_posiciones_validas = false;
        }
    }

    if(! direcciones_posiciones_validas){
        return ERROR;
    }
    return EXITO;
}

/*
    Precondiciones: Los valores de 'posiciones' deben estar inicializados.
    Postcondiciones: Guarda los valores en 'barco_jugador' de las coordenadas de cada barco dada por el parametro 'posicion' con la primera coordenada(fila,columna), largo(2,3,4,5) y dirección(ESTE, OESTE, NORTE, SUR).
*/
int guardar_barco(datos_t posiciones[CANT_BARCOS], barco_t barcos_jugador[CANT_BARCOS]){
    bool barco_guardado = true;
    for (int i = 0; i < CANT_BARCOS; i++){
        barcos_jugador[i].largo = posiciones[i].largo;
        barcos_jugador[i].posiciones[0].fila = posiciones[i].fila - 1;
        barcos_jugador[i].posiciones[0].columna = posiciones[i].columna - 1;
        int guardar = guardar_direccion_posicion(&barcos_jugador[i], posiciones[i]);
        if (guardar == ERROR){
            barco_guardado = false;
        }
        for (int j = posiciones[i].largo; j < MAXIMO_LARGO_BARCO; j++){
            barcos_jugador[i].posiciones[j].fila = -1;
            barcos_jugador[i].posiciones[j].columna = -1;
        }
    }
    if (!barco_guardado){
        return ERROR;
    }
    return EXITO;
}

/*



PROLEMMMASSSSS-----------------
    Precondiciones: El archivo 'arch_pos_barc' debe estar abierto en modo LECTURA.
    Postcondiciones: Guarda en 'barcos_jugador' todas las posiciones de los barcos recibidos por en el 'archivo_pos_barcos'. Devuelve EXITO si se guardo correctamente las posiciones, ERROR en caso contrario
*/
int guardar_barcos_jugador(barco_t barcos_jugador[CANT_BARCOS], FILE* arch_pos_barc){    
    datos_t datos_archivo[CANT_BARCOS];    
    int i = 0;
    bool espacios_reservados = true;
    int leidos = fscanf(arch_pos_barc, FORMATO_LECTURA_BARCOS, &datos_archivo[i].fila, &datos_archivo[i].columna, &datos_archivo[i].direccion, &datos_archivo[i].largo);
    while (i < CANT_BARCOS && espacios_reservados && leidos == 4){
        if (!reservar_espacio(barcos_jugador, i)){
            espacios_reservados = false;
        }
        i++;
        if (i < CANT_BARCOS){
            leidos = fscanf(arch_pos_barc, FORMATO_LECTURA_BARCOS, &datos_archivo[i].fila, &datos_archivo[i].columna, &datos_archivo[i].direccion, &datos_archivo[i].largo);
        }
    }
    if (!espacios_reservados){
        printf("Error al reservar memoria para las posiciones de los barcos\n");
        return ERROR;
    }

    if (i != CANT_BARCOS){
        printf("Error en la cantidad de barcos en el archivo de barcos.csv (se necesita 5 lineas con los barcos)\n");
        return ERROR;
    }

    if (!validar_posiciones_archivo(datos_archivo)){
        printf("Error en el contenido del archivo barcos.csv\n");
        return ERROR;
    }

    if (guardar_barco(datos_archivo, barcos_jugador) != EXITO ){
        return ERROR;
    }

    return EXITO;
}

/*
    Precondiciones: -
    Postcondiciones: Abre en modo lectura el 'archivo_barcos', guarda los datos en 'barcos_jugador', Devuelve ERROR_APERTURA si no se pudo abrir el archivo, ERROR si no se pudo guardar los barcos. EXITO en caso contrario. 
*/
int procesar_archivo_barcos(barco_t barcos_jugador[CANT_BARCOS], char* archivo_barcos ){
    FILE* arch_pos_barc = fopen(archivo_barcos, LECTURA);
    if (!arch_pos_barc){
        printf("Error al abrir el archivo de posiciones de los barcos(posiblemente no exista)\n");
        return ERROR_APERTURA;
    }
    
    if (guardar_barcos_jugador(barcos_jugador, arch_pos_barc) != EXITO){
        fclose(arch_pos_barc);
        return ERROR;
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
int posicionar_barcos(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS], barco_t barco_jugador[CANT_BARCOS]){
    bool sin_superposicion = true;
    for (int barco = 0; barco < CANT_BARCOS; barco++){
        for (int l = 0; l < barco_jugador[barco].largo; l++){
            int fila = barco_jugador[barco].posiciones[l].fila;
            int columna = barco_jugador[barco].posiciones[l].columna;
            if (jugador_tablero[fila][columna] == BARCO){
                sin_superposicion = false;
            }
            jugador_tablero[fila][columna] = BARCO;
        }
    }
    if (!sin_superposicion){
        return ERROR;
    }
    return EXITO;
}

/*
    Precondiciones: los valores de 'jugador_tablero' deben estar reviamente cargados. 
    Postcondiciones: Imprime por pantalla el tablero del jugador con sus respectivos emojis(NUBES, BARCO, IMPACTO, AGUA) en cada posicion.
*/
void imprimir_tablero_jugador(char jugador_tablero[MAX_FILAS][MAX_COLUMNAS]){
    printf("%s\n   %s\n", MSJ_MITABLERO, MSJ_GUIA);
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
    printf("%s\n   %s\n", MSJ_OPONENTETABLERO, MSJ_GUIA);
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
            } else if (oponente_tablero[i][j] == TOCADO){
                printf("%s ", EMOJI_ATACADO);
            } else if (oponente_tablero[i][j] == HUNDIDO){
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
    printf("%s\n",MSJ_BIENVENIDA);
    imprimir_tablero_jugador(jugador_tablero);
    printf("%s\n", MSJ_DECORADOR);
    imprimir_tablero_oponente(oponente_tablero);
    printf("%s\n", MSJ_DECORADOR);
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

    tablero_oponente[fila_disparo][columna_disparo] = impacto_disparo;
    guardar_disparo(impacto_disparo, &(*reporte_balas), &(*barcos_hundidos_oponente));

}

/*
    Precondiciones: Los valores de 'jugador_tablero' de estar previamente inicializado, el tope debe ser concodante con la cantidad de barcos, los valores de 'fila' y 'columna' deben estar entre 1 y 10
    Postcondiciones: Actualiza el 'tope_barcos' restandolo en una unidad si la 'fila' y 'columna' coincide con la posicion del barco. 
*/
void estado_barco(barco_t barco_jugador[CANT_BARCOS], int *tope_barcos, int fila, int columna){
    bool encontrado = false;
    int i = 0;
    while (i < CANT_BARCOS && !encontrado){
        int indice = 0;
        while (indice < MAXIMO_LARGO_BARCO && !encontrado){
            if (barco_jugador[i].posiciones[indice].fila == fila && barco_jugador[i].posiciones[indice].columna == columna){
                barco_jugador[i].largo--;
                if (barco_jugador[i].largo == 0){
                    (*tope_barcos)--;
                }
                encontrado = true;
            }
            indice++;
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
    bool reporte_valido = true;
    for (int i = 0; i < CANT_CATEGORIAS && reporte_valido; i++){
        if (fprintf(archivo_reportes, FORMATO_ESCRITURA_REPORTES, CATEGORIAS[i], reporte_balas.valores_categoria[i]) < 0){
            reporte_valido = false;
        }
    }
    if (!reporte_valido){
        return ERROR;
    }
    return EXITO;
}

/*
    Precondiciones: los valores de 'archivos_reportes' y 'reporte_balas' deben estar inicializados.
    Postcondiciones: Abre y escribe en 'archivo_reportes' los datos/valores obtenidos de 'reporte_balas', devuelve ERROR_APERTURA si no se pudo abrir el archivo, ERROR si no se pudo escribir en el archivo. EXITO en caso contrario. 
*/
int procesar_archivo_reportes(char* archivo_reportes, balas_t reporte_balas){
    FILE* archivo = fopen(archivo_reportes, ESCRITURA);
    if (!archivo){
        printf("Error en abrir el archivo de reportes\n");
        return ERROR_APERTURA;
    }

    if (procesar_reportes(archivo, reporte_balas) != EXITO){
        fclose(archivo);
        return ERROR;
    }
    fclose(archivo);
    return EXITO;
}

/*
    Precondiciones: -
    Postcondiciones: Muestra por pantalla el mensaje de GANASTE si es ganador, PERDISTE en caso contrario.
*/
void motrar_resultado_juego(int barcos_hundidos_oponente){
    system("clear");
    if (barcos_hundidos_oponente == CANT_BARCOS){
        printf("%s\n", MSJ_GANASTE);
    } else {
        printf("%s\n", MSJ_PERDISTE);
    }
    printf("%s\n%s\n", MSJ_JUEGO_TERMINADO, MSJ_DECORADOR);
}

int main(int argc, char* argv[]){
    char jugador_tablero[MAX_FILAS][MAX_COLUMNAS];
    char oponente_tablero[MAX_FILAS][MAX_COLUMNAS];
    barco_t barcos_jugador[CANT_BARCOS];

    int barcos_hundidos_oponente = CANT_INI_BARCOS_HUNDIDOS;
    int tope_barcos = CANT_BARCOS;

    if (argc != CANT_ARGS_MAX){
        printf ("Error en la cantidad de argumentos ejemplo: ./batalla_naval <barcos.csv> <reportes.csv>\n");
        return ERROR_ARGS;
    }
    
    if (procesar_archivo_barcos(barcos_jugador, argv[POS_ARCH_BARCOS]) != 0){
        liberar_espacio_barcos(barcos_jugador, CANT_BARCOS);
        return ERROR;
    }

    oponente_t *oponente;
    oponente = oponente_crear(barcos_jugador);
    
    balas_t reporte_balas;
    inicializar_reporte(&reporte_balas);
    inicializar_tableros(jugador_tablero, oponente_tablero);
    
    if (posicionar_barcos(jugador_tablero, barcos_jugador) != EXITO){
        printf("Error en superposicion de barcos en el archivo\n");
        liberar_espacio_barcos(barcos_jugador, CANT_BARCOS);
        oponente_destruir(oponente);
        return ERROR;
    }
    while (estado_juego(jugador_tablero, barcos_hundidos_oponente) != 0){
        system("clear");
        coordenada_t disparo_jugador_oponente;
        coordenada_t disparo_oponente_jugador;

        imprimir_tableros(jugador_tablero, oponente_tablero);
        pedir_jugada(&disparo_jugador_oponente.fila, &disparo_jugador_oponente.columna);

        char impacto_disparo = oponente_recibe_disparo(oponente, disparo_jugador_oponente);
        accionar_disparo_jugador(oponente_tablero, disparo_jugador_oponente, impacto_disparo, &reporte_balas, &barcos_hundidos_oponente);
        
        if (barcos_hundidos_oponente < CANT_BARCOS){
            disparo_oponente_jugador = oponente_realiza_disparo(oponente);
            accionar_disparo_oponente(barcos_jugador, &tope_barcos, jugador_tablero, disparo_oponente_jugador, &reporte_balas);
        }
    }
    reporte_balas.valores_categoria[5] = tope_barcos;
    
    if (procesar_archivo_reportes(argv[POS_ARCH_REPORTE], reporte_balas) != EXITO){
        liberar_espacio_barcos(barcos_jugador, CANT_BARCOS); 
        oponente_destruir(oponente);
        return ERROR;
    }
    motrar_resultado_juego(barcos_hundidos_oponente);
    liberar_espacio_barcos(barcos_jugador, CANT_BARCOS);
    oponente_destruir(oponente);
    return EXITO;
}