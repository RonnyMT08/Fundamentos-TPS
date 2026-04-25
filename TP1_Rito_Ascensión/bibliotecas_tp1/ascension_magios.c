//gcc juego.c ascension_magios.c utiles.o -o juego -std=c99 -Wall -Wconversion -Werror -lm

#include "ascension_magios.h"
#include "utiles.h"
#include <stdio.h>
#include <stdlib.h> // Para usar rand, limpiar terminal, etc.
#include <time.h>   // Para obtener una semilla desde el reloj

#define MAX_CARACTERES 100

const char HOMERO = 'H';
const char PARED = 'X';
const char RUNA = 'U';
const char ALTAR = 'A';
const char PERGAMINO = 'P';
const char TOTEM = 'T';
const char CAMINO = '-';
const char VACIO = ' ';
const char PIEDRA_CASTIGO = 'R';

const char* MSJ_BIENVENIDA = "--Hola Homero, Bienvenido al juego de ascencion de magios--\n";
const char* MSJ_MOVIMIENTOS = "      -W-          -S-            -A-           -D- \n     Arriba       Abajo        Izquierda      Derecha \n";

const char* MSJ_ELIMINACION = "\n\nELIMINADO...!!\n\nTe quedaste sin vidas\n";
const char* MSJ_JUEGO_GANADO = "\n En hora buena...\n\n tú ascención fue aprobada, pronto nos comunicaremos contigo\n";

/* 
* Pre condiciones: los minimos y maximos deben ser > 0
* Post condiciones: devuelve un numero aleatorio entre minio y maximo inclusive.
*/
int numero_aleatorio(int minimo, int maximo){
    int numero = rand() % (maximo - minimo + 1) + minimo;
    return numero;
}

/* 
* Pre condiciones: - 
* Post condiciones: carga los niveles, paredes, caminos y sus topes, para los tres niveles.
*/
void inicializar_estructura(nivel_t* niveles){
    int tope_paredes = 0;
    int tope_caminos = 0; 

    for (int i = 0; i < MAX_NIVELES; i++){
        int nivel_actual = i+1;

        niveles[i].tope_paredes = tope_paredes;
        niveles[i].tope_camino = tope_caminos;
        
        obtener_mapa(niveles[i].paredes, &niveles[i].tope_paredes, niveles[i].camino, &niveles[i].tope_camino, nivel_actual);
    }

}

/*
* Pre condiciones: -
* Post condiciones: carga un personaje con las su pocision, vidas, hechizos, antorchas
*/
void inicializar_homero (personaje_t *personaje, coordenada_t posicion){
    int vidas = 5;
    int hechizos = 5;
    int antorchas = 5;
    
    int camino_fil = posicion.fil;
    int camino_col = posicion.col;
    
    (*personaje).posicion.fil = camino_fil;
    (*personaje).posicion.col = camino_col;

    (*personaje).vidas_restantes = vidas;
    (*personaje).hechizos_reveladores = hechizos;
    (*personaje).antorchas = antorchas;
    (*personaje).antorcha_encendida = false;
}

/*
* Pre condiciones: -
* Post condiciones: carga la posicion del pergamino para los tres niveles.
*/
void inicializar_items(nivel_t niveles[MAX_NIVELES]){
    // PERGAMINO
    int total_pergaminos = 3;

    for (int i = 0; i < total_pergaminos; i++){

        int tope_camino = niveles[i].tope_camino;
        //la posicion esta entre 1 y antes de la posicion final
        int posicion_pergamino = numero_aleatorio(1, tope_camino-2);

        int pergamino_fil = niveles[i].camino[posicion_pergamino].fil;
        int pergamino_col = niveles[i].camino[posicion_pergamino].col;
        
        niveles[i].pergamino.fil = pergamino_fil;
        niveles[i].pergamino.col = pergamino_col;        
    }
}

