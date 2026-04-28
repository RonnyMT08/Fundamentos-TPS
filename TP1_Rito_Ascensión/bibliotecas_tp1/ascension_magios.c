//gcc juego.c ascension_magios.c utiles.o -o juego -std=c99 -Wall -Wconversion -Werror -lm

#include "ascension_magios.h"
#include "utiles.h"
#include <stdio.h>
#include <stdlib.h> // Para usar rand, limpiar terminal, etc.
#include <time.h>   // Para obtener una semilla desde el reloj

#define MAX_CARACTERES 100

const int NIVEL_INICIO = 1;
const int CORRECCION_NIVEL = -1; //SI EL NIVEL INICIO ES 0 -> CORERCCION =0 (1)-->1

const int VIDAS_INICIALES = 5;
const int ANTORCHAS_INICIALES = 5;
const int RANGO_MANHATTAN = 3;

const int HECHIZOS_REVELADORES_INICIALES = 5;
const int PERGAMINOS_INICIALES = 3;

const int PIEDRAS_CASTIGO_INCIALES = 10;

const char HOMERO = 'H';

const char PARED = 'X';
const char CAMINO = '-';
const char VACIO = ' ';

const char RUNA = 'U';
const char ALTAR = 'A';

const char PERGAMINO = 'P';
const char TOTEM = 'T';

const char PIEDRA_CASTIGO = 'R';
const char CATAPULTA = 'F';

const char* MSJ_DECORADOR = "-----------------------------------------------------------";
const char* MSJ_BIENVENIDA = "-        Bienvenido al juego de ascencion de magios       -";

const char ARRIBA = 'W';
const char ABAJO = 'S';
const char DERECHA = 'D';
const char IZQUIERDA = 'A';

const char ANTORCHA = 'L';
const char HECHIZO_REVELADOR = 'H';

/* 
* Pre condiciones: El numero 'maximo' debe ser mayor al 'minimo'.
* Post condiciones: Devuelve un numero de manera aleatorio entre el valor 'minimo' y el 'maximo' valor (inclusive).
*/
int numero_aleatorio(int minimo, int maximo){
    int numero = rand() % (maximo - minimo + 1) + minimo;
    return numero;
}

/*
* Pre dcondiciones: -
* Post condiciones: Inicializa el nivel de inicio del juego, tope de niveles.
*/
void inicializar_niveles(juego_t* juego){
    (*juego).nivel_actual = NIVEL_INICIO;
    (*juego).tope_niveles = 2;
    (*juego).camino_visible = true;    
}

/* 
* Pre condiciones: Nivel actual dado debe ser 0, tope_niveles >= 0
* Post condiciones: Inicializa las paredes, caminos cada uno de los niveles.   
*/
void inicializar_estructura(nivel_t niveles[MAX_NIVELES], int nivel_actual, int tope_niveles){
    int tope_paredes = 0;
    int tope_caminos = 0; 

    for (int i = nivel_actual; i < tope_niveles; i++){
        
        niveles[i].tope_paredes = tope_paredes;
        niveles[i].tope_camino = tope_caminos;
        
        obtener_mapa (niveles[i].paredes, &niveles[i].tope_paredes, niveles[i].camino, &niveles[i].tope_camino, i+1);
    }
}

/*
* Pre condiciones: La 'fil_camino', 'col_camino' son coordenadas de la primera posicion del vector de caminos
* Post condiciones: Inicializa la 'posicion' (fila, columna) del personaje igual a la posicion 'fil_camino', 'col_camino'.
*/
void inicializar_posicion_personaje(coordenada_t* posicion, int fil_camino, int col_camino){
    (*posicion).fil = fil_camino;
    (*posicion).col = col_camino;
}

/*
* Pre condiciones: -
* Post condiciones: Inicializa la cantidad de herramientas(hechizos, antorchas) con la que el personaje inicia el juego. Inicializa la antorcha encendida con un valor false. 
*/
void inicializar_herramientas_personaje(int* hechizos_reveladores, int* antorchas, bool* antorcha_encendida, bool* recolecto_pergamino){    
    (*hechizos_reveladores) = HECHIZOS_REVELADORES_INICIALES;
    (*antorchas) = ANTORCHAS_INICIALES;
    (*antorcha_encendida) = false;
    (*recolecto_pergamino) = false;
}

/*
* Pre condiciones: EL vector de struct de coordenda_t 'posicion' de los caminos debe estar inicializada previamente.
* Post condiciones: Inicializa la cantidad inicial de vidas, posicion y herramientas del personaje.
*/
void inicializar_homero (personaje_t *personaje, coordenada_t posicion){
    
    (*personaje).vidas_restantes = VIDAS_INICIALES;

    int fil_camino = posicion.fil;
    int col_camino = posicion.col;
    inicializar_posicion_personaje(&(*personaje).posicion, fil_camino, col_camino);
    inicializar_herramientas_personaje(&(*personaje).hechizos_reveladores, &(*personaje).antorchas, &(*personaje).antorcha_encendida, &(*personaje).recolecto_pergamino);
}

