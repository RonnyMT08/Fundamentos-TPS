//gcc juego.c ascension_magios.c utiles.o -o juego -std=c99 -Wall -Wconversion -Werror -lm

#include "ascension_magios.h"
#include "utiles.h"
#include <stdio.h>
#include <stdlib.h> // Para usar rand, limpiar terminal, etc.
#include <time.h>   // Para obtener una semilla desde el reloj

const char HOMERO = 'H';
const char PARED = 'X';
const char RUNA = 'U';
const char ALTAR = 'A';
const char PERGAMINO = 'P';
const char TOTEM = 'T';
const char CAMINO = '-';
const char VACIO = ' ';
const char PIEDRA_CASTIGO = 'R';


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
void inicializar_estructura(juego_t *juego){
    int tope_paredes = 0;
    int tope_caminos = 0; 
    int tope_niveles = 3;

    for (int i = 0; i < MAX_NIVELES; i++){
        int nivel_actual = i+1;

        (*juego).niveles[i].tope_paredes = tope_paredes;
        (*juego).niveles[i].tope_camino = tope_caminos;
        
        obtener_mapa((*juego).niveles[i].paredes, &(*juego).niveles[i].tope_paredes, (*juego).niveles[i].camino, &(*juego).niveles[i].tope_camino, nivel_actual);
    }
    (*juego).tope_niveles = tope_niveles;
}

/*
* Pre condiciones: -
* Post condiciones: carga un personaje con las su pocision, vidas, hechizos, antorchas
*/
void inicializar_homero (juego_t *juego){
    int vidas = 5;
    int hechizos = 5;
    int antorchas = 5;
    
    int camino_fil = (*juego).niveles[0].camino[0].fil;
    int camino_col = (*juego).niveles[0].camino[0].col;
    
    (*juego).homero.posicion.fil = camino_fil;
    (*juego).homero.posicion.col = camino_col;

    (*juego).homero.vidas_restantes = vidas;
    (*juego).homero.hechizos_reveladores = hechizos;
    (*juego).homero.antorchas = antorchas;
}

/*
* Pre condiciones: -
* Post condiciones: carga la posicion del pergamino para los tres niveles.
*/
void inicializar_items(juego_t *juego){

   // PERGAMINO
    for (int i = 0; i < 3; i++){
        int tope_camino = (*juego).niveles[i].tope_camino;
        //la posicion esta entre 1 y antes de la posicion final
        int posicion_pergamino = numero_aleatorio(1, tope_camino-2);

        int pergamino_fil = (*juego).niveles[i].camino[posicion_pergamino].fil;
        int pergamino_col = (*juego).niveles[i].camino[posicion_pergamino].col;
        
        (*juego).niveles[i].pergamino.fil = pergamino_fil;
        (*juego).niveles[i].pergamino.col= pergamino_col;        
    }
}

/*
Pre condicones: -
Post condicioes:  Falta hacerrEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRRRR
*/
void inicializar_herramientas(juego_t *juego){
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
    
    
    for (int i = 0; i < 3; i++){
    int tope_camino = (*juego).niveles[i].tope_camino;
    
        
        for (int j = 0; j < 5; j++){
            int posicion_camino = numero_aleatorio(1, tope_camino-2); // -2 por que en el tope no hay nada, y ultima es el altar
            int fil_totem = (*juego).niveles[i].camino[posicion_camino].fil;
            int col_totem = (*juego).niveles[i].camino[posicion_camino].col;
            
            (*juego).niveles[i].herramientas[j].tipo = TOTEM; //existen 3 herramientas echizo, totems, antorchas
            (*juego).niveles[i].herramientas[j].posicion.fil = fil_totem;
            (*juego).niveles[i].herramientas[j].posicion.col = col_totem;
            }
    }
}

