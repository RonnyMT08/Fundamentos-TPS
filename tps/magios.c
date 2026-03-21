#include <stdio.h>
#include <stdbool.h>

const char RECHAZADO = 'R';
const char ACEPTADO = 'P';

const char JEBEDIAH_SPRINGFIELD = 'J';
const char LOS_ALIENS = 'A';
const char LOS_MAGIOS = 'S';
const char SR_BURNS = 'B';

const char SI = 'S';
const char NO = 'N';

const int ANIO_ACTUAL = 2026;
const int ANIO_LIMITE = 1926;
const int MES_ACTUAL = 03;
const int MES_LIMITE = 03; 

const int DONAS_MAX = 12;
const int DONAS_MIN = 0;

/*
PRE:-
POST: devuelve true si la respuesta dada es una opcion permitida para contar los como intento en la primera pregunta(fundador de springfiel). False en caso contrario.
*/
bool es_opcion_fundador(char *respuesta){
  return ((*respuesta == JEBEDIAH_SPRINGFIELD) || (*respuesta == LOS_ALIENS) || (*respuesta == LOS_MAGIOS) || (*respuesta == SR_BURNS));
}

/*
PRE:-
POST: devuelve true si el mes ingresado por el usuario es representativo de un mes real. False en caso contrario. 
*/
bool es_mes_valido(int *mes){
  return ((*mes <= 12) && (*mes > 0));
}

/*
PRE:-
POST: devuelve true si el año ingresado por el usuario esta etre los limites permitidos para los aspirantes, False en caso contrario. 
*/
bool es_anio_valido(int *anio){
  return ((*anio > ANIO_LIMITE) && (*anio < ANIO_ACTUAL));
}

/*
PRE: -
POST: guarda el estado_del_aspirante a 'ARECHAZADO' si la respuesta dada es inccorrecta.  
*/
void preguntar_fundador(char *fundador){
  char numero_intentos = 3;
  
  for (int i = 0; i < numero_intentos; i++){
    printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns \n---(cuentas con %i intento(s))---\n", numero_intentos - i);
    scanf(" %c", fundador);
    if (*fundador == JEBEDIAH_SPRINGFIELD){
      return;
    }
    
    while ( !es_opcion_fundador(fundador)){    
      printf("---Esa opción no es valida---\n¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns\n");
      scanf(" %c", fundador);
      if (*fundador == JEBEDIAH_SPRINGFIELD){
        return;
      }
    }
  }
}

/*
PRE:-
POST: guarda el estado_del_aspirante a RECHAZADO si revela la existencia de los magios.
*/
void mantener_secreto(bool *secreto){
  char respuesta_dada = '-';
  
  printf("¿Promete mantener en secreto la existencia de los Magios? \n[S] Si \n[N] No\n");
  scanf(" %c", &respuesta_dada);
  
  while (!((respuesta_dada == NO) || (respuesta_dada == SI))){
    printf("---Esa opción no es válida---\n¿Promete matener en secreto la existencia de los Masgios? \n[S] Si \n[N] No\n");
    scanf(" %c", &respuesta_dada);
  }
  *secreto = (respuesta_dada == SI);
}

/*
PRE:-
POST: imprime por pantalla el mensaje de rechazado, y temrmina el programa
*/
void rechazar_aspirante(char *estado_del_aspirante){
  *estado_del_aspirante = RECHAZADO;
  printf("--RECHAZADO-- \n");
}

/*
PRE: se debe ingresar solo numero enteros
POST: guarda en edad_del_aspirante
*/
void pedir_fecha_nacimiento(int* edad){
  int anio = 0;
  int mes = 0;
  printf("¿Cuál es su fecha de nacimiento?(formato:yyyy/mm)\n");
  scanf(" %i/%i", &anio, &mes);
  
  if ( es_mes_valido(&mes) && es_anio_valido(&anio) ){
    if ((anio == ANIO_LIMITE) && (anio > MES_LIMITE) ){
      *edad = 0;
    }
    if (mes < 03){
      anio += 1;
    }
    *edad = 2026-anio; /*faltacalculos del mes*/
  } else {
    *edad = 0;
  }
}

/*
PRE: pide al aspirante un numero entero entre 0(DONAS_MIN), 12(DONAS_MAX) de donas que sacrificará.
POST: valida y actualiza el numero de donas sacrificadas.
*/
void sacrificar_donas(int *numero_donas){
	int donas_dadas;
	printf("¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	scanf(" %i", &donas_dadas);
	
	while (!((donas_dadas >= DONAS_MIN) && (donas_dadas <= DONAS_MAX))){
	  printf("---el número no es valido---\n¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	  scanf(" %i", &donas_dadas);
	}
	*numero_donas = donas_dadas;
}

int main() {
  char estado_del_aspirante = '-';
  
  char fundador = '-';
  bool secreto = false; 
  int edad_aspirante = 0;
  int numero_donas = 0;
  
  printf("---Prueba de iniciación (Los Magios)---\n\n");
  preguntar_fundador(&fundador);
  if (!(fundador == JEBEDIAH_SPRINGFIELD)){
    rechazar_aspirante(&estado_del_aspirante);
    return 0; 
  }
  
  mantener_secreto(&secreto);
  if (!(secreto == true)){
    rechazar_aspirante(&estado_del_aspirante);
    return 0;
  }
  
  pedir_fecha_nacimiento(&edad_aspirante);
  printf("tienes %i años\n", edad_aspirante);
  if (edad_aspirante < 18){
    rechazar_aspirante(&estado_del_aspirante);
    return 0;
  }
  
  sacrificar_donas(&numero_donas);
  
  printf("magio novato\n");
  
  return 0;
}