/*
* Pre condiciones: El vector de caminos debe estar inicializado. el valor de fila debe estar entre 0-19 y columna entre 0-29.
* Post condiciones: Devuelve true si la 'fila', 'columna' coincide con las coordenda (fila, columna) del algun camino. False en caso contrario
*/
bool es_posicion_camino(coordenada_t camino[], int tope_camino, int fila, int columna){
    bool en_camino = false;
    int posicion_camino = 0;
    while(!en_camino && (posicion_camino < tope_camino)){
        if(camino[posicion_camino].fil == fila && camino[posicion_camino].col == columna){
            en_camino = true;
        } else {
            posicion_camino ++;
        }
    }
    return en_camino;
}

/*
* Pre condiciones: los niveles deben estar inicializados, tope_niveles < 0.
* Post condiciones: Inicializa la posicion(fila, columna) aleatoria del pergamino para cada uno de los tres niveles.
*/
void inicializar_pergaminos(nivel_t niveles[MAX_NIVELES], int tope_niveles){
    
    for (int i = 0; i < tope_niveles; i++){

        int tope_camino = niveles[i].tope_camino;
        int posicion_pergamino = numero_aleatorio(1, tope_camino-2);

        int fil_pergamino = niveles[i].camino[posicion_pergamino].fil;
        int col_pergamino = niveles[i].camino[posicion_pergamino].col;
        
        while (!es_posicion_camino(niveles[i].camino, tope_camino, fil_pergamino, col_pergamino)){
            posicion_pergamino = numero_aleatorio(1, tope_camino-2);
            
            fil_pergamino = niveles[i].camino[posicion_pergamino].fil;
            col_pergamino = niveles[i].camino[posicion_pergamino].col;
        }

        niveles[i].pergamino.fil = fil_pergamino;
        niveles[i].pergamino.col = col_pergamino;

    }
}

/*
* Pre condiciones: El pergamino debe estar inicializado. fila debe estar entre (0-19), columna debe estar entre (0-29). 
* Post condiciones Devuelve true si la posicion (fila, columna) del pergamino coincide con la posicion 'fila', 'columna' dados por parametro. False en caso contrario. 
*/
bool es_posicion_pergamino(coordenada_t pergamino, int fila, int columna){
    return(fila == pergamino.fil && columna == pergamino.col);
}

/*
* Pre condicones: los niveles deben estar inicializados, tope_niveles > 0.
* Post condicioes: Inicializa los 5 totems repatidos aleatoriamente en cada uno de los tres niveles, los totems se encuentran en el camino (excluyendo el inicio, final y la posicion del pergamino).  
*/
void inicializar_herramientas(nivel_t niveles[MAX_NIVELES], int tope_niveles){
   
    for (int i = 0; i < tope_niveles; i++){
        int tope_camino = niveles[i].tope_camino;
        
        niveles[i].tope_herramientas = 0;

        for (int j = 0; j < 5; j++){
            int posicion_camino = numero_aleatorio(1, tope_camino-2); // -2 por que en el tope no hay nada, y ultima es el altar
            int fil_totem = niveles[i].camino[posicion_camino].fil;
            int col_totem = niveles[i].camino[posicion_camino].col;

            while (es_posicion_pergamino(niveles[i].pergamino, fil_totem, col_totem)){
                posicion_camino = numero_aleatorio(1, tope_camino-2); // -2 por que en el tope no hay nada, y ultima es el altar
                fil_totem = niveles[i].camino[posicion_camino].fil;
                col_totem = niveles[i].camino[posicion_camino].col;
            }

            niveles[i].herramientas[j].tipo = TOTEM; //existen 3 herramientas echizo, totems, antorchas
            niveles[i].herramientas[j].posicion.fil = fil_totem;
            niveles[i].herramientas[j].posicion.col = col_totem;
            niveles[i].tope_herramientas++;
        }
    }
}

/*
* Pre condiciones: el vector de herramientas debe estar inicializado, el tope_herramientas > 0, fila debe estar entre(0-19), columna debe estar entre (0-29).
* Post condiciones: devuelve true si la fila, columna coincide con una posicion de la herramienta (fila, columna). False en caso contrario
*/
bool es_posicion_herramienta(objeto_t herramienta[], int tope_herramientas, int fila, int columna){
    
    bool en_herramienta = false;
    int posicion_herramienta = 0;
    while(!en_herramienta && posicion_herramienta < tope_herramientas){

        if(herramienta[posicion_herramienta].posicion.fil == fila && herramienta[posicion_herramienta].posicion.col == columna){
            en_herramienta = true;
        } else {
            posicion_herramienta ++;
        }
    }
    return en_herramienta;
}

