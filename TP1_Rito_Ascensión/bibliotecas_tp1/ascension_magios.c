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

const int MIN_FIL_POSICION = 0;
const int MIN_COL_POSICION = 0;
const int MAX_FIL_POSICION = 20;
const int MAX_COL_POSICION = 30;

const int NIVEL_INICIO = 1;
const int NIVELES_MAXIMO = 3;
const int RANGO_MANHATTAN = 3;
const int MAX_MENSAJES_INFORMATIVO = 6;

const int VIDAS_INICIALES = 5;
const int ANTORCHAS_INICIALES = 5;
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
* Pre condiciones:
    -> 'maximo_exclusivo' debe ser > 0.
* Post condiciones: 
    -> Devuele un valor aleatorio dentro del rango de 0 y el ('maximo_exclusivo' - 1).
*/
int numero_aleatorio(int maximo_exclusivo){
    int numero = rand() % (maximo_exclusivo-1);
    return numero;
}

/*
* Pre condiciones: 
    -> El vector de 'estructuras'(caminos y/o paredes) debe estar inicializado. 
    -> El valor de 'tope_estructura' debe ser > 0.
    -> El valor de 'fila_dada' debe estar entre 0-19 y 'columna_dada' entre 0-29.
* Post condiciones: 
    -> Devuelve -true- si la 'fila_dada', 'columna_dada' coincide con las coordenda (fila, columna) del algun camino y/o pared
    -> Devuelve -false- en caso contrario
*/
bool es_posicion_estructura(coordenada_t estructuras[MAX_CAMINO], int tope_estructura, int fila_dada, int columna_dada){
    bool en_estructura = false;
    int indice_estructura = 0;
    while(!en_estructura && (indice_estructura < tope_estructura)){
        if(estructuras[indice_estructura].fil == fila_dada && estructuras[indice_estructura].col == columna_dada){
            en_estructura = true;
        }
        indice_estructura ++;
    }
    return en_estructura;
}

/*
* Pre condiciones: 
    -> EL objeto debe estar incializado. 
    -> El valor de 'fila_dada' debe estar entre (0-19)
    -> El valor de 'columna_dada' debe estar entre (0-29)
* Pos condiciones: 
    -> Devuelve -true- si la posicion de 'coordenda_elemento' coincide con la 'fila_dada' y 'columna_dada'.
    -> Devuelve -false- en caso contrario.
*/
bool es_posicion_elemento(coordenada_t coordenada_elemento, int fila_dada, int columna_dada){
    return (coordenada_elemento.fil == fila_dada && coordenada_elemento.col == columna_dada);
}

/*
* Pre condiciones: 
    -> El vector de objetos debe estar inicializado.
    -> El valor de 'tope_objetos' debe ser > 0.
    -> Los valor de 'fila_dada' debe estar entre(0-19) y 'columna_dada' debe estar entre (0-29).
* Post condiciones: 
    -> Devuelve true si los valores de la 'fila_dada' y 'columna_dada' coincide con alguna posicion del objeto (fila, columna).
    -> Devuelve False en caso contrario.
*/
bool es_posicion_objeto(objeto_t objetos[MAX_ELEMENTOS], int tope_objetos, int fila_dada, int columna_dada){
    
    bool en_objeto = false;
    int indice_objeto = 0;
    while(!en_objeto && indice_objeto < tope_objetos){

        if(objetos[indice_objeto].posicion.fil == fila_dada && objetos[indice_objeto].posicion.col == columna_dada){
            en_objeto = true;
        }
        indice_objeto ++;
    }
    return en_objeto;
}

/*
* Pre condiciones: 
    -> 'nivel' debe estar inicializado.
    -> El valor de 'tope_camino'.
    -> Los valores de 'fila_totem' debe estar entre 0-19 y 'columna_totem' entre 0-29
* Post condiciones: 
    -> Devuelve -true- si es: pergamino, camino inicial, camino final, paredes. 
    -> Devuelve -false- en caso contrario.
*/
bool es_posicion_para_totem(nivel_t* nivel, int tope_camino, int fila_totem, int columna_totem){
    return (
        es_posicion_elemento((*nivel).pergamino, fila_totem, columna_totem) || 
        es_posicion_elemento((*nivel).camino[0], fila_totem, columna_totem) || 
        es_posicion_elemento((*nivel).camino[tope_camino-1], fila_totem, columna_totem) || 
        es_posicion_estructura((*nivel).paredes, (*nivel).tope_paredes, fila_totem, columna_totem)
    );
}

/*
* Pre condiciones:
    -> 'nivel' debe estar inicializado.
    -> Los valores de 'fila_piedra' debe estar entre 0-19 y 'columna_piedra' entre 0-29.
* Post condiciones:
    -> Devuelve -true- si es: pergamino, camino inicial, camino final, paredes y herramientas.
    -> Devuelve -false- en caso contrario.
*/
bool es_posicion_para_piedra(nivel_t* nivel, int fila_piedra, int columna_piedra){
    int tope_camino = (*nivel).tope_camino;
    return (
        es_posicion_para_totem(&(*nivel), tope_camino, fila_piedra, columna_piedra) || 
        es_posicion_objeto((*nivel).herramientas, (*nivel).tope_herramientas, fila_piedra, columna_piedra)
    );
}

/*
* Pre condiciones: 
    -> 'nivel' debe estar inicializado.
    -> Los valores de 'fila_totem' debe estar entre 0-19 y 'columna_totem' entre 0-29
* Post condiciones: 
    -> Devuelve -true- si es: camino, paredes, pergamino, camino inicial, herramientas. 
    -> Devuelve -false- en caso contrario.
*/
bool es_posicion_para_catapulta(nivel_t* nivel, int fila_catapulta, int columna_catapulta){
    return(
        es_posicion_estructura((*nivel).camino, (*nivel).tope_camino, fila_catapulta, columna_catapulta) || 
        es_posicion_estructura((*nivel).paredes, (*nivel).tope_paredes, fila_catapulta, columna_catapulta) || 
        es_posicion_elemento((*nivel).pergamino, fila_catapulta, columna_catapulta) || 
        es_posicion_objeto((*nivel).herramientas, (*nivel).tope_herramientas, fila_catapulta, columna_catapulta)
    );
}

