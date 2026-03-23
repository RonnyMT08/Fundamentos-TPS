#include <stdio.h>
#include <stdbool.h>

const char RECHAZADO = 'R';

const char JEBEDIAH_SPRINGFIELD = 'J';
const char LOS_ALIENS = 'A';
const char LOS_MAGIOS = 'S';
const char SR_BURNS = 'B';

const char SI = 'S';
const char NO = 'N';

const int ANIO_ACTUAL = 2026;
const int MES_ACTUAL = 03;
const int ANIO_LIMITE = 1926;
const int MES_LIMITE = 03; 

const int DONAS_MAX = 12;
const int DONAS_MIN = 0;

/*
PRE:
POST:
*/
int puntuar_fundador(char *fundador){
  if ( *fundador == JEBEDIAH_SPRINGFIELD ){
    return +100;
  } else {
    return -20;
  }
}


/*
PRE:-
POST: devuelve true si la respuesta dada es una opcion permitida para contar los como intento en la primera pregunta(fundador de springfiel). False en caso contrario.
*/
bool es_fundador_valido(char *respuesta){
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
void pedir_fundador(char *fundador, int *puntos, char *estado_del_aspirante){
  char numero_intentos = 3;
  
  for (int i = 0; i < numero_intentos; i++){

    printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns \n---cuentas con %i intento(s) más---\n", numero_intentos - i);
    scanf(" %c", fundador);

    if ( (es_fundador_valido(fundador) && (*fundador == JEBEDIAH_SPRINGFIELD)) ){
      puntos += puntuar_fundador(fundador);
      return;
    } else if ((es_fundador_valido(fundador))){
      puntos += puntuar_fundador(fundador);
    }
    
    while ( !es_fundador_valido(fundador)){    
      printf("---'%c' No es una opción valida---\n¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns\n", *fundador);
      scanf(" %c", fundador);
    }

    if ( (es_fundador_valido(fundador) && (*fundador == JEBEDIAH_SPRINGFIELD)) ){
      puntos += puntuar_fundador(fundador);
      return;
    } else {
      puntos += puntuar_fundador(fundador);
    }
  }
  *estado_del_aspirante = RECHAZADO;
}

/*
PRE:-
POST: imprime por pantalla el mensaje de rechazado, y temrmina el programa
*/
void rechazar_aspirante(char *estado_del_aspirante){
  *estado_del_aspirante = RECHAZADO;
  printf("-RECHAZADO- \n");
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
    printf("---'%c' No es una opción valida---\n¿Promete matener en secreto la existencia de los Masgios? \n[S] Si \n[N] No\n", respuesta_dada);
    scanf(" %c", &respuesta_dada);
  }
  *secreto = (respuesta_dada == SI);
}


/*
PRE: se debe ingresar solo numero enteros
POST: guarda en edad_del_aspirante
*/
void pedir_fecha_nacimiento(int* edad){
  int anio = 0;
  int mes = 0;
  printf("¿Cuál es su fecha de nacimiento?(formato:yyyy/mm)\n Fecha:");
  scanf(" %i/%i", &anio, &mes);
  
  if ( es_mes_valido(&mes) && es_anio_valido(&anio) ){
    if ((anio == ANIO_LIMITE) && (anio > MES_LIMITE) ){
      *edad = 0;
    }
    else if ((anio == ANIO_ACTUAL) && (mes < 03)){
      anio += 1;
    }
    *edad = 2026-anio;
  } else {
    *edad = 0;
  }
}

/*
PRE: pide al aspirante un numero entero entre 0(DONAS_MIN), 12(DONAS_MAX) de donas que sacrificará.
POST: valida y actualiza el numero de donas sacrificadas.
*/
void pedir_donas(int *numero_donas){
	int donas_dadas;
	printf("¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	scanf(" %i", &donas_dadas);
	
	while (!((donas_dadas >= DONAS_MIN) && (donas_dadas <= DONAS_MAX))){
	  printf("---el número no es valido---\n¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	  scanf(" %i", &donas_dadas);
	}
	*numero_donas = donas_dadas;
}

/*
PRE: -
POST: calcula los puntos obtenidos por el número de donas sacrificadas.
*/
void calcular_puntos_donas(int numero_donas, int *puntos){
  if (numero_donas == 0){
    *puntos -= 100;
  } else if (numero_donas > 0 && numero_donas <= 3){
    *puntos += 10;
  } else if (numero_donas > 3 && numero_donas <= 6){
    *puntos += 40;
  } else if (numero_donas > 6 && numero_donas <= 9){
    *puntos += 70;
  } else if (numero_donas > 9 && numero_donas <= 12){
    *puntos += 120;
  }
} 

/*
PRE: se debe ingresar solo numero enteros
POST: imprime por pantalla el grado de magio obtenido por el aspirante segun los puntos
*/
void grado_de_magio(int puntos){
  if (puntos < 0){
    printf("-RECHAZADO- \n");
  } else if (puntos >= 0 && puntos <= 150){
    printf("-ASPIRANTE- \n");
  } else if (puntos >= 151 && puntos <= 250){
    printf("-MAGIO NOVATO- \n");
  } else if (puntos >= 251 && puntos <= 349){
    printf("-MAGIO-\n");
  } else if (puntos >= 350){
    printf("-LIDER SUPREMO- \n");
  }
}


int main() {
  char estado_del_aspirante = '-';
  
  char fundador = '-';
  bool secreto = false; 
  int edad_aspirante = 0;
  int numero_donas = 0;
  
  int puntos = 0;
  
  printf("---Prueba de iniciación de Magia---\nPara determinar si una persona es digna de ingresar a la sociedad secreta.\n\n");
  
  pedir_fundador(&fundador, &puntos , &estado_del_aspirante);
  if (estado_del_aspirante == RECHAZADO){
    rechazar_aspirante(&estado_del_aspirante);
    return 0;
  }
  
  mantener_secreto(&secreto);
  if (secreto == true){
    puntos += 50;
  } else {
    puntos -= 300;
  }
  
  pedir_fecha_nacimiento(&edad_aspirante);
  printf("tienes %i años\n", edad_aspirante);
  if (edad_aspirante >= 18){
    puntos += ( 2 * edad_aspirante );
  }else if (edad_aspirante < 18){
    rechazar_aspirante(&estado_del_aspirante);
    return 0;
  }
  
  pedir_donas(&numero_donas);
  calcular_puntos_donas(numero_donas, &puntos);

  printf("puntos obtenidos: %i\n", puntos);
  grado_de_magio(puntos);

  
  return 0;
}