/*
* Pre condiciones: el vector de niveles debe estar inicializado, 'cantidad_obstaculos' debera ser >= 0. tope_niveles debe ser > 0. 
* Post condiciones: Inicializa una 10 piedras en posicion aleatoria(perteneciente al camino) sin superponer a otro objeto por cada uno de los tres niveles, carga el tope de los obstaculos por nivel.
*/
void inicializar_piedras_castigo(nivel_t niveles[MAX_NIVELES], int tope_niveles){

    for (int i = 0; i < tope_niveles; i++){
        niveles[i].tope_obstaculos = 0;
        int tope_camino = niveles[i].tope_camino;

        for (int j = 0; j < PIEDRAS_CASTIGO_INCIALES; j++){
            int posicion_camino =  numero_aleatorio(1, (tope_camino-2));
            
            int fil_piedra = niveles[i].camino[posicion_camino].fil;
            int col_piedra = niveles[i].camino[posicion_camino].col;

            while (es_posicion_pergamino(niveles[i].pergamino, fil_piedra, col_piedra) && es_posicion_herramienta(niveles[i].herramientas, niveles[i].tope_herramientas, fil_piedra, col_piedra)){
                posicion_camino = numero_aleatorio(1, tope_camino-2);
                fil_piedra = niveles[i].camino[posicion_camino].fil;
                col_piedra = niveles[i].camino[posicion_camino].col;
            }

            niveles[i].obstaculos[j].tipo = PIEDRA_CASTIGO;
            niveles[i].obstaculos[j].posicion.fil = fil_piedra;
            niveles[i].obstaculos[j].posicion.col = col_piedra;
            niveles[i].tope_obstaculos ++;
        }
    }
}

/*
* Pre condiciones: nivel debe estar inicializado, tope_paredes debe ser un entero > 0 , fila debe estar entre(0-19), columna debe estar entre (0-29).
* Post condiciones: devuelve true si la fila y columna coinciden con una posicion de la herramienta (fila, columna). False en caso contrario
*/
bool es_posicion_pared(nivel_t nivel, int tope_paredes, int fila, int columna){
    bool en_pared = false;
    int posicion_pared = 0;

    while(!en_pared && posicion_pared < tope_paredes){
        if(nivel.paredes[posicion_pared].fil == fila && nivel.paredes[posicion_pared].col == columna){
            en_pared = true;
        } else {
            posicion_pared ++;
        }
    }
    return en_pared;
}

/*
* Pre condiciones: nivel debe estar inicializado, tope_niveles debe ser > 0;
* Post condiciones: Inicializa la posicion de una catapulta por nivel, la posicion no pertenece a una posicion del camino y distinto de la posicion de una pared. 
*/
void inicializar_catapultas(nivel_t niveles[MAX_NIVELES], int tope_niveles){
    
    for(int i = 0; i < tope_niveles; i++){
        int tope_obstaculos = niveles[i].tope_obstaculos;

        int fil_catapulta = numero_aleatorio(0, MAX_FILAS-1);
        int col_catapulta = numero_aleatorio(0, MAX_COLUMNAS-1);

        while (es_posicion_camino(niveles[i].camino, niveles[i].tope_camino, fil_catapulta, col_catapulta) || es_posicion_pared(niveles[i], niveles[i].tope_paredes, fil_catapulta, col_catapulta)){

            fil_catapulta = numero_aleatorio(0, MAX_FILAS-1);
            col_catapulta = numero_aleatorio(0, MAX_COLUMNAS-1);
        }
        niveles[i].obstaculos[tope_obstaculos].posicion.fil = fil_catapulta;
        niveles[i].obstaculos[tope_obstaculos].posicion.col = col_catapulta;
        niveles[i].obstaculos[tope_obstaculos].tipo = CATAPULTA;

        niveles[i].tope_obstaculos ++;
    }
}

/*
* Pre condiciones: el vector de niveles debe estar inicializado, tope_niveles debe ser > 0. 
* Post condciones: Inicializa los obstaculos (piedras y catapultas) en cada uno de los niveles.
*/
void inicializar_obstaculos(nivel_t niveles[MAX_NIVELES], int tope_niveles){

    inicializar_piedras_castigo(niveles, tope_niveles);
    inicializar_catapultas(niveles, tope_niveles);
}