/*
* Pre condiciones: -
* Post condiciones: 
    -> Carga el valor de la cantidad de niveles que tendra el juego.
*/
void inicializar_cantidad_niveles(juego_t* juego){
    (*juego).nivel_actual = NIVEL_INICIO;
    (*juego).tope_niveles = NIVELES_MAXIMO;
    (*juego).camino_visible = true;    
}

/* 
* Pre condiciones: 
    -> EL valor de 'tope_niveles' debe ser > 0.
* Post condiciones: 
    -> Carga los valores de las estructuras (caminos y paredes) en el vector 'niveles' por cada nivel.   
*/
void inicializar_estructura(nivel_t niveles[MAX_NIVELES], int tope_niveles){
    int tope_paredes = 0;
    int tope_caminos = 0; 

    for (int i = 0; i < tope_niveles; i++){
        
        niveles[i].tope_paredes = tope_paredes;
        niveles[i].tope_camino = tope_caminos;
        
        obtener_mapa (niveles[i].paredes, &niveles[i].tope_paredes, niveles[i].camino, &niveles[i].tope_camino, i+1);
    }
}

/*
* Pre condiciones: 
    -> Los valores de 'fila_dada' debera estar entre 0-19 y 'columna_dada' entre 0-29.
* Post condiciones: 
    -> Carga en 'posicion' (fila, columna) con los valores de la 'fila_dada' y 'columna_dada'.
*/
void cargar_posicion(coordenada_t* posicion, int fila_dada, int columna_dada){
    (*posicion).fil = fila_dada;
    (*posicion).col = columna_dada;
}

/*
* Pre condiciones: -
* Post condiciones: 
    -> Carga la cantidad de herramientas(hechizos, antorchas) con la que el personaje inicia. 
*/
void cargar_cant_herramientas(int* vidas_restantes, int* hechizos_reveladores, int* antorchas){
    (*vidas_restantes) = VIDAS_INICIALES;
    (*hechizos_reveladores) = HECHIZOS_REVELADORES_INICIALES;
    (*antorchas) = ANTORCHAS_INICIALES;  
}

/*
* Pre condiciones: -
* Post condiciones:
    -> Carga el estado (antorcha, pergamino) con la que el persinaje inicia.
*/
void cargar_estado(bool* antorcha_encendida, bool* recolecto_pergamino){
    (*antorcha_encendida) = false;
    (*recolecto_pergamino) = false;
}

/*
* Pre condiciones: 
    -> Los valores de 'posicion_inicial' deben estar inicializado.
* Post condiciones: 
    -> Inicializa los valore del personaje(estado, herramientas, estado).
*/
void inicializar_personaje (personaje_t *personaje, coordenada_t posicion_inicial){    

    cargar_posicion(&(*personaje).posicion, posicion_inicial.fil, posicion_inicial.col);

    cargar_cant_herramientas( &(*personaje).vidas_restantes, &(*personaje).hechizos_reveladores, &(*personaje).antorchas);

    cargar_estado(&(*personaje).antorcha_encendida, &(*personaje).recolecto_pergamino);
}

/*
* Pre condiciones: 
    -> El vector de 'caminos' debe estar inicializado.
    -> El valor de 'tope_caminos' debe ser > 0.
* Post condiciones:
    -> Carga en 'posicion_pergamino' el valor de la posicion aleatoria del pergamino.
*/
void inicializar_pergaminos(coordenada_t caminos[MAX_CAMINO], int tope_caminos, coordenada_t* posicion_pergamino){

    int fil_pergamino = numero_aleatorio(MAX_FILAS);
    int col_pergamino = numero_aleatorio(MAX_COLUMNAS);

    while ( !es_posicion_estructura(caminos, tope_caminos, fil_pergamino, col_pergamino) ){
        fil_pergamino = numero_aleatorio(MAX_FILAS);
        col_pergamino = numero_aleatorio(MAX_COLUMNAS);
    }
    cargar_posicion(&(*posicion_pergamino), fil_pergamino, col_pergamino);
}

/*
* Pre condiciones:
    -> El valor de 'tipo_dado' debera ser TOTEM, PIEDRA_CASTIGO ó CATAPULTA.
    -> Los valores de 'fila_dada' deben estar entre 0-19 y 'columna_dada entre 0-29. 
* Post condicione:
    -> Carga en 'objeto' los valores 'tipo_dado', 'fila_dada', 'columna_dada'.
*/
void cargar_objeto(objeto_t* objeto, int* tope_objeto, char tipo_dado, int fila_dada, int columna_dada){
    (*objeto).tipo = tipo_dado;
    (*objeto).posicion.fil = fila_dada;
    (*objeto).posicion.col = columna_dada;
    (*tope_objeto) ++;
}

/*
* Pre condicones: 
    -> Los 'nivel' deben estar inicializados. 
* Post condicioes: 
    -> Inicializa los 5 totems repatidos aleatoriamente, los totems se encuentran en el camino (excluyendo el inicio, final y la posicion del pergamino).  
*/
void inicializar_herramientas (nivel_t* nivel){
    int tope_camino = (*nivel).tope_camino;
    for (int j = 0; j < TOTEMS_INICIALES; j++){
        int fil_totem = numero_aleatorio(MAX_FILAS);
        int col_totem = numero_aleatorio(MAX_COLUMNAS);
       
        while (es_posicion_para_totem(&(*nivel), tope_camino, fil_totem, col_totem) ){
            fil_totem = numero_aleatorio(MAX_FILAS);
            col_totem = numero_aleatorio(MAX_COLUMNAS);
        }
        cargar_objeto(&(*nivel).herramientas[j], &(*nivel).tope_herramientas, TOTEM, fil_totem, col_totem);
    }
    
}