/*
Pre condicones: -
Post condicioes:  Falta hacerrEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRRRR
*/
void inicializar_herramientas(nivel_t niveles[MAX_NIVELES]){
    /*
    //hechizo revelador (5 oportinidades)
    for (int i = 0; i < 3; i++){
        (*juego).niveles[i].herramientas[1].tipo = 'E'; //existen 3 herramientas echizo, totems, antorchas
        (*juego).niveles[i].herramientas[1].posicion.fil = 0;
        (*juego).niveles[i].herramientas[1].posicion.col = 1;
    }

    //antorcha(5 antorchas)
    for (int i = 0; i < 3; i++){
        (*juego).niveles[i].herramientas[2].tipo = 'T'; //existen 3 herramientas echizo, totems, antorchas
        (*juego).niveles[i].herramientas[2].posicion.fil = 0;
        (*juego).niveles[i].herramientas[2].posicion.col = 2;
    }
    */
   
   //totem(5 repartidos de manera aleatoria en cada nivel) en el camino
    int tope_niveles = MAX_NIVELES;

    for (int i = 0; i < tope_niveles; i++){
    int tope_camino = niveles[i].tope_camino;
        
        for (int j = 0; j < 5; j++){
            int posicion_camino = numero_aleatorio(1, tope_camino-2); // -2 por que en el tope no hay nada, y ultima es el altar
            int fil_totem = niveles[i].camino[posicion_camino].fil;
            int col_totem = niveles[i].camino[posicion_camino].col;
            
            niveles[i].herramientas[j].tipo = TOTEM; //existen 3 herramientas echizo, totems, antorchas
            niveles[i].herramientas[j].posicion.fil = fil_totem;
            niveles[i].herramientas[j].posicion.col = col_totem;
        }
    }
}

/*
* Pre condiciones: -
* Post condciones: carga PIEDRA DEL CASTIGO, CATAPULCA FALTA HACERRRRR!!!!!!!!!!!!!!!!!!
*/
void inicializar_obtaculos(nivel_t niveles[MAX_NIVELES], int tope_niveles){

    int posicion_camino = numero_aleatorio(2,5);
    int cantidad_obstaculos_nivel = 10;
    int tope_obstaculos = 0;
    // piedra del catigo (10 en cada nivel de manera aleatoria)
    
    for (int i = 0; i < tope_niveles; i++){
        int tope_camino = niveles[i].tope_camino;
     //   niveles[i].tope_obstaculos = cantidad_obstaculos;

        for (int j = 0; j < cantidad_obstaculos_nivel; j++){
            posicion_camino =  numero_aleatorio(1, tope_camino-2);
            
            // asigno la fila, columna del camino a la piedra
            int fil_piedra = niveles[i].camino[posicion_camino].fil;
            int col_piedra = niveles[i].camino[posicion_camino].col;
            
            /// la piedra en el camino
            niveles[i].obstaculos[j].tipo = PIEDRA_CASTIGO;
            // asigno a la piedra la fila y columna 
            niveles[i].obstaculos[j].posicion.fil = fil_piedra;
            niveles[i].obstaculos[j].posicion.col = col_piedra;
            tope_obstaculos ++;
        }
        niveles[i].tope_obstaculos = tope_obstaculos;
    }

    /*
    //catapulta(fuera del camino, cuando h utilice una runa o echizo)
    for (int i = 0; i < 3; i++){
        for (int j; j < 10; j++){
            (*juego).niveles[i].obstaculos[2].tipo = 'K';
            (*juego).niveles[i].obstaculos[2].posicion.fil = 0;
            (*juego).niveles[i].obstaculos[2].posicion.col = 5;
        }
    }
    */
}


/////////////////////////////////////////////////////////
void inicializar_juego(juego_t* juego) {

    /*
    Paredes
    Caminos
    Homero
    Runa
    Altar
    Pergamino
    Herramientas
    Obstáculos
    */
    int tope_niveles = 3;
    int nivel_actual = 0;

    (*juego).nivel_actual = nivel_actual;
    (*juego).tope_niveles = tope_niveles;
    (*juego).camino_visible = true;

   //crea paredes y camino para cada nivel
    inicializar_estructura( (*juego).niveles);

    //crea Homero posicion inicial
    inicializar_homero( &(*juego).homero, (*juego).niveles[0].camino[0]);

    //crear los items (runa, altar, pergamino)
    inicializar_items( (*juego).niveles );

    // crea echizoz, antorchas y totems
    inicializar_herramientas( (*juego).niveles);

    // crea piedra castigo y catapulta
    inicializar_obtaculos((*juego).niveles, (*juego).tope_niveles);

    
}
//////////////////////////////////////////////////////////



