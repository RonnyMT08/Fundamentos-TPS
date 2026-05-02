//gcc juego.c ascension_magios.c utiles.o -o juego -std=c99 -Wall -Wconversion -Werror -lm

#include "ascension_magios.h"
#include "utiles.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // Para usar rand, limpiar terminal, etc.
#include <time.h>   // Para obtener una semilla desde el reloj

#define MAX_CARACTERES 200

#define EMOJI_VACIO "  "
#define EMOJI_HOMERO "\U0001f9d4\u200D" 
#define EMOJI_FUEGO "\U0001f525"
#define EMOJI_PARED "\U0001f3e0"

#define EMOJI_PIEDRA "\U0001faa6"
#define EMOJI_CATAPULTA "\U0001f680"
#define EMOJI_TOTEM "\U0001f369"
#define EMOJI_CAMINO "\U0001f343"
#define EMOJI_RUNA "\u2721\uFE0F " 
#define EMOJI_ALTAR "\u26EA"
#define EMOJI_PERGAMINO "\U0001f4dc"
#define EMOJI_ANTOCHA "\U0001f3ee"
#define EMOJI_HECHIZO "\U0001f9ff"

#define EMOJI_SI "\u2705"
#define EMOJI_NO "\u274E"

const int VIDAS_INICIALES = 5;

const int NIVEL_INICIO = 1;
const int ANTORCHAS_INICIALES = 5;
const int RANGO_MANHATTAN = 2;

const int HECHIZOS_REVELADORES_INICIALES = 5;
const int PERGAMINOS_INICIALES = 3;
const int TOTEMS_INICIALES = 5;

const int PIEDRAS_CASTIGO_INCIALES = 10;

const char HOMERO = 'H';

const char PARED = 'X';
const char CAMINO = 'C';
const char VACIO = ' ';

const char RUNA = 'U';
const char ALTAR = 'A';

const char PERGAMINO = 'P';
const char TOTEM = 'T';

const char PIEDRA_CASTIGO = 'R';
const char CATAPULTA = 'F';

const char* MSJ_DECORADOR = "------------------------------------------------------------";
const char* MSJ_BIENVENIDA = "BIENVENIDO AL JUEGO DE ASCENCION DE MAGIOS";
const char* MSJ_POSICION_RANURA = ":      Hola Homero (\U0001f9d4\u200D)... Ahora estas en la Runa(\u2721\uFE0F )      :\n:      obten el Pergamino(\U0001f4dc) y llevalo al Altar(\u26EA)!!     :";
const char* MSJ_ALTAR_SIN_PERGAMINO   = ":   \u26A0\uFE0F   Prohibido el acceso del siguiente nivel..          :\n:       necesitas el Pergamino(\U0001f4dc)                         :";

const char* MSJ_POSICION_FUERA_CAMINO = ": (\U0001f525)!!!HOMERO!!!! TE ESTAS QUEMANDO... vuelve al camino  :\n:         (perdiste una de tus vidas)                      :" ;
const char* MSJ_INFORMATIVO1 = ":   \U0001f4dd  Las Piedras del Castigo(\U0001faa6) son molestas...!!      :\n:       activan la catapulta(\U0001f680) quitandote el pergamino   :";
const char* MSJ_INFORMATIVO2 = ":   \U0001f4dd  Usar Hechizo Revelador(H) puede romper caminos(\U0001f343) :\n:       camino ten mucho cuidado!!                         :";
const char* MSJ_INFORMATIVO3 = ":   \U0001f4dd  Los Totems(\U0001f369) te aumentarán las vidas...          :\n:       No existe limite de vidas!!                        :";
const char* MSJ_INFORMATIVO4 = ":   \U0001f4dd  La Antorcha(L) ilumina el camino(\U0001f343) en un rango   :\n:       Manhattan 3!!                                      :";
const char* MSJ_INFORMATIVO5 = ":   \U0001f4dd  Solo puedes ganar el nivel una ves puesto el       :\n:       pergamino en el altar                              :";
const char* MSJ_INFORMATIVO6 = ":   \U0001f4dd  No intentes atravesar las casas...                 :\n:       al chocarte con una casa no perderas vidas         :";

const char ARRIBA = 'W';
const char ABAJO = 'S';
const char DERECHA = 'D';
const char IZQUIERDA = 'A';

const char ANTORCHA = 'L';
const char HECHIZO_REVELADOR = 'H';


typedef struct emoji {
    char tipo[20];
    char representacion[40];
} emoji_t;


/* 
* Pre condiciones: El numero 'maximo' debe ser mayor al 'minimo'.
* Post condiciones: Devuelve un numero de manera aleatorio entre el valor 'minimo' y el 'maximo' valor (inclusive).
*/
int numero_aleatorio(int minimo, int maximo){
    int numero = rand() % (maximo - minimo + 1) + minimo;
    return numero;
}