/*
* Pre condiciones: -
* Post condciones: carga PIEDRA DEL CARTIGO, CATAPULCA FALTA HACERRRRR!!!!!!!!!!!!!!!!!!
*/
void inicializar_obtaculos(juego_t *juego){

    int posicion_camino = numero_aleatorio(2,5);
    // piedra del catigo (10 en cada nivel de manera aleatoria)
    for (int i = 0; i < 3; i++){
        int tope_camino = (*juego).niveles[i].tope_camino;
        (*juego).niveles[i].tope_obstaculos = 10;

        for (int j = 0; j < 10; j++){
            (*juego).niveles[i].obstaculos[j].tipo = PIEDRA_CASTIGO;
            
            /// la piedra en el camino
            posicion_camino =  numero_aleatorio(1, tope_camino-2);


            // asigno la fila, columna del camino a la piedra
            int fil_piedra = (*juego).niveles[i].camino[posicion_camino].fil;
            int col_piedra = (*juego).niveles[i].camino[posicion_camino].col;
            
            // asigno a la piedra la fila y columna 
            (*juego).niveles[i].obstaculos[j].posicion.fil = fil_piedra;
            (*juego).niveles[i].obstaculos[j].posicion.col = col_piedra;
        }
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
void inicializar_juego(juego_t *juego) {

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
    int nivel_inicial = 0;
    //nivel inicial 1 o 2
    (*juego).nivel_actual = nivel_inicial;

   //crea paredes y camino
    inicializar_estructura( &(*juego) );

    //crea Homero posicion inicial
    inicializar_homero( &(*juego) );

    //crear los items (runa, altar, pergamino)
    inicializar_items(&(*juego));

    // crea echizoz, antorchas y totems
    inicializar_herramientas(&(*juego));

    // crea piedra castigo y catapulta
    inicializar_obtaculos(&(*juego));

    
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

/*
* Pre condiciones: juego debe ser inicializado, movimiento tiene que ser A,S,D o W
* Post condciciones: guarda la nueva posicion del jugador si el movimiento es la posicion de un camino
*/
void analizar_movimiento(juego_t *juego, char movimiento){
    bool se_mueve = false;
    int fil_movimiento = 0;
    int col_movimiento = 0;

    int fil_personaje = (*juego).homero.posicion.fil;
    int col_personaje = (*juego).homero.posicion.col;
    
    int nivel = (*juego).nivel_actual;
    int tope_camino = (*juego).niveles[nivel].tope_camino;

    switch (movimiento){
        case 'W':
            fil_movimiento = -1;
            break;
        case 'S':
            fil_movimiento = +1;
            break;
        case 'A':
            col_movimiento = -1;
            break;
        case 'D':
            col_movimiento = +1;
            break;   
        default:
            break;
    }
    
    // verifica si el movimiento sea un comino
    for(int i = 0; i < tope_camino; i++){
        int fil_camino = (*juego).niveles[nivel].camino[i].fil; 
        int col_camino = (*juego).niveles[nivel].camino[i].col;
        
        // el movimiento pertenece al camino LO GUARDA
        if ((fil_personaje + fil_movimiento == fil_camino) &&(col_personaje+col_movimiento == col_camino)){
            (*juego).homero.posicion.fil += fil_movimiento;
            (*juego).homero.posicion.col += col_movimiento;
            se_mueve = true;
        }
    }
    
    // si el movimiento coincide con el pergamino le damos a homero
    //int nfil_personaje = (*juego).homero.posicion.fil;
    //int ncol_personaje = (*juego).homero.posicion.col;
    
    int fil_pergamino = (*juego).niveles[nivel].pergamino.fil;
    int col_pergamino = (*juego).niveles[nivel].pergamino.col;

    if (fil_personaje == fil_pergamino && col_personaje == col_pergamino){
        (*juego).niveles[nivel].pergamino.fil += fil_movimiento;
        (*juego).niveles[nivel].pergamino.col += col_movimiento;
    }


    // si no se mueve es por que no es camino -> es pared? pierde vida?
    if (!se_mueve){
        (*juego).homero.vidas_restantes -=1;
    }

    // no pertenece al camino ni a las paredes
    // el movimiento no pertenece al camino resta una vida
}
//////////////////////////////////////////////////

void realizar_jugada(juego_t *juego, char movimiento){

    //realiza el pre-movimiento
    if (es_movimiento(movimiento)){
        analizar_movimiento(juego, movimiento);   
    }
    //
        
}
////////////////////////////////////////////////////


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

    agregar_camino(mapa, juego.niveles[juego.nivel_actual].camino, juego.niveles[juego.nivel_actual].tope_camino);

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
void ver_mapa(char mapa[MAX_FILAS][MAX_COLUMNAS]){
    //imprimir matriz
    for (int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}
/////////////////////////////////////////////////////////////////////
void mostrar_juego(juego_t juego){

    char mapa[MAX_FILAS][MAX_COLUMNAS];
    int nivel = juego.nivel_actual;
    
    // el juego se mantiene mientras el estado del juego sea jugando y el nivel sea menor a 3
    while ((estado_juego(juego) == 0) && ( nivel < 3)){
        int numero_vidas = juego.homero.vidas_restantes;
        int numero_echizos = juego.homero.hechizos_reveladores;
        int numero_antorchas = juego.homero.antorchas; 
        
        char movimiento = ' ';

        system("clear");
        crear_mapa(mapa, juego);
    
        printf("--------------------Welcome to The Game %d-------------------\n\n", nivel+1);
        ver_mapa(mapa);
        printf("-----------------------------------------------------------\n\n");
        printf("Vidas: %i             \nEchizo revelador: %i              presionar (H) para utilizar\nAntorchas: %i                     presionar (L) para utilizar\n\n", numero_vidas, numero_echizos, numero_antorchas);
        printf("      -W-          -S-            -A-           -D- \n");
        printf("     Arriba       Abajo        Izquierda      Derecha \n");

        
        scanf(" %c", &movimiento);
        realizar_jugada(&juego, movimiento);

        if (estado_nivel((juego).niveles[nivel]) == 1){
            cambiar_nivel(&(juego));
            nivel = juego.nivel_actual;
        }
    }  
    if (estado_juego(juego) == -1){
        system("clear");
        printf("ELIMINADO...  TE QUEDASTE SIN VIDAS!! \n");
    } else {
        system("clear");

        printf("ELIMINADO...  TE QUEDASTE SIN VIDAS!! \n\n\n\n\n\n\n\n\n\n");
        printf("NO MEN TIRA GANASTE!! \n");
    }
}
//////////////////////////////////////////////////////////////////////////////