///////////////////////////////////////////////
void cambiar_nivel(juego_t* juego) {    
    // primer nivel 1 despues 2 como cambio al tres
    (*juego).nivel_actual += 1;
}
//////////////////////////////////////////////////

/*
* Pre condiciones: -
* Post condicones: devuelve true si el movimiento es S,D,A o W, false en caso contrario
*/
bool es_movimiento(char movimiento){
    return (movimiento == 'A' || movimiento == 'S' || movimiento == 'D' || movimiento == 'W' );
}

bool es_herramienta(char movimiento){
    return (movimiento == 'H' || movimiento == 'L');
}
/*
*
*
*/
void dirigir_movimiento(char movimiento, int* fil_movimiento, int* col_movimiento){
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
*
*
*/
bool es_camino(juego_t* juego, int fil_movimiento, int col_movimiento){

    bool pisa_camino = false;
    int fil_personaje = (*juego).homero.posicion.fil;
    int col_personaje = (*juego).homero.posicion.col;
    int nivel_actual = (*juego).nivel_actual;
    int tope_camino = (*juego).niveles[nivel_actual].tope_camino;

    for(int i = 0; i < tope_camino; i++){
        if( (fil_personaje + fil_movimiento == (*juego).niveles[nivel_actual].camino[i].fil ) &&  (col_personaje + col_movimiento == (*juego).niveles[nivel_actual].camino[i].col )){
            pisa_camino = true;
        }
    }

    return pisa_camino;
}



/*
* Pre condiciones: juego debe ser inicializado, movimiento tiene que ser A,S,D o W
* Post condciciones: guarda la nueva posicion del jugador si el movimiento es la posicion de un camino
*/
void analizar_movimiento(juego_t *juego, int fil_movimiento, int col_movimiento){
    int nivel = (*juego).nivel_actual;

    int fil_personaje = (*juego).homero.posicion.fil;
    int col_personaje = (*juego).homero.posicion.col;

    int fil_pergamino = (*juego).niveles[nivel].pergamino.fil;
    int col_pergamino = (*juego).niveles[nivel].pergamino.col;
    
    // verifica que la posicion + movimiento sea un camino
    if (es_camino(&(*juego), fil_movimiento, col_movimiento)){
        (*juego).homero.posicion.fil += fil_movimiento;
        (*juego).homero.posicion.col += col_movimiento;

        // si el movimiento es el pergamino lo lleva con el
        if(fil_personaje == fil_pergamino && col_personaje == col_pergamino){
            (*juego).niveles[nivel].pergamino.fil += fil_movimiento;
            (*juego).niveles[nivel].pergamino.col += col_movimiento;
        }
    /////hacer que si es pared no se elimine ...
    } else {
        (*juego).homero.vidas_restantes -=1;
    }

    if ((*juego).homero.posicion.fil == (*juego).niveles[nivel].camino[0].fil && (*juego).homero.posicion.col == (*juego).niveles[nivel].camino[0].col ){
        (*juego).camino_visible = true;
    } else {
        (*juego).camino_visible = false;
    }
    
    

    // no pertenece al camino ni a las paredes
    // el movimiento no pertenece al camino resta una vida
}
//////////////////////////////////////////////////

void realizar_jugada(juego_t* juego, char movimiento){
    int fil_movimiento = 0;
    int col_movimiento = 0;
    //realiza el pre-movimiento
    if (es_movimiento(movimiento)){
        dirigir_movimiento(movimiento, &fil_movimiento, &col_movimiento);
        analizar_movimiento(juego, fil_movimiento, col_movimiento);  
    } else if(es_herramienta(movimiento)){
        /// H para hechizos

        if ((movimiento == 'H') && ((*juego).homero.hechizos_reveladores > 0)){
            (*juego).camino_visible = true;
            (*juego).homero.hechizos_reveladores -= 1;
        } 

        if ((movimiento == 'L') && ((*juego).homero.antorchas > 0)){
            (*juego).homero.antorcha_encendida = true;
            (*juego).homero.antorchas -= 1;
        }
        // L para antorcha

    }
}
////////////////////////////////////////////////////

void mostrar_bienvenida(){
    system("clear");
    printf("%s\n", MSJ_BIENVENIDA);
}
/*
* Pre condiciones: mapa debe estar inicializado
* Post condiciones: carga paredes al mapa.
*/
void agregar_paredes(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t paredes[MAX_PAREDES], int tope_paredes){
    //agregar paredes
    for (int i = 0; i < tope_paredes; i++){
        int fil_pared = paredes[i].fil;
        int col_pared = paredes[i].col;
        mapa[fil_pared][col_pared] = PARED;
    }
}

/*
* Pre condiciones: mapa debe ser inicializado
* Post condiciones: agrega camino al mapa.
*/
void agregar_camino(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino){
    //agregar camino
    for (int i = 0; i < tope_camino; i++){
        int fil_camino = camino[i].fil;
        int col_camino = camino[i].col;

        mapa[fil_camino][col_camino] = CAMINO;
    }
}


void agregar_camino_manhattan(char mapa[MAX_FILAS][MAX_COLUMNAS], coordenada_t camino[MAX_CAMINO], int tope_camino, coordenada_t posicion){
    int fil_homero = posicion.fil;
    int col_homero = posicion.col;
    // (0,0) --> (1,2)
    for (int i = 0; i < tope_camino; i++){
        if(((fil_homero - camino[i].fil) <= 3) && ((col_homero - camino[i].col <= 3)) ){

            if(((fil_homero - camino[i].fil) >= -3) && ((col_homero - camino[i].col >= -3))){

                mapa[camino[i].fil][camino[i].col] = CAMINO;
            } 
        } 

        /*
        if((fil_homero +1 == camino[i].fil) && (col_homero == camino[i].col)){
            mapa[camino[i].fil][camino[i].col] = CAMINO;
        }
        
        if((fil_homero == camino[i].fil) && (col_homero +1 == camino[i].col)){
            mapa[camino[i].fil][camino[i].col] = CAMINO;
        }
        
        if((fil_homero -1 == camino[i].fil) && (col_homero == camino[i].col)){
            mapa[camino[i].fil][camino[i].col] = CAMINO;
        }
        
        if( (fil_homero == camino[i].fil) && (col_homero -1 == camino[i].col)){
            mapa[camino[i].fil][camino[i].col] = CAMINO;
        }
        */
    }
}
/*
* Pre condiciones: mapa debe ser inicializado
* Post condiciones: agrega el peronaje al mapa.
*/
void agregar_personaje(char mapa[MAX_FILAS][MAX_COLUMNAS], personaje_t *homero){
    int fil_homero = (*homero).posicion.fil;
    int col_homero = (*homero).posicion.col;

    mapa[fil_homero][col_homero] = HOMERO;
}



/*
* Pre condiciones: mapa, juego debe estar inicializado
* Post condicones: carga la runa, altar, pergamino en el mapa 
*/
void agregar_items(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego){
    // Runa (donde se encuntra homero)
    int nivel = juego.nivel_actual;
    int tope_camino = juego.niveles[nivel].tope_camino;

    int camino_inicial_fil = juego.niveles[nivel].camino[0].fil;
    int camino_inicial_col = juego.niveles[nivel].camino[0].col;

    mapa [camino_inicial_fil][camino_inicial_col] = RUNA;

    // altar (en la ultima posicion del camino)
    int camino_final_fil = juego.niveles[nivel].camino[tope_camino-1].fil;
    int camino_final_col = juego.niveles[nivel].camino[tope_camino-1].col;

    mapa [camino_final_fil][camino_final_col] = ALTAR;

    // pergamino esta en posicion ramdom
    int fil_pergamino = (juego).niveles[nivel].pergamino.fil;
    int col_pergamino = (juego).niveles[nivel].pergamino.col;

    mapa [fil_pergamino][col_pergamino] = PERGAMINO;
}

/*
* Pre condiciones: mapa, juego deben estar inicializados
* Post condiciones: FALTA HACERRRRRR!!!!!!!!!!!!!!!!!
*/
void agregar_obtaculos(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego){
    // piedras
    int nivel = juego.nivel_actual;
    for (int i = 0; i < 10; i++){
        int piedra_fil = (juego).niveles[nivel].obstaculos[i].posicion.fil;
        int piedra_col = (juego).niveles[nivel].obstaculos[i].posicion.col; 

        mapa[piedra_fil][piedra_col] = PIEDRA_CASTIGO; /////AREGLAR
    }
}

/*
* Pre condiciones: mapa, juego deben estar inicializados
* Post condiciones: FALTA HACERRRRRR!!!!!!!!!!!!!!!!!
*/
void agregar_herramientas(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego){
    
    // Totems
    for (int i = 0; i < 5; i++){
        int totem_fil = (juego).niveles[juego.nivel_actual].herramientas[i].posicion.fil;
        int totem_col = (juego).niveles[juego.nivel_actual].herramientas[i].posicion.col;
        mapa[totem_fil][totem_col] = TOTEM; /////AREGLAR
    }
}



/*
* Pre condiciones: mapa, juego deben estar inicializados
* Post condiciones: crea un mapa del juego
*/
void crear_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS], juego_t juego){
    
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            mapa[i][j] = VACIO;
        }
    }
  
    agregar_paredes(mapa, juego.niveles[juego.nivel_actual].paredes, juego.niveles[juego.nivel_actual].tope_paredes);

    if (juego.camino_visible){
        agregar_camino(mapa, juego.niveles[juego.nivel_actual].camino, juego.niveles[juego.nivel_actual].tope_camino);
    } else if (juego.homero.antorcha_encendida){
        agregar_camino_manhattan(mapa, juego.niveles[juego.nivel_actual].camino, juego.niveles[juego.nivel_actual].tope_camino, juego.homero.posicion);
   
    }

    agregar_personaje(mapa, &juego.homero);
    
    /// agregar items (Runa, altar, pergamino)
    agregar_items(mapa, juego);

    /// agregar obtaculos
    agregar_obtaculos(mapa, juego);

    agregar_herramientas (mapa,juego);
}