/*
* Pre condiciones: El vector de caminos debe estar inicializado. el valor de fila debe estar entre 0-19 y columna entre 0-29.
* Post condiciones: Devuelve true si la 'fila', 'columna' coincide con las coordenda (fila, columna) del algun camino. False en caso contrario
*/
bool es_posicion_camino(coordenada_t camino[MAX_CAMINO], int tope_camino, int fila, int columna){
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
* Pre condiciones: El pergamino debe estar inicializado. fila debe estar entre (0-19), columna debe estar entre (0-29). 
* Post condiciones Devuelve true si la posicion (fila, columna) del pergamino coincide con la posicion 'fila', 'columna' dados por parametro. False en caso contrario. 
*/
bool es_posicion_pergamino(coordenada_t pergamino, int fila, int columna){
    return(fila == pergamino.fil && columna == pergamino.col);
}

/*
* Pre condiciones: la posicion de la runa debe estar inicializado, fil_movimiento debe estar entre (0-19) y col_movimiento debe estar entre (0-29)  
* Post condiciones: devuelve true si la posicion del personaje es igual a la posicion de la runa.
*/
bool es_posicion_runa(coordenada_t posicion, int fil_personaje, int col_personaje){
    return (posicion.fil == fil_personaje && posicion.col == col_personaje);
}

/*
* Pre condiciones: la posicion de la altar debe estar inicializado, fil_movimiento debe estar entre (0-19) y col_movimiento debe estar entre (0-29)  
* Post condiciones: devuelve true si la posicion del personaje es igual a la posicion de la runa.
*/
bool es_posicion_altar(coordenada_t posicion, int fil_personaje, int col_personaje){
    return (posicion.fil == fil_personaje && posicion.col == col_personaje);
}

/*
* Pre condiciones: el vector de herramientas debe estar inicializado, el tope_herramientas > 0, fila debe estar entre(0-19), columna debe estar entre (0-29).
* Post condiciones: devuelve true si la fila, columna coincide con una posicion de la herramienta (fila, columna). False en caso contrario
*/
bool es_posicion_herramienta(objeto_t herramienta[MAX_ELEMENTOS], int tope_herramientas, int fila, int columna){
    
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
* Pre condiciones: nivel debe estar inicializado, tope_paredes debe ser un entero > 0 , fila debe estar entre(0-19), columna debe estar entre (0-29).
* Post condiciones: devuelve true si la fila y columna coinciden con una posicion de la herramienta (fila, columna). False en caso contrario
*/
bool es_posicion_pared(nivel_t nivel, int tope_paredes, int fila, int columna){
    bool en_pared = false;
    int posicion_pared = 0;

    while((!en_pared) && (posicion_pared < tope_paredes)){
        if(nivel.paredes[posicion_pared].fil == fila && nivel.paredes[posicion_pared].col == columna){
            en_pared = true;
        } else {
            posicion_pared ++;
        }
    }
    return en_pared;
}

/*
* Pre dcondiciones: -
* Post condiciones: carga la cantidad de niveles que tendra el juego, tope de niveles.
*/
void inicializar_cantidad_niveles(juego_t* juego){
    (*juego).nivel_actual = NIVEL_INICIO;
    (*juego).tope_niveles = 3;
    (*juego).camino_visible = true;    
}

/* 
* Pre condiciones: Nivel actual dado debe ser 0, tope_niveles >= 0
* Post condiciones: Inicializa las paredes, caminos cada uno de los niveles.   
*/
void inicializar_posicion_estructura(nivel_t niveles[MAX_NIVELES], int tope_niveles){
    int tope_paredes = 0;
    int tope_caminos = 0; 

    for (int i = 0; i < tope_niveles; i++){
        
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
void inicializar_herramientas_personaje(int* vidas_restantes, int* hechizos_reveladores, int* antorchas, bool* antorcha_encendida, bool* recolecto_pergamino){
    (*vidas_restantes) = VIDAS_INICIALES;
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
    
    int fil_camino = posicion.fil;
    int col_camino = posicion.col;
    inicializar_posicion_personaje(&(*personaje).posicion, fil_camino, col_camino);
    inicializar_herramientas_personaje( &(*personaje).vidas_restantes, &(*personaje).hechizos_reveladores, &(*personaje).antorchas, &(*personaje).antorcha_encendida, &(*personaje).recolecto_pergamino);
}

/*
* Pre condiciones: los niveles deben estar inicializados, tope_niveles < 0.
* Post condiciones: Inicializa la posicion(fila, columna) aleatoria del pergamino para cada uno de los tres niveles.
*/
void inicializar_pergaminos(nivel_t niveles[MAX_NIVELES], int tope_niveles){
    
    for (int i = 0; i < tope_niveles; i++){

        int tope_camino = niveles[i].tope_camino;
        int fil_pergamino = numero_aleatorio(0, 19);
        int col_pergamino = numero_aleatorio(0, 29);
        
        while ( !(es_posicion_camino(niveles[i].camino, tope_camino, fil_pergamino, col_pergamino))){
            fil_pergamino = numero_aleatorio(0, 19);
            col_pergamino = numero_aleatorio(0, 29);
        }

        niveles[i].pergamino.fil = fil_pergamino;
        niveles[i].pergamino.col = col_pergamino;

    }
}

/*
* Pre condicones: los niveles deben estar inicializados, tope_niveles > 0.
* Post condicioes: Inicializa los 5 totems repatidos aleatoriamente en cada uno de los tres niveles, los totems se encuentran en el camino (excluyendo el inicio, final y la posicion del pergamino).  
*/
void inicializar_herramientas (nivel_t niveles[MAX_NIVELES], int tope_niveles){
   
    for (int i = 0; i < tope_niveles; i++){
        int tope_camino = niveles[i].tope_camino;        
        niveles[i].tope_herramientas = 0;

        for (int j = 0; j < TOTEMS_INICIALES; j++){
            int fil_totem = numero_aleatorio(0, 19);
            int col_totem = numero_aleatorio(0, 29);

            while (es_posicion_pergamino(niveles[i].pergamino, fil_totem, col_totem) || (es_posicion_altar(niveles[i].camino[0], fil_totem, col_totem) || es_posicion_runa(niveles[i].camino[tope_camino-1], fil_totem, col_totem) || es_posicion_pared(niveles[i], niveles[i].tope_paredes, fil_totem, col_totem))){
                fil_totem = numero_aleatorio(0, 19);
                col_totem = numero_aleatorio(0, 29);
            }
            niveles[i].herramientas[j].tipo = TOTEM;
            niveles[i].herramientas[j].posicion.fil = fil_totem;
            niveles[i].herramientas[j].posicion.col = col_totem;
            niveles[i].tope_herramientas++;
        }
    }
}

/*
* Pre condiciones: el vector de niveles debe estar inicializado, 'cantidad_obstaculos' debera ser >= 0. tope_niveles debe ser > 0. 
* Post condiciones: Inicializa una 10 piedras en posicion aleatoria(perteneciente al camino) sin superponer a otro objeto por cada uno de los tres niveles, carga el tope de los obstaculos por nivel.
*/
void inicializar_piedras_castigo(nivel_t niveles[MAX_NIVELES], int tope_niveles){

    for (int i = 0; i < tope_niveles; i++){
        int tope_camino = niveles[i].tope_camino;        
        niveles[i].tope_obstaculos = 0;

        for (int j = 0; j < PIEDRAS_CASTIGO_INCIALES; j++){
            int fil_piedra = numero_aleatorio(0, 19);
            int col_piedra = numero_aleatorio(0, 29);

            while (es_posicion_pergamino(niveles[i].pergamino, fil_piedra, col_piedra) || es_posicion_herramienta(niveles[i].herramientas, niveles[i].tope_herramientas, fil_piedra, col_piedra) || (es_posicion_altar(niveles[i].camino[0], fil_piedra, col_piedra) || es_posicion_runa(niveles[i].camino[tope_camino-1], fil_piedra, col_piedra) || es_posicion_pared(niveles[i], niveles[i].tope_paredes, fil_piedra, col_piedra))){
                
                fil_piedra = numero_aleatorio(0, 19);
                col_piedra = numero_aleatorio(0, 29);
            }

            niveles[i].obstaculos[j].tipo = PIEDRA_CASTIGO;
            niveles[i].obstaculos[j].posicion.fil = fil_piedra;
            niveles[i].obstaculos[j].posicion.col = col_piedra;
            niveles[i].tope_obstaculos ++;
        }
    }
}

/*
* Pre condiciones: El vector de nivel debe estar inicializado, tope_niveles debe ser > 0;
* Post condiciones: Inicializa la posicion de una catapulta por nivel, la posicion no pertenece a una posicion del camino y distinto de la posicion de una pared. 
*/
void inicializar_catapultas(nivel_t niveles[MAX_NIVELES], int tope_niveles){
    
    for(int i = 0; i < tope_niveles; i++){
        int tope_obstaculos = niveles[i].tope_obstaculos;

        int fil_catapulta = numero_aleatorio(0, MAX_FILAS-1);
        int col_catapulta = numero_aleatorio(0, MAX_COLUMNAS-1);

        while (es_posicion_camino(niveles[i].camino, niveles[i].tope_camino, fil_catapulta, col_catapulta) || es_posicion_pared(niveles[i], niveles[i].tope_paredes, fil_catapulta, col_catapulta) || es_posicion_pergamino(niveles[i].pergamino, fil_catapulta, col_catapulta) || es_posicion_herramienta(niveles[i].herramientas, niveles[i].tope_herramientas, fil_catapulta, col_catapulta)){

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

void inicializar_juego(juego_t* juego) {
    
    
    inicializar_cantidad_niveles( &(*juego));
    
    inicializar_posicion_estructura( (*juego).niveles, (*juego).tope_niveles);
    
    int posicion_nivel = (*juego).nivel_actual-1;
    
    inicializar_homero( &(*juego).homero, (*juego).niveles[posicion_nivel].camino[0]);

    inicializar_pergaminos( (*juego).niveles, (*juego).tope_niveles);

    inicializar_herramientas( (*juego).niveles, (*juego).tope_niveles);

    inicializar_obstaculos((*juego).niveles, (*juego).tope_niveles);
}

void cambiar_nivel(juego_t* juego) {   
    int posicion_nivel_actual = (*juego).nivel_actual-1;
    int nivel_actual = (*juego).nivel_actual;

    if (nivel_actual < 3){
        (*juego).nivel_actual ++;
        
        posicion_nivel_actual = (*juego).nivel_actual-1;
        nivel_actual = (*juego).nivel_actual;
        
        (*juego).homero.posicion.fil = (*juego).niveles[posicion_nivel_actual].camino[0].fil; 
        (*juego).homero.posicion.col = (*juego).niveles[posicion_nivel_actual].camino[0].col;

        (*juego).camino_visible = true;
        (*juego).homero.recolecto_pergamino = false;
        (*juego).homero.antorchas = 5;
    }

}

/*
* Pre condiciones: -
* Post condicones: devuelve true si el movimiento es S, D, A ó W, False en caso contrario.
*/
bool es_movimiento(char movimiento){
    return (movimiento == DERECHA || movimiento == ABAJO || movimiento == IZQUIERDA || movimiento == ARRIBA );
}

/*
* Pre condiciones: -
* Post condiciones: devuelve true si fil_movimiento esta entre 0-19 y col_movimiento esta entre 0-29.
*/
bool es_posicion_mapa(int fil_movimiento, int col_movimiento){
    return (fil_movimiento >= 0 && fil_movimiento <= 19) && (col_movimiento >= 0 && col_movimiento <= 29);
}

/*
* Pre condicones: -
* Post condiciones: devuelve true si el 'movimiento' es una herramienta (H) ó (L). False en caso contrario. 
*/
bool es_herramienta(char movimiento){
    return (movimiento == HECHIZO_REVELADOR || movimiento == ANTORCHA);
}

/*
* Pre condiciones: la posicion del personaje debe estar inicializado, fil_movimiento debe estar entre (0-19) y col_movimiento debe estar entre (0-29)  
* Post condiciones: devuelve true si la posicion del personaje es igual a la posicion del camino(fila, columna) dado.
*/
bool es_posicion_homero(coordenada_t posicion_personaje, int fil_camino, int col_camino) {
    return(posicion_personaje.fil == fil_camino && posicion_personaje.col == col_camino);
}

/* 
* Pre condiciones: el vector de obstaculos debe estar incializado, tope_obtaculos debe ser > 0, fila entre 0 - 19 y columna entre 0 - 29. 
* Post condiciones: devuelve true si la fila, columana coinciden con la posicion (fila, columna) de alguno de los obstaculos. False en caso contrario.
*/
bool es_posicion_obstaculos (objeto_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos, int fila, int columna, char tipo){
    
    bool en_obstaculo = false;
    int posicion_obstaculo = 0;
    while(!en_obstaculo && posicion_obstaculo < tope_obstaculos){

        if((obstaculos[posicion_obstaculo].posicion.fil == fila) && (obstaculos[posicion_obstaculo].posicion.col == columna) && (obstaculos[posicion_obstaculo].tipo == tipo)){
            en_obstaculo = true;
        } else {
            posicion_obstaculo ++;
        }
    }
    return en_obstaculo;
}

/*
* Pre condiciones: el vector de objetos debe estar inicializado, tope de objetos > 0, fil_movimiento debe estar entre (0-19) y col_movimiento debe estar entre (0-29)  
* Post condiciones: devuelve el indice de la posicion en la que se encuentra la el objeto.
*/
int busca_indic_posicion_objeto(objeto_t objetos[MAX_ELEMENTOS], int tope_objetos, int fil_movimiento, int col_movimiento){
    int posicion = 0;
    bool posicion_encontrada = false;
    
    while(!posicion_encontrada && posicion < tope_objetos){

        if (objetos[posicion].posicion.fil == fil_movimiento && objetos[posicion].posicion.col == col_movimiento){
            posicion_encontrada = true;
        } else {
            posicion ++;
        }
    }
    return posicion;
}

/*
* Pre condiciones: el vector de posiciones debe estar inicializado, tope_posiciones > 0, fil_camino debe estar entre (0-19) y col_camino debe estar entre (0-29)  
* Post condiciones: devuelve el indice de la posicion en la que se encuentra el camino.
*/
int busca_indice_posicion(coordenada_t posiciones[MAX_CAMINO], int tope_posiciones, int fil_camino, int col_camino){
    int indice_posicion = -1;
    bool posicion_encontrada = false;
    
    while((!posicion_encontrada) && (indice_posicion < tope_posiciones)){
        indice_posicion ++;

        if (posiciones[indice_posicion].fil == fil_camino && posiciones[indice_posicion].col == col_camino){
            posicion_encontrada = true;
        }
    }
    return indice_posicion;
}

/*
* Pre condiciones: -
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
* Post condiciones: actualiza la posicion del personaje de acuerdo a los valores del movimiento (fil_movimineto y col_movimientos) dados.
*/
void posicionar_personaje(coordenada_t* posicion, int fil_movimiento, int col_movimiento){
    (*posicion).fil += fil_movimiento;
    (*posicion).col += col_movimiento;
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
* Pre condiciones: el vector de objetos debe estar inicializado, tope_objetos > 0 y posicion_objetos debera ser >= 0.   
* Post condiciones: actualiza un vector de objetos, tope_objetos..
*/
void eliminar_objeto(objeto_t objetos[MAX_ELEMENTOS], int* tope_objetos, int posicion_objeto){
    for (int i = posicion_objeto; i < (*tope_objetos)-1 ; i++){
        objetos[i].tipo = objetos[i+1].tipo;
        objetos[i].posicion.fil = objetos[i+1].posicion.fil;
        objetos[i].posicion.col = objetos[i+1].posicion.col;
    }
    (*tope_objetos) -= 1;
}

/*
* Pre condiciones: el vector de caminos debe estar inicializado, tope_caminos >0 y posicion del camino deben ser >= 0.
* Post condiciones: elimina el elemento del vector de caminos segun la posicion_camino dado, actualiza el valor del tope.
*/
void eliminar_camino(coordenada_t caminos[MAX_CAMINO], int* tope_caminos, int posicion_camino){
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
void lanzar_bola_fuego(nivel_t* nivel, int* tope_caminos, coordenada_t posicion_personaje){
    int fil_bola = numero_aleatorio(0, 19);
    int col_bola = numero_aleatorio(0, 29);
    
    while (es_posicion_pared((*nivel), (*nivel).tope_paredes, fil_bola, col_bola) || es_posicion_runa( (*nivel).camino[0], fil_bola, col_bola)  || es_posicion_altar( (*nivel).camino[(*tope_caminos)-1], fil_bola, col_bola) || es_posicion_homero(posicion_personaje, fil_bola, col_bola) ){        
        fil_bola = numero_aleatorio(0, 19);
        col_bola = numero_aleatorio(0, 29);
    }
    
    if (es_posicion_camino( (*nivel).camino, *tope_caminos, fil_bola, col_bola)){
        int indice_posicion_camino;
        indice_posicion_camino = busca_indice_posicion((*nivel).camino, (*nivel).tope_camino, fil_bola, col_bola);
        eliminar_camino((*nivel).camino, &(*nivel).tope_camino, indice_posicion_camino);        
    }
}

/*
* Pre condiciones: el nivel y el vector de caminos deben estar inicializados, la posicion_pergamino >= 0, tope_camino debera ser > 0, 
* Post condiciones: actualiza la posicion del pergamino en una posicion aleatoria, sobre el camino (sin superponerse a cualquier obstaculo u herramienta).
*/
void posicionar_pergamino(nivel_t nivel, coordenada_t* posicion_pergamino, int tope_camino){
    
    int fil_pergamino = numero_aleatorio(0, 19);
    int col_pergamino = numero_aleatorio(0, 29);
    
    while (es_posicion_altar(nivel.camino[tope_camino-1], fil_pergamino, col_pergamino) || es_posicion_runa(nivel.camino[0], fil_pergamino, col_pergamino) || es_posicion_herramienta(nivel.herramientas, nivel.tope_herramientas, fil_pergamino, col_pergamino) || es_posicion_obstaculos(nivel.obstaculos, nivel.tope_obstaculos, fil_pergamino, col_pergamino, CATAPULTA) || es_posicion_obstaculos(nivel.obstaculos, nivel.tope_obstaculos, fil_pergamino, col_pergamino, PIEDRA_CASTIGO) || !(es_posicion_camino(nivel.camino, tope_camino, fil_pergamino, col_pergamino))){
        fil_pergamino = numero_aleatorio(0, 19);
        col_pergamino = numero_aleatorio(0, 29);    
    }

    (*posicion_pergamino).fil = fil_pergamino;
    (*posicion_pergamino).col = col_pergamino;
}

/*
* Pre condiciones: juego debe ser inicializado, movimiento tiene que ser A,S,D o W
* Post condciciones: actualiza la nueva posicion del jugador (siendo este dentro del camino).
*/
void dirigir_movimiento(nivel_t* nivel, coordenada_t* posicion_homero, juego_t* juego, int fil_movimiento, int col_movimiento){
    int fil_personaje = (*posicion_homero).fil;
    int col_personaje = (*posicion_homero).col;

    int nueva_fil_personaje = fil_movimiento + fil_personaje;
    int nueva_col_personaje = col_movimiento + col_personaje;

    if (!es_posicion_pared(*nivel , (*nivel).tope_paredes, nueva_fil_personaje, nueva_col_personaje) &&  es_posicion_mapa(nueva_fil_personaje, nueva_col_personaje)){
        
        if (es_posicion_pergamino((*nivel).pergamino, nueva_fil_personaje, nueva_col_personaje)){
            int posicion_altar = (*nivel).tope_camino -1;
            int fil_altar = (*nivel).camino[posicion_altar].fil;
            int col_altar = (*nivel).camino[posicion_altar].col;

            (*juego).camino_visible = true;
            (*juego).homero.recolecto_pergamino = true;
            (*juego).homero.antorcha_encendida = false;     

            (*nivel).pergamino.fil = fil_altar;
            (*nivel).pergamino.col = col_altar;
       
        } else if ( es_posicion_runa((*nivel).camino[0], nueva_fil_personaje, nueva_col_personaje) ){
            lanzar_bola_fuego(&(*nivel), &(*nivel).tope_camino, *posicion_homero);

            (*juego).camino_visible = true;
            (*juego).homero.antorcha_encendida = false;     
    
        } else if (es_posicion_herramienta((*nivel).herramientas, (*nivel).tope_herramientas, nueva_fil_personaje, nueva_col_personaje) ){
            int indice_posic_herramienta;
            indice_posic_herramienta = busca_indic_posicion_objeto((*nivel).herramientas,( *nivel).tope_herramientas, nueva_fil_personaje, nueva_col_personaje);
            
            if (indice_posic_herramienta >= 0){

                if ( es_posicion_camino((*nivel).camino, (*nivel).tope_camino, nueva_fil_personaje, nueva_col_personaje)){
                    tomar_herramienta(&(*juego).homero.vidas_restantes, (*nivel).herramientas[indice_posic_herramienta]);
                    eliminar_objeto((*nivel).herramientas, &(*nivel).tope_herramientas, indice_posic_herramienta);        
                } else {
                    eliminar_objeto((*nivel).herramientas, &(*nivel).tope_herramientas, indice_posic_herramienta);        
                }
                (*juego).camino_visible = false;
                (*juego).homero.antorcha_encendida = false;                 
            }
            
        } else if (es_posicion_obstaculos((*nivel).obstaculos, (*nivel).tope_obstaculos, nueva_fil_personaje, nueva_col_personaje, PIEDRA_CASTIGO)){
            int indice_posic_obstaculo;
    
            indice_posic_obstaculo = busca_indic_posicion_objeto((*nivel).obstaculos, (*nivel).tope_obstaculos, nueva_fil_personaje, nueva_col_personaje);
            posicionar_pergamino((*nivel), &(*nivel).pergamino, (*nivel).tope_camino );
            eliminar_objeto((*nivel).obstaculos, &(*nivel).tope_obstaculos, indice_posic_obstaculo);
            (*juego).homero.recolecto_pergamino = false;
            (*juego).camino_visible = false;
            (*juego).homero.antorcha_encendida = false;
        
        } else if (!es_posicion_camino((*nivel).camino, (*nivel).tope_camino, nueva_fil_personaje, nueva_col_personaje) && !(es_posicion_pared((*nivel), (*nivel).tope_paredes, nueva_fil_personaje, nueva_col_personaje)) ) {
            (*juego).homero.vidas_restantes -=1;
            (*juego).homero.antorcha_encendida = false;
            (*juego).camino_visible = false;
        
        } else {
            (*juego).homero.antorcha_encendida = false;
            (*juego).camino_visible = false;
        }
        posicionar_personaje(&(*juego).homero.posicion, fil_movimiento, col_movimiento);     
    }
}

/*
* Pre condiciones: el juego debe estar incializado
* Post condiciones: actualiza el juego segun el movimiento dado (moverse o utilizar una herramienta).
*/
void realizar_jugada(juego_t* juego, char movimiento){

    int fil_movimiento = 0;
    int col_movimiento = 0;
    int posicion_nivel_actual = (*juego).nivel_actual -1;

    if (es_movimiento(movimiento)){
        valorar_movimiento(movimiento, &fil_movimiento, &col_movimiento);
        dirigir_movimiento(&(*juego).niveles[posicion_nivel_actual], &(*juego).homero.posicion, &(*juego), fil_movimiento, col_movimiento);

    } else if (es_herramienta(movimiento)){

        if ((movimiento == HECHIZO_REVELADOR) && ((*juego).homero.hechizos_reveladores > 0) && !(*juego).camino_visible && !(*juego).homero.antorcha_encendida && !(es_posicion_runa((*juego).homero.posicion, (*juego).niveles[posicion_nivel_actual].camino[0].fil, (*juego).niveles[posicion_nivel_actual].camino[0].col) )){
            lanzar_bola_fuego(&(*juego).niveles[posicion_nivel_actual], &(*juego).niveles[posicion_nivel_actual].tope_camino, (*juego).homero.posicion);
            (*juego).camino_visible = true;
            (*juego).homero.hechizos_reveladores -= 1;

        } else if ((movimiento == ANTORCHA) && ((*juego).homero.antorchas > 0) && !(*juego).homero.antorcha_encendida && !(*juego).camino_visible && !(es_posicion_runa((*juego).homero.posicion, (*juego).niveles[posicion_nivel_actual].camino[0].fil, (*juego).niveles[posicion_nivel_actual].camino[0].col))){
            (*juego).homero.antorcha_encendida = true;
            (*juego).homero.antorchas -= 1;
        }
    }
}


/*
* Pre condiciones: nota debe estar inicializado de forma vacia;
* Post condiciones: muestra por pantalla una informacion al usuario de forma aleatoria entre (primer o sexto) mensaje.
*/
void mostrar_mensaje_informacion(char nota[MAX_CARACTERES]){
    int numero_mensaje = numero_aleatorio(1, 6);
    switch (numero_mensaje){
    case 1:
        strcpy(nota, MSJ_INFORMATIVO1);
        break;
    case 2:
        strcpy(nota, MSJ_INFORMATIVO2);
        break;
    case 3:
        strcpy(nota, MSJ_INFORMATIVO3);
        break;
    case 4:
        strcpy(nota, MSJ_INFORMATIVO4);
        break;
    case 5:
        strcpy(nota, MSJ_INFORMATIVO5);
        break;
    case 6:
        strcpy(nota, MSJ_INFORMATIVO6);
        break;
    default:
        break;
    }        
}

/*
* Pre condiciones: -
* Post condiciones: muestra por pantalla mensajes de interaccion con el usuario (bienvenida, consejos).
*/
void mostrar_bienvenida(juego_t juego){
    
    int posicion_nivel_actual = juego.nivel_actual -1;
    int nivel_actual = juego.nivel_actual;
    int fil_homero = juego.homero.posicion.fil;
    int col_homero = juego.homero.posicion.col;
    int tope_camino = juego.niveles[posicion_nivel_actual].tope_camino;
    char nota[500] = "";

    system("clear");
    printf("    %s (Nvl %i/%i)\n",MSJ_BIENVENIDA, nivel_actual, juego.tope_niveles);

    if (es_posicion_runa(juego.homero.posicion, juego.niveles[posicion_nivel_actual].camino[0].fil, juego.niveles[posicion_nivel_actual].camino[0].col)){
        strcpy(nota,MSJ_POSICION_RANURA);
    } else if (es_posicion_altar(juego.niveles[posicion_nivel_actual].camino[tope_camino-1], fil_homero, col_homero)){
        strcpy(nota, MSJ_ALTAR_SIN_PERGAMINO);
    } else if (!es_posicion_camino(juego.niveles[posicion_nivel_actual].camino, tope_camino, fil_homero, col_homero)){
        strcpy(nota, MSJ_POSICION_FUERA_CAMINO);
    } else if (es_posicion_camino(juego.niveles[posicion_nivel_actual].camino, tope_camino, fil_homero, col_homero)){
        mostrar_mensaje_informacion(nota);
    }  else {
        printf("%s\n\n\n%s\n", MSJ_DECORADOR, MSJ_DECORADOR);
    }
    printf("%s\n%s\n%s\n", MSJ_DECORADOR, nota, MSJ_DECORADOR);
}

/*
* Pre condiciones: -
* Post condiciones: carga el mapa sin elementos. 
*/
void inicializar_mapa( emoji_t mapa[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            strcpy(mapa[i][j].representacion, EMOJI_VACIO);
        }
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de paredes deben estar inicializado, el tope de paredes debe ser > 0.
* Post condiciones: carga paredes en la matriz mapa segun las posiciones(fila, columna) de cada pared. 
*/
void agregar_paredes(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t paredes[MAX_PAREDES], int tope_paredes){
    for (int i = 0; i < tope_paredes; i++){
        int fil_pared = paredes[i].fil;
        int col_pared = paredes[i].col;

        strcpy(mapa[fil_pared][col_pared].representacion, EMOJI_PARED);
    }
}


/*
* Pre condiciones: la matriz del mapa, vector de caminos, nivel deben estar inicializados, el tope de caminos y tope de paredes debe ser > 0.
* Post condiciones: carga llamas de fuego en la matriz mapa segun no sean las posiciones(fila, columna) del camino y la pared.
*/
void agregar_fuego(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t caminos[MAX_CAMINO], int tope_caminos, nivel_t nivel, int tope_paredes){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            if (!es_posicion_camino(caminos, tope_caminos, i, j) && !es_posicion_pared(nivel, tope_paredes, i, j)){
                strcpy(mapa[i][j].representacion, EMOJI_FUEGO);
            }
        }
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de caminos deben estar inicializados, tope_camino debe ser >0.
* Post condiciones: carga los caminos en la matriz mapa segun las posiciones (fila, columna) de cada camino
*/
void agregar_camino(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino){
    for (int i = 0; i < tope_camino; i++){
        int fil_camino = camino[i].fil;
        int col_camino = camino[i].col;
        strcpy(mapa[fil_camino][col_camino].representacion, EMOJI_CAMINO);
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de caminos, nivel deben estar inicializados, tope_camino y tope_paredes debe ser > 0, la posicion del personaje debe ser dentro de los limites de la matriz mapa.  
* Post condiciones: carga en el mapa los caminos y paredes en un rango manhattan 3 segun la posicion del personaje.
*/
void agregar_camino_manhattan(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t caminos[MAX_CAMINO], int tope_caminos, nivel_t nivel, int tope_paredes, coordenada_t posicion){
    int fil_homero = posicion.fil;
    int col_homero = posicion.col;
    int rango = RANGO_MANHATTAN;

    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){

            if ( ((fil_homero - i) <= rango) && ((col_homero - j) <= rango) && ((fil_homero - i) >= -rango) && ((col_homero - j) >= -rango)){

                int fil_coordenada = fil_homero - i;
                int col_coordenada = col_homero - j;
        
                if  ( ((fil_coordenada + col_coordenada) <= rango && (fil_coordenada - col_coordenada) >= -rango ) && ((fil_coordenada - col_coordenada) <= rango && (fil_coordenada + col_coordenada) >= -rango )){

                    if (es_posicion_camino(caminos, tope_caminos, i, j)){
                        strcpy(mapa[i][j].representacion, EMOJI_CAMINO);
                    } else if (!es_posicion_pared(nivel, tope_paredes, i, j)) {
                        strcpy(mapa[i][j].representacion, EMOJI_FUEGO);
                    }
                }

            }
        }
    }
}

/*
* Pre condiciones: la matriz del mapa, el personaje deben ser inicializado.
* Post condiciones: carga en el mapa el personaje segun su posicion(fila, columna) en el que se encuentra.
*/
void agregar_personaje(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], personaje_t homero){
    int fil_homero = homero.posicion.fil;
    int col_homero = homero.posicion.col;

    strcpy(mapa[fil_homero][col_homero].representacion, EMOJI_HOMERO);
}

/*
* Pre condiciones: la matriz del mapa, vector de niveles deben estar inicializado, el nivel_actual debe ser >= 0.
* Post condicones: carga en el mapa la runa, el altar y pergamino, segun sus posiciones(fila, columna) del mismo correspondiente al nivel.  
*/
void agregar_items(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel){
    int tope_camino = nivel.tope_camino;

    int camino_inicial_fil = nivel.camino[0].fil;
    int camino_inicial_col = nivel.camino[0].col;
    strcpy(mapa[camino_inicial_fil][camino_inicial_col].representacion, EMOJI_RUNA);

    int camino_final_fil = nivel.camino[tope_camino-1].fil;
    int camino_final_col = nivel.camino[tope_camino-1].col;

    strcpy(mapa[camino_final_fil][camino_final_col].representacion, EMOJI_ALTAR);
    int fil_pergamino = nivel.pergamino.fil;
    int col_pergamino = nivel.pergamino.col;

    if (!es_posicion_altar(nivel.camino[tope_camino-1], fil_pergamino, col_pergamino)){
        strcpy(mapa[fil_pergamino][col_pergamino].representacion, EMOJI_PERGAMINO);
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de nivel deben estar inicializados, tope_obstaculos > 0;
* Post condiciones: agrega en el mapa los obstaculos segun su posicion(fila, columna).
*/
void agregar_obstaculos(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel, int tope_obstaculos){
    for (int i = 0; i < tope_obstaculos; i++){
        int piedra_fil = nivel.obstaculos[i].posicion.fil;
        int piedra_col = nivel.obstaculos[i].posicion.col; 
        if (nivel.obstaculos[i].tipo == PIEDRA_CASTIGO){

            strcpy(mapa[piedra_fil][piedra_col].representacion, EMOJI_PIEDRA);

        } else if (nivel.obstaculos[i].tipo == CATAPULTA) {
            strcpy(mapa[piedra_fil][piedra_col].representacion, EMOJI_CATAPULTA);
        }
    }
}

/*
* Pre condiciones: la matriz del mapa, vector de niveles deben estar inicializados, tope_herramientas > 0;
* Post condiciones: agrega a la matriz del mapa en las coordenadas de la posicion (fila, columna) las herramientas segun de la herramienta.
*/
void agregar_herramientas(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel, int tope_herramientas){
    
    for (int i = 0; i < tope_herramientas; i++){
        int totem_fil = nivel.herramientas[i].posicion.fil;
        int totem_col = nivel.herramientas[i].posicion.col;
        strcpy(mapa[totem_fil][totem_col].representacion, EMOJI_TOTEM);
    }
}

/*
* Pre condiciones: la matriz del mapa y el juego deben estar incializados, posicion_nivel_actual debe ser >= 0..
* Post condiciones: crea el mapa del juego de acuerdo al nivel correspondiente.
*/
void crear_mapa(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego, int posicion_nivel_actual){
    
    inicializar_mapa(mapa);
  
    agregar_paredes(mapa, juego.niveles[posicion_nivel_actual].paredes, juego.niveles[posicion_nivel_actual].tope_paredes);

    if (juego.camino_visible){
        agregar_fuego(mapa, juego.niveles[posicion_nivel_actual].camino, juego.niveles[posicion_nivel_actual].tope_camino,juego.niveles[posicion_nivel_actual], juego.niveles[posicion_nivel_actual].tope_paredes);
        agregar_camino(mapa, juego.niveles[posicion_nivel_actual].camino, juego.niveles[posicion_nivel_actual].tope_camino);

    } else if (juego.homero.antorcha_encendida){
        agregar_camino_manhattan(mapa, juego.niveles[posicion_nivel_actual].camino, juego.niveles[posicion_nivel_actual].tope_camino, juego.niveles[posicion_nivel_actual], juego.niveles[posicion_nivel_actual].tope_paredes, juego.homero.posicion);
    }

    agregar_personaje(mapa, juego.homero);
    
    agregar_items(mapa, juego.niveles[posicion_nivel_actual]);

    agregar_obstaculos(mapa, juego.niveles[posicion_nivel_actual], juego.niveles[posicion_nivel_actual].tope_obstaculos);

    agregar_herramientas (mapa,juego.niveles[posicion_nivel_actual], juego.niveles[posicion_nivel_actual].tope_herramientas);
}

int estado_nivel(nivel_t nivel, personaje_t homero){
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

int estado_juego(juego_t juego){
  //  int ganado = 1;
    int perdido = -1;
    int jugando = 0;
    int ganado = 1;
    int vidas = juego.homero.vidas_restantes;
    int posicion_nivel_actual = (juego.nivel_actual)-1;
    int nivel_actual = (juego).nivel_actual;
    int posicion_altar = juego.niveles[posicion_nivel_actual].tope_camino-1;
    int fil_homero = juego.homero.posicion.fil;
    int col_homero = juego.homero.posicion.col;

    if ((vidas >= 0) && (nivel_actual == 3 && juego.homero.recolecto_pergamino) && (es_posicion_altar(juego.niveles[posicion_nivel_actual].camino[posicion_altar], fil_homero, col_homero))){
        return ganado;
    } else if ((nivel_actual < 4) && (vidas <= 0)){
        return perdido;
    } else {
        return jugando;
    }

}

/*
* Pre condicones: mapa debe ser inicializado 
* Post condiciones: muestra por pantalla el mapa del juego
*/
void mostrar_mapa(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            printf("%s", mapa[i][j].representacion);
        }
        printf("\n");
    }
}

/*
* Pre condicones: cantidad_emojis debe ser >0, cadena debe estar vacio, emoji debe estar en formato codigo. 
* Post condiciones: carga emojis en cadena segun la cantidad_emojis dada.
*/
void agregar_emojis(int cantidad_emojis, char cadena[MAX_CARACTERES], char emoji[]){
    for(int i = 0; i < cantidad_emojis; i++){
        strcat(cadena, emoji);
    }
}

/*
* Pre condicones: - 
* Post condiciones: muestra por pantalla los datos con el estado del usuario (cantidad vidas, antorchas, hechizos, pergaminos).
*/
void mostrar_datos_usuario (int numero_vidas, int numero_hechizos, int numero_antorchas, bool pergamino_obtenido){
    char vidas[MAX_CARACTERES] = " "; 
    char antorcha[MAX_CARACTERES] = " ";
    char hechizo[MAX_CARACTERES] = " ";
    
    agregar_emojis(numero_vidas, vidas, EMOJI_TOTEM);
    agregar_emojis(numero_antorchas, antorcha, EMOJI_ANTOCHA);
    agregar_emojis(numero_hechizos, hechizo, EMOJI_HECHIZO);
    printf("%s \nVidas: %s \nAntorchas(L): %s \nHechizo revelador(H): %s \n", MSJ_DECORADOR, vidas, antorcha, hechizo);

    if (pergamino_obtenido){
        printf("Pergamino: %s \n", EMOJI_SI);
    } else {
        printf("Pergamino: %s \n", EMOJI_NO);
    }
}

void mostrar_juego(juego_t juego){

    emoji_t mapa[MAX_FILAS][MAX_COLUMNAS];
    int posicion_nivel_actual = juego.nivel_actual -1;
    mostrar_bienvenida(juego);

    crear_mapa (mapa, juego, posicion_nivel_actual);
    mostrar_mapa(mapa);
    mostrar_datos_usuario(juego.homero.vidas_restantes, juego.homero.hechizos_reveladores, juego.homero.antorchas, juego.homero.recolecto_pergamino);
}