/////////////////////////////////////////////////////////
void inicializar_juego(juego_t* juego) {
    
    inicializar_niveles( &(*juego));

    inicializar_estructura( (*juego).niveles, (*juego).nivel_actual+CORRECCION_NIVEL, (*juego).tope_niveles);

    inicializar_homero( &(*juego).homero, (*juego).niveles[0].camino[0]);

    inicializar_pergaminos( (*juego).niveles, (*juego).tope_niveles);

    inicializar_herramientas( (*juego).niveles, (*juego).tope_niveles);

    inicializar_obstaculos((*juego).niveles, (*juego).tope_niveles);
}
//////////////////////////////////////////////////////////

///////////////////////////////////////////////
void cambiar_nivel(juego_t* juego) {    
    (*juego).nivel_actual ++;
}
//////////////////////////////////////////////////





/*
* Pre condiciones: -
* Post condicones: devuelve true si el movimiento es S, D, A ó W, False en caso contrario.
*/
bool es_movimiento(char movimiento){
    return (movimiento == DERECHA || movimiento == ABAJO || movimiento == IZQUIERDA || movimiento == ARRIBA );
}

/*
* Pre condicones: -
* Post condiciones: devuelve true si el 'movimiento' es H ó L. False en caso contrario. 
*/
bool es_herramienta(char movimiento){
    return (movimiento == HECHIZO_REVELADOR || movimiento == ANTORCHA);
}

/*
* Pre condiciones: fil_movimiento, col_movimiento son parametros que se actualizaran.
* Post condiciones: Carga el nuevo valor del movimiento en (fil_movimiento, col_movimiento) correspondiente.  
*/
void valorar_movimiento(char movimiento, int* fil_movimiento, int* col_movimiento){
    switch (movimiento){
        case 'W':
            (*fil_movimiento) = -1;
            break;
        case 'S':
            (*fil_movimiento) = +1;
            break;
        case 'A':
            (*col_movimiento) = -1;
            break;
        case 'D':
            (*col_movimiento) = +1;
            break;   
        default:
            break;
    }
}

/*
* Pre condiciones: la 'posicion' del personaje debe estar inicializado. 
* Post condiciones: actualiza la posicion del personaje deacuerdo a los valores del movimiento (fil_movimineto y col_movimientos) dados.
*/
void posicionar_personaje(coordenada_t* posicion, int fil_movimiento, int col_movimiento){
    (*posicion).fil += fil_movimiento;
    (*posicion).col += col_movimiento;
}

/*
* Pre condiciones: herramientas debe estar inicializado, fil_movimiento debe estar entre (0-19) y col_movimiento debe estar entre (0-29)  
* Post condiciones: devuelve el indice de la posicion en la que se encuentra la herramienta.
*/
int busca_posicion_herramienta(objeto_t herramientas[], int tope_herramientas, int fil_movimiento, int col_movimiento){
    int posicion = 0;
    bool posicion_encontrada = false;
    
    while(!posicion_encontrada && posicion < tope_herramientas){

        if (herramientas[posicion].posicion.fil == fil_movimiento && herramientas[posicion].posicion.col == col_movimiento){
            posicion_encontrada = true;
        } else {
            posicion ++;
        }
    }
    return posicion;
}

/*
* Pre condiciones: vidas_restantes y herramientas deben estar inicializados.
* Post condiciones: incrementa en una unidad las vidas_restantes al personaje. 
*/
void tomar_herramienta(int *vidas_restantes, objeto_t herramienta){
    if (herramienta.tipo == TOTEM){
        (*vidas_restantes) ++;
    }
}

/*
* Pre condiciones: el vector herramientas debe estar inicializado, tope_herramientas y posicion_herramienta debera ser >= 0.   
* Post condiciones: actualiza un vector de herramientas, tope_herramientas.
*/
void eliminar_herramienta(objeto_t herramientas[], int* tope_herramientas, int posicion_herramienta){
    for (int i = posicion_herramienta; i < (*tope_herramientas)-1 ; i++){
        herramientas[i].tipo = herramientas[i+1].tipo;
        herramientas[i].posicion.fil = herramientas[i+1].posicion.fil;
        herramientas[i].posicion.col = herramientas[i+1].posicion.col;
    }
    (*tope_herramientas) -= 1;
}

/*
* Pre condiciones: la posicion de la runa debe estar inicializado, fil_movimiento debe estar entre (0-19) y col_movimiento debe estar entre (0-29)  
* Post condiciones: devuelve true si la posicion del personaje es igual a la posicion de la runa.
*/
bool es_runa(coordenada_t posicion, int fil_personaje, int col_personaje){
    return (posicion.fil == fil_personaje && posicion.col == col_personaje);
}