///////////////////////////////////////////////
int estado_nivel(nivel_t nivel){
    // nivel  ganado si el pergamino+homero esta en el altar 0= jugando, 1 ganado
    int tope = nivel.tope_camino;

    int altar_fil = nivel.camino[tope-1].fil;
    int altar_col = nivel.camino[tope-1].col;

    int pergamino_fil = nivel.pergamino.fil;
    int pergamino_col = nivel.pergamino.col;

    if (pergamino_fil == altar_fil && pergamino_col == altar_col){
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

void pedir_movimiento(char* movimiento, int numero_vidas, int numero_hechizos, int numero_antorchas){
    printf("-----------------------------------------------------------\n\n");
    printf("Vidas: %i             \nHechizo revelador: %i              presionar (H) para utilizar\nAntorchas: %i                     presionar (L) para utilizar\n\n", numero_vidas, numero_hechizos, numero_antorchas);
    printf("%s", MSJ_MOVIMIENTOS);
    
    scanf(" %c", movimiento);
}
/////////////////////////////////////////////////////////////////////
/*
void mostrar_juego(juego_t juego){

    char mapa[MAX_FILAS][MAX_COLUMNAS];
    int nivel = juego.nivel_actual;
    
    // el juego se mantiene mientras el estado del juego sea jugando y el nivel sea menor a 3
    while ((estado_juego(juego) == 0) && ( nivel < 3)){
        int numero_vidas = juego.homero.vidas_restantes;
        int numero_hechizos = juego.homero.hechizos_reveladores;
        int numero_antorchas = juego.homero.antorchas;
        
        char movimiento = ' ';
        
        crear_mapa(mapa, juego);
        
        mostrar_bienvenida();
        mostrar_mapa(mapa);

        pedir_movimiento(&movimiento, numero_vidas, numero_hechizos, numero_antorchas);
        realizar_jugada(&juego, movimiento);

        if (estado_nivel((juego).niveles[nivel]) == 1){
            cambiar_nivel(&(juego));
            nivel = juego.nivel_actual;
            juego.camino_visible = true;
        }
    }  
    if (estado_juego(juego) == -1){
        system("clear");
        printf("%s", MSJ_ELIMINACION);
    } else {
        system("clear");
        printf("%s", MSJ_JUEGO_GANADO);
    }
}
*/
//////////////////////////////////////////////////////////////////////////////
void mostrar_juego(juego_t juego){
    char mapa[MAX_FILAS][MAX_COLUMNAS];
    int numero_vidas = juego.homero.vidas_restantes;
    int numero_hechizos = juego.homero.hechizos_reveladores;
    int numero_antorchas = juego.homero.antorchas;

    crear_mapa (mapa, juego);
    mostrar_bienvenida();
    mostrar_mapa(mapa);
}