/*
* Pre condiciones: 
    -> El nivel debe estar inicializado.
    -> El valor de 'tope_camino' debera ser = 0. 
* Post condiciones: 
    -> Inicializa una 10 piedras en posicion aleatoria(perteneciente al camino).
    -> No superponer a otro objeto por cada uno de los tres niveles, carga el tope de los obstaculos por nivel.
*/
void inicializar_piedras_castigo(nivel_t* nivel){
    (*nivel).tope_obstaculos = 0;

    for (int j = 0; j < PIEDRAS_CASTIGO_INCIALES; j++){
        int fil_piedra = numero_aleatorio(MAX_FILAS);
        int col_piedra = numero_aleatorio(MAX_COLUMNAS);

        while ( es_posicion_para_piedra(&(*nivel), fil_piedra, col_piedra)){
            fil_piedra = numero_aleatorio(MAX_FILAS);
            col_piedra = numero_aleatorio(MAX_COLUMNAS);
        }
        cargar_objeto(&((*nivel)).obstaculos[j], &(*nivel).tope_obstaculos, PIEDRA_CASTIGO, fil_piedra, col_piedra);
    }
}

/*
* Pre condiciones: 
    -> El nivel debe estar inicializado.
* Post condiciones: 
    -> Inicializa la posicion de una catapulta. 
    -> La posicion no pertenece a una posicion del camino y distinto de la posicion de una pared. 
*/
void inicializar_catapultas(nivel_t* nivel){
    int indice_catapulta = (*nivel).tope_obstaculos;

    int fil_catapulta = numero_aleatorio(MAX_FILAS);
    int col_catapulta = numero_aleatorio(MAX_COLUMNAS);

    while (es_posicion_para_catapulta(&(*nivel), fil_catapulta, col_catapulta)){
        fil_catapulta = numero_aleatorio(MAX_FILAS);
        col_catapulta = numero_aleatorio(MAX_COLUMNAS);
    }
    cargar_objeto(&((*nivel)).obstaculos[indice_catapulta], &(*nivel).tope_obstaculos, CATAPULTA, fil_catapulta, col_catapulta);    
}

/*
* Pre condiciones: 
    -> El 'nivel' debe estar inicializado.
* Post condciones: 
    -> Inicializa los obstaculos (piedras y catapultas) en 'nivel'.
*/
void inicializar_obstaculos(nivel_t* nivel){

    inicializar_piedras_castigo(&(*nivel));
    inicializar_catapultas(&(*nivel));
}

/*
* Pre condiciones: 
    -> El vector de niveles debe estar inicializado.
    -> El valor de 'tope_niveles' debe ser >0.
* Post condciones: 
    -> Inicializa los niveles con pergaminos, herramientas y obstaculos.
*/
void inicializar_niveles(nivel_t niveles[MAX_NIVELES], int tope_niveles){

    for(int i = 0; i < tope_niveles; i++){
        inicializar_pergaminos( niveles[i].camino, niveles[i].tope_camino, &(niveles[i]).pergamino );
        inicializar_herramientas( &niveles[i]);
        inicializar_obstaculos(&niveles[i]);
    }
}