/*
* Pre condiciones: coordenada debe estar inicializado, fil_movimiento debe estar entre (0-19) y col_movimiento debe estar entre (0-29)  
* Post condiciones: devuelve el indice de la posicion en la que se encuentra el camino.
*/
int busca_posicion_camino(coordenada_t coordenada[], int tope_caminos, int fil_camino, int col_camino){
    int posicion = 0;
    bool posicion_encontrada = false;
    
    while(!posicion_encontrada && posicion < tope_caminos){

        if (coordenada[posicion].fil == fil_camino && coordenada[posicion].col == col_camino){
            posicion_encontrada = true;
        } else {
            posicion ++;
        }
    }
    return posicion;
}

/*
* Pre condiciones: el vector de caminos debe estar inicializado, tope_caminos y posicion del camino deben ser > 0.
* Post condiciones: elimina el elemento del vector de caminos segun la posicion_camino dado, actualiza el valor del tope.
*/
void eliminar_camino(coordenada_t caminos[], int* tope_caminos, int posicion_camino){
    for (int i = posicion_camino; i < (*tope_caminos)-1 ; i++){
        caminos[i].fil = caminos[i+1].fil;
        caminos[i].col = caminos[i+1].col;
    }
    (*tope_caminos) -= 1;
}

/*
* Pre condiciones: nivel debe estar inicializado, tope_caminos debe ser > 0.
* Post condiciones: actualiza el vector de caminos eliminando un camino de manera aleatoria.
*/
void lanzar_bola_fuego(nivel_t* nivel, int* tope_caminos){
    int posicion_camino = numero_aleatorio(0, (*tope_caminos)-1);

    int fil_camino = (*nivel).camino[posicion_camino].fil;
    int col_camino = (*nivel).camino[posicion_camino].col;

    while (es_posicion_herramienta((*nivel).herramientas, (*nivel).tope_herramientas, fil_camino, col_camino) || es_posicion_pergamino((*nivel).pergamino, fil_camino, col_camino)){
        posicion_camino = numero_aleatorio(0, (*tope_caminos)-1);

        fil_camino = (*nivel).camino[posicion_camino].fil;
        col_camino = (*nivel).camino[posicion_camino].col;
    }

    int indice_posicion_camino;
    indice_posicion_camino = busca_posicion_camino((*nivel).camino, (*nivel).tope_camino, fil_camino, col_camino); ///!!!!!!!!!!!!!!!!
    eliminar_camino((*nivel).camino, &(*nivel).tope_camino, indice_posicion_camino);/////////!!!!!!!!!
}

/* 
* Pre condiciones: el vector de obstaculos debe estar incializado, tope_obtaculos debe ser > 0, fila entre 0 - 19 y columna entre 0 - 29. 
* Post condiciones: devuelve true si la fila, columana coinciden con la posicion (fila, columna) de alguno de los obstaculos. False en caso contrario.
*/
bool es_posicion_obstaculos (objeto_t obstaculos[], int tope_obstaculos, int fila, int columna){
    
    bool en_obstaculo = false;
    int posicion_obstaculo = 0;
    while(!en_obstaculo && posicion_obstaculo < tope_obstaculos){

        if(obstaculos[posicion_obstaculo].posicion.fil == fila && obstaculos[posicion_obstaculo].posicion.col == columna){
            en_obstaculo = true;
        } else {
            posicion_obstaculo ++;
        }
    }
    return en_obstaculo;
}

/*
* Pre condiciones: el nivel y el vecotor de caminos deben estar inicializados, la posicion_pergamino >= 0, tope_camino debera ser > 0, 
* Post condiciones: actualiza la posicion del pergamino en una posicion aleatoria, sobre el camino (sin superponerse a cualquier obstaculo u herramienta).
*/
void posicionar_pergamino(nivel_t nivel, coordenada_t* posicion_pergamino, int tope_camino, coordenada_t camino[]){
    
    int posicion_camino = numero_aleatorio(1, (tope_camino-2));

    int fil_pergamino = camino[posicion_camino].fil;
    int col_pergamino = camino[posicion_camino].col;
    while (es_posicion_herramienta(nivel.herramientas, nivel.tope_herramientas, fil_pergamino, col_pergamino) || es_posicion_obstaculos(nivel.obstaculos, nivel.tope_obstaculos, fil_pergamino, col_pergamino)){
        posicion_camino = numero_aleatorio(1, (tope_camino-2));
    
        fil_pergamino = camino[posicion_camino].fil;
        col_pergamino = camino[posicion_camino].col;
    }

    (*posicion_pergamino).fil = fil_pergamino;
    (*posicion_pergamino).col = col_pergamino;

}

/*
* Pre condiciones: juego debe ser inicializado, movimiento tiene que ser A,S,D o W
* Post condciciones: actualiza la nueva posicion del jugador (siendo este dentro del camino).
*/
void dirigir_movimiento(juego_t *juego, int fil_movimiento, int col_movimiento){
    int nivel_actual = (*juego).nivel_actual+CORRECCION_NIVEL;

    int fil_personaje = (*juego).homero.posicion.fil;
    int col_personaje = (*juego).homero.posicion.col;

    if (es_posicion_camino((*juego).niveles[nivel_actual].camino, (*juego).niveles[nivel_actual].tope_camino, fil_personaje+fil_movimiento, col_personaje+col_movimiento)){
        posicionar_personaje(&(*juego).homero.posicion, fil_movimiento, col_movimiento);

        if (es_posicion_pergamino((*juego).niveles[nivel_actual].pergamino, fil_personaje+fil_movimiento, col_personaje+col_movimiento)){
            (*juego).camino_visible = true;
            (*juego).homero.recolecto_pergamino = true;
            printf("tienes un pergamino");

        } else {
            (*juego).camino_visible = false;
        }
        
        if (es_posicion_herramienta((*juego).niveles[nivel_actual].herramientas, (*juego).niveles[nivel_actual].tope_herramientas, fil_personaje+fil_movimiento, col_personaje+col_movimiento)){
            int indice_posic_herramienta;

            indice_posic_herramienta = busca_posicion_herramienta((*juego).niveles[nivel_actual].herramientas,(*juego).niveles[nivel_actual].tope_herramientas, fil_personaje+fil_movimiento, col_personaje+col_movimiento);
            tomar_herramienta(&(*juego).homero.vidas_restantes, (*juego).niveles[nivel_actual].herramientas[indice_posic_herramienta]);
            eliminar_herramienta((*juego).niveles[nivel_actual].herramientas, &(*juego).niveles[nivel_actual].tope_herramientas, indice_posic_herramienta);        
        }
        
        if (es_runa((*juego).niveles[nivel_actual].camino[0], fil_personaje+fil_movimiento, col_personaje+col_movimiento)){

            lanzar_bola_fuego(&(*juego).niveles[nivel_actual], &(*juego).niveles[nivel_actual].tope_camino);
            (*juego).camino_visible = true;
        }
        
        (*juego).homero.antorcha_encendida = false;
        
        if (es_posicion_obstaculos((*juego).niveles[nivel_actual].obstaculos, (*juego).niveles[nivel_actual].tope_obstaculos, fil_personaje+fil_movimiento, col_personaje+col_movimiento)){
            int indice_posic_obstaculo;

            indice_posic_obstaculo = busca_posicion_herramienta((*juego).niveles[nivel_actual].obstaculos, (*juego).niveles[nivel_actual].tope_obstaculos, fil_personaje+fil_movimiento, col_personaje+col_movimiento);
            posicionar_pergamino((*juego).niveles[nivel_actual], &(*juego).niveles[nivel_actual].pergamino, (*juego).niveles[nivel_actual].tope_camino, (*juego).niveles[nivel_actual].camino );
            eliminar_herramienta((*juego).niveles[nivel_actual].obstaculos, &(*juego).niveles[nivel_actual].tope_obstaculos, indice_posic_obstaculo);
            (*juego).homero.recolecto_pergamino = false;
            printf("EL PERGAMINO CAMBIO DE POSICION, RECOGELO!");
        }
    
    } else if (es_posicion_pared((*juego).niveles[nivel_actual], (*juego).niveles[nivel_actual].tope_paredes, fil_personaje+fil_movimiento, col_personaje+col_movimiento)){
        printf("--- TE CHOCASTE CON UNA PARED ---");

    } else {
        (*juego).homero.vidas_restantes -=1;
    
    }

}


//////////////////////////////////////////////////
void realizar_jugada(juego_t* juego, char movimiento){
    int fil_movimiento = 0;
    int col_movimiento = 0;
    int nivel_actual = (*juego).nivel_actual+CORRECCION_NIVEL;

    if (es_movimiento(movimiento)){
        valorar_movimiento(movimiento, &fil_movimiento, &col_movimiento);
        dirigir_movimiento(&(*juego), fil_movimiento, col_movimiento);

    } else if (es_herramienta(movimiento)){

        if ((movimiento == HECHIZO_REVELADOR) && ((*juego).homero.hechizos_reveladores > 0) ){
            (*juego).camino_visible = true;
            (*juego).homero.hechizos_reveladores -= 1;
            lanzar_bola_fuego(&(*juego).niveles[nivel_actual], &(*juego).niveles[nivel_actual].tope_camino);
        } 
 
        if ((movimiento == ANTORCHA) && ((*juego).homero.antorchas > 0)){
            (*juego).homero.antorcha_encendida = true;
            (*juego).homero.antorchas -= 1;
        }
    }
}
////////////////////////////////////////////////////