void inicializar_juego(juego_t* juego) {
    
    inicializar_cantidad_niveles( &(*juego));
    
    inicializar_estructura( (*juego).niveles, (*juego).tope_niveles);
    
    int posicion_nivel = (*juego).nivel_actual-1;
    
    inicializar_personaje( &(*juego).homero, (*juego).niveles[posicion_nivel].camino[0]);

    inicializar_niveles((*juego).niveles, (*juego).tope_niveles);
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
* Post condicones: 
    -> Devuelve -true- si el movimiento es: S, D, A ó W.
    -> Devuelve -false- en caso contrario.
*/
bool es_movimiento(char movimiento){
    return (movimiento == DERECHA || movimiento == ABAJO || movimiento == IZQUIERDA || movimiento == ARRIBA );
}

/*
* Pre condiciones: -
* Post condiciones: 
    -> Devuelve -true- si fil_movimiento esta entre 0-19 y col_movimiento esta entre 0-29.
*/
bool es_posicion_mapa(int fil_movimiento, int col_movimiento){
    return (fil_movimiento >= MIN_FIL_POSICION && fil_movimiento < MAX_FIL_POSICION) && (col_movimiento >= MIN_COL_POSICION && col_movimiento < MAX_COL_POSICION);
}

/*
* Pre condicones: -
* Post condiciones: 
    -> Devuelve -true- si valor del 'movimiento' es una herramienta (H ó L). 
    -> Devuelve -false- en caso contrario. 
*/
bool es_herramienta(char movimiento){
    return (movimiento == HECHIZO_REVELADOR || movimiento == ANTORCHA);
}

/*
* Pre condiciones: 
    -> El vector de 'objetos' debe estar inicializado.
    -> El valor 'tope_objetos' debe ser > 0.
    -> El valor de 'fil_movimiento' debe estar entre (0-19) y 'col_movimiento' debe estar entre (0-29).  
* Post condiciones: 
    -> Devuelve el valor del indice de la posicion en la que se encuentra el 'objeto'.
*/
int busca_indice_objeto(objeto_t objetos[MAX_ELEMENTOS], int tope_objetos, int fil_movimiento, int col_movimiento){
    int posicion = -1;
    bool posicion_encontrada = false;
    
    while(!posicion_encontrada && (posicion < tope_objetos)) {

        if (objetos[posicion].posicion.fil == fil_movimiento && objetos[posicion].posicion.col == col_movimiento){
            posicion_encontrada = true;
        } else {
            posicion ++;
        }
    }
    return posicion;
}

/*
* Pre condiciones: 
    -> El vector de 'posiciones' debe estar inicializado.
    -> El valor de 'tope_posiciones' debe ser > 0.
    -> fil_camino debe estar entre (0-19) y col_camino debe estar entre (0-29)  
* Post condiciones: 
    -> Devuelve el valor del indice de la posicion en la que se encuentra el camino.
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
* Pre condiciones: 
    -> 'juego' debe estar inicializado.
    -> El vector 'camino' debe estar inicializado.
    -> El valor de 'tope_camino' debe ser > 0.
* Post condiciones: 
    -> Actualiza la posicion de 'pergamino' ubicandolo en la ultima posicion del 'camino'.
    -> Actualiza el estado del personaje. 
*/
void recoger_pergamino(coordenada_t* pergamino, coordenada_t camino[MAX_CAMINO], int tope_camino){
    int posicion_altar = tope_camino -1;
    int fil_altar = camino[posicion_altar].fil;
    int col_altar = camino[posicion_altar].col;

    (*pergamino).fil = fil_altar;
    (*pergamino).col = col_altar;
}

/*
* Pre condiciones: 
    -> -
* Post condiciones: 
    -> Actualiza los valores de dados por parametros cambiando a true si el pergamino fue tomado.
*/
void guardar_pergamino(bool* camino_visible, bool* antorcha_encendida, bool* recolecto_pergamino){
    (*camino_visible) = true;
    (*antorcha_encendida) = false;     
    (*recolecto_pergamino) = true;
}

/*
* Pre condiciones: -
* Post condiciones: 
    -> Carga los valores del 'movimiento_dado' en 'fila_movimiento' y 'col_movimiento'.  
*/
void valorar_movimiento(char movimiento_dado, int* fila_movimiento, int* columna_movimiento){
    switch (movimiento_dado){
        case 'W':
            (*fila_movimiento) = -1;
            break;
        case 'S':
            (*fila_movimiento) = +1;
            break;
        case 'A':
            (*columna_movimiento) = -1;
            break;
        case 'D':
            (*columna_movimiento) = +1;
            break;   
        default:
            break;
    }
}

/*
* Pre condiciones: 
    -> La 'posicion' del personaje debe estar inicializado.
    -> Los valores de 'fila_movimiento' y 'columna_movimiento' deben ser > 0.
* Post condiciones: 
    -> Actualiza la 'posicion' del personaje de sumando los valores del movimiento (fil_movimineto y col_movimientos) dados.
*/
void posicionar_personaje(coordenada_t* posicion, int fil_movimiento, int col_movimiento){
    (*posicion).fil += fil_movimiento;
    (*posicion).col += col_movimiento;
}

/*
* Pre condiciones: 
    -> El valor de 'vidas_restantes' deben estar inicializado.
    -> La 'herramienta' debe estar inicializada.
* Post condiciones: 
    -> Actualiza el valor de 'vidas_restantes' incrementadolo en una unidad. 
*/
void tomar_herramienta(int *vidas_restantes, objeto_t herramienta){
    if (herramienta.tipo == TOTEM){
        (*vidas_restantes) ++;
    }
}

/*
* Pre condiciones: 
    -> El vector de objetos debe estar inicializado.
    -> El valor de 'tope_objetos' debe ser > 0.
    -> EL valor de 'posicion_objeto' debera ser >= 0.   
* Post condiciones: 
    -> Actualiza el vector de 'objetos' eliminando ordenadamente el objeto con el indice 'posicion_objeto'.
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
* Pre condiciones: 
    -> -   
* Post condiciones: 
    -> Actualiza los valores de los parametro, 'recolecto_pergamino' a false.
*/
void tirar_pergamino(bool* camino_visible, bool* antorcha_encendida, bool* recolecto_pergamino){
    (*camino_visible) = false;
    (*antorcha_encendida) = false;     
    (*recolecto_pergamino) = false;
}


/*
* Pre condiciones: 
    -> El vector de 'caminos' debe estar inicializado.
    -> El valor del 'tope_caminos' debe ser >0.
    -> EL valor de la posicion del camino debe ser >= 0.
* Post condiciones: 
    -> Actualiza el vector 'caminos' eliminando(ordenada) el camino con el indice 'posicion_camino'.
*/
void eliminar_camino(coordenada_t caminos[MAX_CAMINO], int* tope_caminos, int posicion_camino){
    for (int i = posicion_camino; i < (*tope_caminos)-1 ; i++){
        caminos[i].fil = caminos[i+1].fil;
        caminos[i].col = caminos[i+1].col;
    }
    (*tope_caminos) -= 1;
}

/*
* Pre condiciones: 
    -> El 'nivel' debe estar inicializado.
    -> El valor de 'tope_caminos' debe ser > 0.
    -> Los valores de 'posicion personaje' debe estar inicializado.
* Post condiciones: 
    -> Actualiza el vector de caminos eliminando un camino de manera aleatoria.
*/
void lanzar_bola_fuego( nivel_t* nivel, int* tope_caminos, coordenada_t posicion_personaje){
    int fil_bola = numero_aleatorio(MAX_FILAS);
    int col_bola = numero_aleatorio(MAX_COLUMNAS);
    
    while (
        es_posicion_estructura((*nivel).paredes, (*nivel).tope_paredes, fil_bola, col_bola) || 
        es_posicion_elemento( (*nivel).camino[0], fil_bola, col_bola)  || 
        es_posicion_elemento( (*nivel).camino[(*tope_caminos)-1], fil_bola, col_bola) || 
        es_posicion_elemento(posicion_personaje, fil_bola, col_bola) ){
            fil_bola = numero_aleatorio(MAX_FILAS);
            col_bola = numero_aleatorio(MAX_COLUMNAS);
    }
    
    if (es_posicion_estructura((*nivel).camino, *tope_caminos, fil_bola, col_bola)){
        int indice_posicion_camino;
        indice_posicion_camino = busca_indice_posicion((*nivel).camino, (*nivel).tope_camino, fil_bola, col_bola);
        eliminar_camino((*nivel).camino, &(*nivel).tope_camino, indice_posicion_camino);        
    }
    
}


/*
* Pre condiciones: 
    -> -   
* Post condiciones: 
    -> Actualiza los valores de los parametros, 'antorcha_encendida' a true.
*/
void activar_runa(bool* camino_visible, bool* antorcha_encendida){
    (*camino_visible) = true;
    (*antorcha_encendida) = false;
}

/*
* Pre condiciones: 
    -> 'nivel' debe estar inicializado.
    -> El valor de 'indice_herramienta' debe ser >0.
    -> Los valores de 'nueva_fila' debe estar entre 0-19 y nueva columna entre 0-29
* Post condiciones:
    -> Actualiza el valor de 'vidas_restantes'
    -> Actualiza el vector de herramientas eliminando ordenadamente el valor de posicion en 'indice_herramienta'.
*/
void recoger_herramienta(int* vidas_restantes, nivel_t* nivel, int indice_herramienta, int nueva_fila, int nueva_columna){
    if (indice_herramienta >= 0){

        if ( es_posicion_estructura((*nivel).camino, (*nivel).tope_camino, nueva_fila, nueva_columna)){
            tomar_herramienta(&(*vidas_restantes), (*nivel).herramientas[indice_herramienta]);
            eliminar_objeto((*nivel).herramientas, &(*nivel).tope_herramientas, indice_herramienta);        
        } else {
            eliminar_objeto((*nivel).herramientas, &(*nivel).tope_herramientas, indice_herramienta);        
        }
                      
    }
}

/*
* Pre condiciones:  
    -> El 'nivel' debe estar inicializado.
    -> El valor de 'tope_camino' debera ser > 0.
* Post condiciones: 
    -> Actualiza los valores de 'posicion del pergamino' en una posicion aleatoria, sobre el camino (sin superponerse a cualquier obstaculo u herramienta).
*/
void posicionar_pergamino(nivel_t nivel, coordenada_t* posicion_pergamino, int tope_camino){
    
    int fil_pergamino = numero_aleatorio(MAX_FILAS);
    int col_pergamino = numero_aleatorio(MAX_COLUMNAS);
    
    while (es_posicion_elemento(nivel.camino[tope_camino-1], fil_pergamino, col_pergamino) || es_posicion_elemento(nivel.camino[0], fil_pergamino, col_pergamino) || es_posicion_objeto(nivel.herramientas, nivel.tope_herramientas, fil_pergamino, col_pergamino) || es_posicion_objeto(nivel.obstaculos, nivel.tope_obstaculos, fil_pergamino, col_pergamino) || !(es_posicion_estructura(nivel.camino, tope_camino, fil_pergamino, col_pergamino))){
        fil_pergamino = numero_aleatorio(MAX_FILAS);
        col_pergamino = numero_aleatorio(MAX_COLUMNAS);    
    }

    (*posicion_pergamino).fil = fil_pergamino;
    (*posicion_pergamino).col = col_pergamino;

}

/*
* Pre condiciones: 
    -> -   
* Post condiciones: 
    -> Actualiza los valores de los parametros, restando una unidad a 'vidas_restantes'.
*/
void salir_camino(int* vidas_restantes, bool* antorcha_encendida, bool* camino_visible){
    (*vidas_restantes) --;
    (*antorcha_encendida) = false;
    (*camino_visible) = false;
}

/*
* Pre condiciones: 
    -> -   
* Post condiciones: 
    -> Actualiza los valores de los parametros, 'antorcha_encendida' y 'camino_visble' a false, .
*/
void desactivar_herramientas(bool* antorcha_encendida, bool* camino_visible){
    (*antorcha_encendida) = false;
    (*camino_visible) = false;
}


/*
* Pre condiciones: 
    -> 'juego' y 'nivel' deben estar inicializados.
    -> El valor de 'fila_personaje' debe estar entre 0-190 y 'columna_personaje' entre 0-29.
* Post condiciones: 
    -> Carga en 'juego' y 'nivel' la acción correspondiente a la posicion(camino) del personaje(pergamino, runa, herramienta, obstaculo, camino)
*/
void accionar_camino(juego_t* juego, nivel_t* nivel, int fila_personaje, int columna_personaje){

    if (es_posicion_elemento((*nivel).pergamino, fila_personaje, columna_personaje)){
        recoger_pergamino( &(*nivel).pergamino, (*nivel).camino, (*nivel).tope_camino);
        guardar_pergamino(&(*juego).camino_visible, &(*juego).homero.antorcha_encendida, &(*juego).homero.recolecto_pergamino);

    } else if ( es_posicion_elemento((*nivel).camino[0], fila_personaje, columna_personaje) ){
        lanzar_bola_fuego(&(*nivel), &(*nivel).tope_camino, (*juego).homero.posicion);
        activar_runa(&(*juego).camino_visible, &(*juego).homero.antorcha_encendida);

    } else if (es_posicion_objeto((*nivel).herramientas, (*nivel).tope_herramientas, fila_personaje, columna_personaje) ){
        int indice_herramienta = busca_indice_objeto((*nivel).herramientas,( *nivel).tope_herramientas, fila_personaje, columna_personaje);
        recoger_herramienta(&(*juego).homero.vidas_restantes, &(*nivel), indice_herramienta, fila_personaje, columna_personaje);
        desactivar_herramientas(&(*juego).homero.antorcha_encendida, &(*juego).camino_visible);
        
    } else if (es_posicion_objeto((*nivel).obstaculos, (*nivel).tope_obstaculos-1, fila_personaje, columna_personaje)){
        int indice_obstaculo = busca_indice_objeto((*nivel).obstaculos, (*nivel).tope_obstaculos, fila_personaje, columna_personaje);
        posicionar_pergamino((*nivel), &(*nivel).pergamino, (*nivel).tope_camino );
        eliminar_objeto((*nivel).obstaculos, &(*nivel).tope_obstaculos, indice_obstaculo);
        tirar_pergamino(&(*juego).camino_visible, &(*juego).homero.antorcha_encendida, &(*juego).homero.recolecto_pergamino);
        
    }  else {
        desactivar_herramientas(&(*juego).homero.antorcha_encendida, &(*juego).camino_visible);
    }

}

/*
* Pre condiciones: 
    -> 'juego' y 'nivel' deben estar inicializados.
    -> El valor de 'fila_personaje' debe estar entre 0-190 y 'columna_personaje' entre 0-29.
* Post condiciones: 
    -> Carga en 'juego' y 'nivel' la acción correspondiente a la posicion(fuera de camino) del personaje(herramienta, obstaculo, fuera_camino)
*/
void accionar_posicion(juego_t*juego, nivel_t* nivel, int fila_personaje, int columna_personaje){

    if (es_posicion_objeto((*nivel).herramientas, (*nivel).tope_herramientas, fila_personaje, columna_personaje) ){
        int indice_herramienta = busca_indice_objeto((*nivel).herramientas,( *nivel).tope_herramientas, fila_personaje, columna_personaje);
        recoger_herramienta(&(*juego).homero.vidas_restantes, &(*nivel), indice_herramienta, fila_personaje, columna_personaje);
        desactivar_herramientas(&(*juego).homero.antorcha_encendida, &(*juego).camino_visible);
        
    } else if (es_posicion_objeto((*nivel).obstaculos, (*nivel).tope_obstaculos-1, fila_personaje, columna_personaje)){
        int indice_obstaculo = busca_indice_objeto((*nivel).obstaculos, (*nivel).tope_obstaculos, fila_personaje, columna_personaje);
        posicionar_pergamino((*nivel), &(*nivel).pergamino, (*nivel).tope_camino );
        eliminar_objeto((*nivel).obstaculos, &(*nivel).tope_obstaculos, indice_obstaculo);
        tirar_pergamino(&(*juego).camino_visible, &(*juego).homero.antorcha_encendida, &(*juego).homero.recolecto_pergamino);
        
    }  else {
        desactivar_herramientas(&(*juego).homero.antorcha_encendida, &(*juego).camino_visible);
        salir_camino(&(*juego).homero.vidas_restantes, &(*juego).homero.antorcha_encendida, &(*juego).camino_visible);
    }

}

/*
* Pre condiciones: 
    -> 'nivel', 'juego' deben estar inicializado.
    -> Los valores de 'posicion_personaje' deben estar inicializado.
    -> Los valores de 'fil_movimiento' debe estar entre 0-19, 'col_movimiento' entre 0-29.
* Post condciciones: 
    -> Actualiza los valores del juego de acuerdo a los valores de la posicion en la que se movera el personaje.
    -> Actualiza los valores de 'posicion_personaje' con los valores de 'fil_movimiento' y 'col_movimiento'.
*/
void dirigir_movimiento(nivel_t* nivel, coordenada_t* posicion_personaje, juego_t* juego, int fil_movimiento, int col_movimiento){

    int nueva_fil_personaje = fil_movimiento + (*posicion_personaje).fil;
    int nueva_col_personaje = col_movimiento + (*posicion_personaje).col;
    if (es_posicion_mapa(nueva_fil_personaje, nueva_col_personaje)){

        if (es_posicion_estructura((*nivel).camino, (*nivel).tope_camino, nueva_fil_personaje, nueva_col_personaje)){
            accionar_camino(&(*juego), &(*nivel), nueva_fil_personaje, nueva_col_personaje);
        } else if (!es_posicion_estructura((*nivel).camino, (*nivel).tope_camino, nueva_fil_personaje, nueva_col_personaje) && !es_posicion_estructura((*nivel).paredes, (*nivel).tope_paredes, nueva_fil_personaje, nueva_col_personaje)){
            accionar_posicion(&(*juego), &(*nivel), nueva_fil_personaje, nueva_col_personaje); 
        }
        posicionar_personaje(&(*juego).homero.posicion, fil_movimiento, col_movimiento);
    }
}

/*
* Pre condiciones: 
    -> 'juego' debe estar incializado.
* Post condiciones:
    -> Actualiza el 'juego' segun el movimiento dado (moverse o utilizar una herramienta).
*/
void realizar_jugada(juego_t* juego, char movimiento){

    int fil_movimiento = 0;
    int col_movimiento = 0;
    int posicion_nivel_actual = (*juego).nivel_actual -1;

    if (es_movimiento(movimiento)){
        valorar_movimiento(movimiento, &fil_movimiento, &col_movimiento);
        dirigir_movimiento(&(*juego).niveles[posicion_nivel_actual], &(*juego).homero.posicion, &(*juego), fil_movimiento, col_movimiento);

    } else if (es_herramienta(movimiento)){

        if ((movimiento == HECHIZO_REVELADOR) && ((*juego).homero.hechizos_reveladores > 0) && !(*juego).camino_visible && !(*juego).homero.antorcha_encendida && !(es_posicion_elemento((*juego).homero.posicion, (*juego).niveles[posicion_nivel_actual].camino[0].fil, (*juego).niveles[posicion_nivel_actual].camino[0].col) )){
            lanzar_bola_fuego(&(*juego).niveles[posicion_nivel_actual], &(*juego).niveles[posicion_nivel_actual].tope_camino, (*juego).homero.posicion);
            (*juego).homero.hechizos_reveladores --;
            (*juego).homero.antorcha_encendida = false;
            (*juego).camino_visible = true;

        } else if ((movimiento == ANTORCHA) && ((*juego).homero.antorchas > 0) && !(*juego).homero.antorcha_encendida && !(*juego).camino_visible && !(es_posicion_elemento((*juego).homero.posicion, (*juego).niveles[posicion_nivel_actual].camino[0].fil, (*juego).niveles[posicion_nivel_actual].camino[0].col))){
            (*juego).homero.antorcha_encendida = true;
            (*juego).homero.antorchas -= 1;
        }
    }
}


/*
* Pre condiciones: 
    -> El vector de notas debe estar vacia;
* Post condiciones: 
    -> Carga en 'nota' un mensaje dee forma aleatoria entre (primer o sexto) mensaje.
*/
void mostrar_mensaje_informacion(char nota[MAX_CARACTERES]){
    int numero_mensaje = numero_aleatorio(MAX_MENSAJES_INFORMATIVO-1);
    switch (numero_mensaje){
    case 0:
        strcpy(nota, MSJ_INFORMATIVO1);
        break;
    case 1:
        strcpy(nota, MSJ_INFORMATIVO2);
        break;
    case 2:
        strcpy(nota, MSJ_INFORMATIVO3);
        break;
    case 3:
        strcpy(nota, MSJ_INFORMATIVO4);
        break;
    case 4:
        strcpy(nota, MSJ_INFORMATIVO5);
        break;
    case 5:
        strcpy(nota, MSJ_INFORMATIVO6);
        break;
    default:
        break;
    }        
}

/*
* Pre condiciones: 
    -> 'juego' debe estar inicializado.
* Post condiciones: 
    -> imprime por pantalla mensajes de interaccion con el usuario (bienvenida, consejos de juego).
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

    if (es_posicion_elemento(juego.homero.posicion, juego.niveles[posicion_nivel_actual].camino[0].fil, juego.niveles[posicion_nivel_actual].camino[0].col)){
        strcpy(nota,MSJ_POSICION_RANURA);
    } else if (es_posicion_elemento(juego.niveles[posicion_nivel_actual].camino[tope_camino-1], fil_homero, col_homero)){
        strcpy(nota, MSJ_ALTAR_SIN_PERGAMINO);
    } else if (!es_posicion_estructura(juego.niveles[posicion_nivel_actual].camino, tope_camino, fil_homero, col_homero)){
        strcpy(nota, MSJ_POSICION_FUERA_CAMINO);
    } else if (es_posicion_estructura(juego.niveles[posicion_nivel_actual].camino, tope_camino, fil_homero, col_homero)){
        mostrar_mensaje_informacion(nota);
    }  else {
        printf("%s\n\n\n%s\n", MSJ_DECORADOR, MSJ_DECORADOR);
    }
    printf("%s\n%s\n%s\n", MSJ_DECORADOR, nota, MSJ_DECORADOR);
}

/*
* Pre condiciones: -
* Post condiciones: 
    -> Inicaliza los valores de la matriz del mapa con elementos vacios. 
*/
void inicializar_mapa( emoji_t mapa[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            strcpy(mapa[i][j].representacion, EMOJI_VACIO);
        }
    }
}