/*
* Pre condiciones: -
* Post condiciones: limpia la pantalla y imprime el mensaje de bienvenida al usuario.
*/
void mostrar_bienvenida(){
    system("clear");
    printf("%s\n%s\n%s\n", MSJ_DECORADOR, MSJ_BIENVENIDA, MSJ_DECORADOR);
}

/*
* Pre condiciones: la matriz del mapa, vector de paredes deben estar inicializados (sin contenido), el tope de paredes debe ser > 0.
* Post condiciones: carga las posiciones (fila, columna) de las paredes a la matriz del mapa.
*/
void agregar_paredes(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t paredes[MAX_PAREDES], int tope_paredes){
    for (int i = 0; i < tope_paredes; i++){
        int fil_pared = paredes[i].fil;
        int col_pared = paredes[i].col;
        mapa[fil_pared][col_pared] = PARED;
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de caminos deben estar inicializados, tope_camino debe ser >0.
* Post condiciones: carga las posiciones(fila, columna) de los caminos a la matriz del mapa.
*/
void agregar_camino(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino){
    for (int i = 0; i < tope_camino; i++){
        int fil_camino = camino[i].fil;
        int col_camino = camino[i].col;

        mapa[fil_camino][col_camino] = CAMINO;
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de caminos deben estar inicializados, tope_camino debe ser > 0, la posicion del personaje debe ser dentro de los limites de la matriz mapa.  
* Post condiciones: carga las posiciones(fila, columna) de los caminos a la matriz del mapa con un RANGO_MANHATTAN = 3 o el valor declarado.
*/
void agregar_camino_manhattan(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino, coordenada_t posicion){
    int fil_homero = posicion.fil;
    int col_homero = posicion.col;
    int rango = RANGO_MANHATTAN;

    for (int i = 0; i < tope_camino; i++){

        if(((fil_homero - camino[i].fil) <= rango) && ((col_homero - camino[i].col <= rango)) ){
            
            if(((fil_homero - camino[i].fil) >= -rango) && ((col_homero - camino[i].col >= -rango))){
                int fil_coordenada = fil_homero - camino[i].fil;
                int col_coordenada = col_homero - camino[i].col;
            
                if ((fil_coordenada + col_coordenada) <= rango && (fil_coordenada - col_coordenada) >= -rango ){

                    if ((fil_coordenada - col_coordenada) <= rango && (fil_coordenada + col_coordenada) >= -rango ){
                        mapa[camino[i].fil][camino[i].col] = CAMINO;
                    }
                }                    
            } 
        } 
    }
}

/*
* Pre condiciones: la matriz de mapa, el personaje deben ser inicializado.
* Post condiciones: carga en la posicion de coordendas (fila, columna) el personaje segun su ubicacion. 
*/
void agregar_personaje(char mapa[MAX_FILAS][MAX_COLUMNAS], personaje_t homero){
    int fil_homero = homero.posicion.fil;
    int col_homero = homero.posicion.col;

    mapa[fil_homero][col_homero] = HOMERO;
}

/*
* Pre condiciones: la matriz del mapa, vector de niveles deben estar inicializado, el nivel_actual debe ser >= 0.
* Post condicones: carga en la posicion de coordendas de la matriz mapa los items (la runa, altar, pergamino) del nivel_actual.  
*/
void agregar_items(char mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t niveles[], int nivel_actual){
    int tope_camino = niveles[nivel_actual].tope_camino;

    int camino_inicial_fil = niveles[nivel_actual].camino[0].fil;
    int camino_inicial_col = niveles[nivel_actual].camino[0].col;

    mapa [camino_inicial_fil][camino_inicial_col] = RUNA;

    int camino_final_fil = niveles[nivel_actual].camino[tope_camino-1].fil;
    int camino_final_col = niveles[nivel_actual].camino[tope_camino-1].col;

    mapa [camino_final_fil][camino_final_col] = ALTAR;

    int fil_pergamino = niveles[nivel_actual].pergamino.fil;
    int col_pergamino = niveles[nivel_actual].pergamino.col;

    mapa [fil_pergamino][col_pergamino] = PERGAMINO;
}

/*
* Pre condiciones: la matriz del mapa, vector de niveles deben estar inicializados, nivel_actual debe ser >= 0, tope > 0;
* Post condiciones: agrega en las coordenadas del mapa, los obstaculos(catapulta, piedra del catigo) del nivel_actual.
*/
void agregar_obstaculos(char mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t niveles[], int nivel_actual, int tope_obstaculos){
    for (int i = 0; i < tope_obstaculos; i++){
        int piedra_fil = niveles[nivel_actual].obstaculos[i].posicion.fil;
        int piedra_col = niveles[nivel_actual].obstaculos[i].posicion.col; 
        if (niveles[nivel_actual].obstaculos[i].tipo == PIEDRA_CASTIGO){
            mapa[piedra_fil][piedra_col] = PIEDRA_CASTIGO; 
        } else if (niveles[nivel_actual].obstaculos[i].tipo == CATAPULTA) {
            mapa[piedra_fil][piedra_col] = CATAPULTA;
        }
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de niveles deben estar inicializados, nivel_actual debe ser >=0, tope_herramientas > 0;
* Post condiciones: agrega a la matriz del mapa en las coordenadas de la posicion (fila, columna) los herramientas segun el nivel_actual.
*/
void agregar_herramientas(char mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t niveles[], int nivel_actual, int tope_herramientas){
    
    // Totems
    for (int i = 0; i < tope_herramientas; i++){
        int totem_fil = niveles[nivel_actual].herramientas[i].posicion.fil;
        int totem_col = niveles[nivel_actual].herramientas[i].posicion.col;
        mapa[totem_fil][totem_col] = TOTEM; /////AREGLAR
    }
}


/*
* Pre condiciones: la matriz del mapa y el juego deben estar incializados, nivel_actual debe ser >= 0.
* Post condiciones: crea el mapa del juego que contiene (paredes, caminos, personaje, pergaminos, herramientas) segun corresponda el nivel.
*/
void crear_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego, int nivel_actual){
    
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            mapa[i][j] = VACIO;
        }
    }
  
    agregar_paredes(mapa, juego.niveles[nivel_actual].paredes, juego.niveles[nivel_actual].tope_paredes);

    if (juego.camino_visible){
        agregar_camino(mapa, juego.niveles[nivel_actual].camino, juego.niveles[nivel_actual].tope_camino);

    } else if (juego.homero.antorcha_encendida){
        agregar_camino_manhattan(mapa, juego.niveles[nivel_actual].camino, juego.niveles[nivel_actual].tope_camino, juego.homero.posicion);
    }

    agregar_personaje(mapa, juego.homero);
    
    agregar_items(mapa, juego.niveles, juego.nivel_actual+CORRECCION_NIVEL);

    agregar_obstaculos(mapa, juego.niveles, nivel_actual, juego.niveles[nivel_actual].tope_obstaculos);

    agregar_herramientas (mapa,juego.niveles, nivel_actual, juego.niveles[nivel_actual].tope_herramientas);
}

///////////////////////////////////////////////
int estado_nivel(nivel_t nivel, personaje_t homero){
    // nivel  ganado si el pergamino+homero esta en el altar 0= jugando, 1 ganado utilizar personaje _t!!!!
    int tope = nivel.tope_camino;

    int altar_fil = nivel.camino[tope-1].fil;
    int altar_col = nivel.camino[tope-1].col;

    int fil_personaje = homero.posicion.fil;
    int col_personaje = homero.posicion.col;

    if ((fil_personaje == altar_fil && col_personaje == altar_col) && (homero.recolecto_pergamino)){
        return 1;
    } else {
        return 0;
    }
}
/////////////////////////////////////////////////

//////////////////////////////////////////////////
int estado_juego(juego_t juego){
  //  int ganado = 1;
    int perdido = -1;
    int jugando = 0;
//    int tope = juego.niveles[2].tope_camino -1;
//    int nivel_final = juego.niveles[3].camino[tope].fil;
//    int nivel_finnal = juego.niveles[3].camino[tope].col;
    int vidas = juego.homero.vidas_restantes;

    if (vidas == 0){
        return perdido;
    } else {
        return jugando;
    }

}
////////////////////////////////////////////////////

/*
* Pre condicones: mapa debe ser inicializado 
* Post condiciones: muestra por pantalla el mapa del juego
*/
void mostrar_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    //imprimir matriz
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

void mostrar_menu (int numero_vidas, int numero_hechizos, int numero_antorchas){
    printf("-----------------------------------------------------------\n\n");
    printf("Vidas: %i             \nHechizo revelador: %i             presionar (H) para utilizar\nAntorchas: %i                     presionar (L) para utilizar\n\n", numero_vidas, numero_hechizos, numero_antorchas);
}
/////////////////////////////////////////////////////////////////////

void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    mostrar_bienvenida();

    crear_mapa (mapa, juego, juego.nivel_actual+CORRECCION_NIVEL);
    mostrar_mapa(mapa);
    mostrar_menu(juego.homero.vidas_restantes, juego.homero.hechizos_reveladores, juego.homero.antorchas);
}

//////////////////////////////////////////////////////////////////////////////