/*
* Pre condiciones: 
    -> 'mapa' y 'paredes' deben estar inicializado
    -> El valor de 'tope_paredes' debe ser > 0.
* Post condiciones:
    -> Carga paredes(emoji) en 'mapa' segun las posiciones(fila, columna) de cada elemento de 'paredes'. 
*/
void agregar_paredes(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t paredes[MAX_PAREDES], int tope_paredes){
    for (int i = 0; i < tope_paredes; i++){
        int fil_pared = paredes[i].fil;
        int col_pared = paredes[i].col;

        strcpy(mapa[fil_pared][col_pared].representacion, EMOJI_PARED);
    }
}


/*
* Pre condiciones: 
    -> 'mapa', 'caminos y 'paredes' deben estar inicializados.
    -> El valor de 'tope_caminos' y 'tope_paredes' debe ser > 0.
* Post condiciones: 
    -> Carga fuego(emoji) en 'mapa' segun no sean las posiciones(fila, columna) del camino ó la pared.
*/
void agregar_fuego(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t caminos[MAX_CAMINO], int tope_caminos, coordenada_t paredes[MAX_CAMINO], int tope_paredes){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            if (!es_posicion_estructura(caminos, tope_caminos, i, j) && !es_posicion_estructura(paredes, tope_paredes, i, j)){
                strcpy(mapa[i][j].representacion, EMOJI_FUEGO);
            }
        }
    }
}

/*
* Pre condiciones: 
    -> 'mapa' y 'camino' deben estar inicializado
    -> El valor de 'tope_camino' debe ser > 0.
* Post condiciones:
    -> Carga caminos(emoji) en 'mapa' segun las posiciones(fila, columna) de cada elemento de 'camino'. 
*/
void agregar_camino(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino){
    for (int i = 0; i < tope_camino; i++){
        int fil_camino = camino[i].fil;
        int col_camino = camino[i].col;
        strcpy(mapa[fil_camino][col_camino].representacion, EMOJI_CAMINO);
    }
}

/*
* Pre condiciones: 
    -> 'mapa', 'caminos', 'paredes' deben estar inicializados.
    -> Los valores de 'tope_camino' y 'tope_paredes' debe ser > 0.
    -> La posicion del personaje debe ser dentro de los limites de la matriz mapa.  
* Post condiciones: 
    -> Carga en 'mapa' los valores de 'caminos' y 'paredes' en un rango manhattan 3 segun la posicion del personaje.
*/
void agregar_camino_manhattan(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t caminos[MAX_CAMINO], int tope_caminos, coordenada_t paredes[MAX_CAMINO], int tope_paredes, coordenada_t posicion){
    int fil_homero = posicion.fil;
    int col_homero = posicion.col;
    int rango = RANGO_MANHATTAN;

    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){

            if ( ((fil_homero - i) <= rango) && ((col_homero - j) <= rango) && ((fil_homero - i) >= -rango) && ((col_homero - j) >= -rango)){

                int fil_coordenada = fil_homero - i;
                int col_coordenada = col_homero - j;
        
                if  ( ((fil_coordenada + col_coordenada) <= rango && (fil_coordenada - col_coordenada) >= -rango ) && ((fil_coordenada - col_coordenada) <= rango && (fil_coordenada + col_coordenada) >= -rango )){

                    if (es_posicion_estructura(caminos, tope_caminos, i, j)){
                        strcpy(mapa[i][j].representacion, EMOJI_CAMINO);
                    } else if (!es_posicion_estructura(paredes, tope_paredes, i, j)) {
                        strcpy(mapa[i][j].representacion, EMOJI_FUEGO);
                    }
                }

            }
        }
    }
}

/*
* Pre condiciones: 
    -> 'mapa' , 'personaje' deben ser inicializado.
* Post condiciones: 
    -> Carga en 'mapa' los valores del personaje segun su posicion(fila, columna) en el que se encuentra.
*/
void agregar_personaje(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_personaje){
    int fil_homero = posicion_personaje.fil;
    int col_homero = posicion_personaje.col;

    strcpy(mapa[fil_homero][col_homero].representacion, EMOJI_HOMERO);
}

/*
* Pre condiciones: 
    -> 'mapa', 'camino', 'posicion_pergamino' deben estar inicializados.
    -> El valor de 'tope_camino' debe ser > 0.
* Post condicones: 
    -> Carga en 'mapa' los valores de la runa(emoji), el altar(emoji) y pergamino(emji) segun sus posiciones(fila, columna).  
*/
void agregar_items(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino, coordenada_t posicion_pergamino){

    int fil_camino_inicial = camino[0].fil;
    int col_camino_inicial = camino[0].col;
    strcpy(mapa[fil_camino_inicial][col_camino_inicial].representacion, EMOJI_RUNA);

    int fil_camino_final = camino[tope_camino-1].fil;
    int col_camino_final = camino[tope_camino-1].col;
    strcpy(mapa[fil_camino_final][col_camino_final].representacion, EMOJI_ALTAR);
    
    int fil_pergamino = posicion_pergamino.fil;
    int col_pergamino = posicion_pergamino.col;
    if (!es_posicion_elemento(camino[tope_camino-1], fil_pergamino, col_pergamino)){
        strcpy(mapa[fil_pergamino][col_pergamino].representacion, EMOJI_PERGAMINO);
    }
}

/*
* Pre condiciones: 
    -> 'mapa', 'obstaculos' deben estar inicializados
    -> El valor de 'tope_obstaculos' debe ser > 0;
* Post condiciones: 
    -> Carga en 'mapa' los obstaculos(emoji) segun su posicion de 'obstaculos'(fila, columna) correspondiente.
*/
void agregar_obstaculos(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], objeto_t obstaculos[MAX_ELEMENTOS], int tope_obstaculos){
    for (int i = 0; i < tope_obstaculos; i++){
        int piedra_fil = obstaculos[i].posicion.fil;
        int piedra_col = obstaculos[i].posicion.col; 
        if (obstaculos[i].tipo == PIEDRA_CASTIGO){

            strcpy(mapa[piedra_fil][piedra_col].representacion, EMOJI_PIEDRA);

        } else if (obstaculos[i].tipo == CATAPULTA) {
            strcpy(mapa[piedra_fil][piedra_col].representacion, EMOJI_CATAPULTA);
        }
    }
}

/*
* Pre condiciones: 
    -> 'mapa',  'herramientas' deben estar inicializados
    -> El valor de 'tope_herramientas' debe ser > 0;
* Post condiciones: 
    -> Carga en 'mapa' las herramientas(emoji) segun la posicion de 'herramientas'(fila, columna) correspondiente.
*/
void agregar_herramientas(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], objeto_t herramientas[MAX_ELEMENTOS], int tope_herramientas){
    
    for (int i = 0; i < tope_herramientas; i++){
        int totem_fil = herramientas[i].posicion.fil;
        int totem_col = herramientas[i].posicion.col;
        strcpy(mapa[totem_fil][totem_col].representacion, EMOJI_TOTEM);
    }
}

/*
* Pre condiciones: 
    -> 'niveles', 'personaje' debe estar incializado.
    -> El valor de 'indice_nivel' debe ser >= 0.
* Post condiciones: 
    -> Crea el mapa('mapa') del juego de acuerdo al nivel('indice_nivel') completo correspondiente.
*/
void crear_mapa(emoji_t mapa[MAX_FILAS][MAX_COLUMNAS], nivel_t nivel, personaje_t personaje, bool camino_visible){
    
    inicializar_mapa(mapa);
  
    agregar_paredes(mapa, nivel.paredes, nivel.tope_paredes);

    if (camino_visible){
        agregar_fuego(mapa, nivel.camino, nivel.tope_camino,nivel.paredes, nivel.tope_paredes);
        agregar_camino(mapa, nivel.camino, nivel.tope_camino);

    } else if (personaje.antorcha_encendida){
        agregar_camino_manhattan(mapa, nivel.camino, nivel.tope_camino, nivel.paredes, nivel.tope_paredes, personaje.posicion);
    }

    agregar_personaje(mapa, personaje.posicion);
    
    agregar_items(mapa, nivel.camino, nivel.tope_camino, nivel.pergamino);

    agregar_obstaculos(mapa, nivel.obstaculos, nivel.tope_obstaculos);

    agregar_herramientas (mapa, nivel.herramientas, nivel.tope_herramientas);
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

    if ((vidas >= 0) && (nivel_actual == 3 && juego.homero.recolecto_pergamino) && (es_posicion_elemento(juego.niveles[posicion_nivel_actual].camino[posicion_altar], fil_homero, col_homero))){
        return ganado;
    } else if ((nivel_actual < 4) && (vidas <= 0)){
        return perdido;
    } else {
        return jugando;
    }

}

/*
* Pre condicones: 
    -> 'mapa' debe ser inicializado.
* Post condiciones: 
    -> Imprime por pantalla el 'mapa' del juego.
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
* Pre condicones: 
    -> El valor de 'cantidad_emojis' debe ser >0
    -> 'cadena' debe estar vacio.
    -> 'emoji' debe estar en formato codigo. 
* Post condiciones: 
    -> Carga emojis en 'cadena' segun la cantidad_emojis dada.
*/
void agregar_emojis(int cantidad_emojis, char cadena[MAX_CARACTERES], char* emoji){
    for(int i = 0; i < cantidad_emojis; i++){
        strcat(cadena, emoji);
    }
}

/*
* Pre condicones: - 
* Post condiciones: 
    -> Imprime por pantalla los datos(emojis) con el estado del usuario (cantidad vidas, antorchas, hechizos, pergaminos).
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
    int indice_nivel = juego.nivel_actual -1;
    mostrar_bienvenida(juego);

    crear_mapa (mapa, juego.niveles[indice_nivel], juego.homero, juego.camino_visible);
    mostrar_mapa(mapa);
    mostrar_datos_usuario(juego.homero.vidas_restantes, juego.homero.hechizos_reveladores, juego.homero.antorchas, juego.homero.recolecto_pergamino);
}
