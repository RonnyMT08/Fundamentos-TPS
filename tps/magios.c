#include <stdio.h>
#include <stdbool.h>

const int INTENTOS_MAXIMO =  3;

const char JEBEDIAH_SPRINGFIELD = 'J';
const char LOS_ALIENS = 'A';
const char LOS_MAGIOS = 'S';
const char SR_BURNS = 'B';

const int PTOS_FUNDADOR_CORRECTO = 100;
const int PTOS_FUNDADOR_INCORRECTO = 20;

const char SI = 'S';
const char NO = 'N';

const int PTOS_GUARDAR_SECRETO = 50;
const int PTOS_DECIR_SECRETO = 300;

const int ANIO_ACTUAL = 2026;
const int MES_ACTUAL = 03;
const int ANIO_LIMITE = 1926;
const int MES_LIMITE = 03; 

const int EDAD_LIMITE = 18;
const int INDICE_PTOS_POR_EDAD = 2;

const int DONAS_MAX = 12;
const int DONAS_MIN = 0;

const int RANG_DONAS_NULO = 0;
const int RANG_DONAS_BAJO = 3;
const int RANG_DONAS_MEDIO = 6;
const int RANG_DONAS_ALTO = 9;
const int RANG_DONAS_MAXIMO = 12;

const int PTOS_DONAS_RANG_NULO = 100;
const int PTOS_DONAS_RANG_BAJO = 10;
const int PTOS_DONAS_RANG_MEDIO = 40;
const int PTOS_DONAS_RANG_ALTO = 70;
const int PTOS_DONAS_RANG_MAXIMO = 120;

const int PTOS_GRADO_RECHAZADO = 0;
const int PTOS_GRADO_ASPIRANTE = 150;
const int PTOS_GRADO_MAGIO_NOVATO = 250;
const int PTOS_GRADO_MAGIO = 350;
const int PTOS_GRADO_LIDER_SUPREMO = 350;

/*
PRE: el fundador debe ser JEBEDIAH SPRINGFIEL, LOS ALIENS, SR. BRUNS O LOS MAGIOS
POST: devuelve los puntos acreditados por seleccionar un de fundador seleccionado
*/
int puntos_por_fundador(char *fundador_dado){
  if ( *fundador_dado == JEBEDIAH_SPRINGFIELD ){
    return +PTOS_FUNDADOR_CORRECTO;
  } else if (*fundador_dado == LOS_ALIENS || *fundador_dado == LOS_MAGIOS ||*fundador_dado == SR_BURNS ){
    return -PTOS_FUNDADOR_INCORRECTO;
  } else {
    return 0;
  }
}

/*
PRE: el secreto debe ser true(guardado) o false(no guardado)
POST: devuelve los puntos acreaditados por guardar silencio (no decir el secreto)
*/
int puntos_por_silencio(bool *guardar_secreto){
  if (*guardar_secreto == true){
    return +PTOS_GUARDAR_SECRETO;
  } else if (*guardar_secreto == false){
    return -PTOS_DECIR_SECRETO;
  } else {
    return 0;
  }
}

/*
  PRE:-
  POST: devuelve la edad del aspirante
*/
int edad_del_aspirante(int *anio_dado, int *mes_dado){
  if ((MES_ACTUAL - (*mes_dado)) < 0){
    *anio_dado += 1;
  }
  return (ANIO_ACTUAL-(*anio_dado));
}

/*
PRE: la edad debe ser mayor o igual a 18 años
POST: devuelve los puntos acreditados por la (edad*2) del aspirante
*/
int puntos_por_edad(int *edad_dado){
  if ( *edad_dado >= EDAD_LIMITE ){
    return INDICE_PTOS_POR_EDAD*(*edad_dado) ;
  } else {
    return 0;
  }
}

/*
PRE: las de donas debe ser entre 0 donas y 12 donas
POST: devuelve los puntos obtenidos por el número de donas sacrificadas.
*/
int puntos_por_donas(int *numero_donas){
  if ( *numero_donas == RANG_DONAS_NULO ){
    return PTOS_DONAS_RANG_NULO;
  } else if (*numero_donas > RANG_DONAS_NULO && *numero_donas <= RANG_DONAS_BAJO){
    return PTOS_DONAS_RANG_BAJO;
  } else if (*numero_donas > RANG_DONAS_BAJO && *numero_donas <= RANG_DONAS_MEDIO){
    return PTOS_DONAS_RANG_MEDIO;
  } else if (*numero_donas > RANG_DONAS_MEDIO && *numero_donas <= RANG_DONAS_ALTO){
    return PTOS_DONAS_RANG_ALTO;
  } else if (*numero_donas > RANG_DONAS_ALTO && *numero_donas <= RANG_DONAS_MAXIMO){
    return PTOS_DONAS_RANG_MAXIMO;
  } else {
    return PTOS_DONAS_RANG_NULO;
  }
} 

/*
PRE: el fundador debe ser una opcion de fundador incorrecta 
POST: devuelve true si la el fundador son: LOS ALIENS, MAGIOS OSR o BURNS. False en caso contrario
*/
bool es_fundador_valido(char *fundador_dado){
  return ((*fundador_dado == LOS_ALIENS) || (*fundador_dado == LOS_MAGIOS) || (*fundador_dado == SR_BURNS) || (*fundador_dado == JEBEDIAH_SPRINGFIELD));
}

/*
  PRE: -
  POST: devuelve true si la fecha(año/mes) es una fecha entre 1926/03 - 2026/03. False en caso contrario
*/
bool es_fecha_valida(int *mes_dado, int *anio_dado){
  return(((*anio_dado >= ANIO_LIMITE) && (*anio_dado <= ANIO_ACTUAL)) && ((*mes_dado <= 12) && (*mes_dado > 0)));
}

/*
  PRE: -
  POST: devuelve true si las el numero de donas esta entre 0-12. False en caso contrario
*/
bool numero_donas_validas(int *donas_dadas){
  return ((*donas_dadas >= DONAS_MIN) && (*donas_dadas <= DONAS_MAX));
}
/*
PRE: -
POST: guarda el fundador ingresado por el aspirante 
*/
void pedir_fundador(char *fundador_dado){
  printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns \n" );
  scanf(" %c", fundador_dado);
  
  while ( !es_fundador_valido(fundador_dado) ){
    printf("---'%c' No es una opción, recordá seleccionar [J], [A], [S] o [B]---\n", *fundador_dado);
    printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns \n");      
    scanf(" %c", fundador_dado);
  }
}

/*
PRE:-
POST: guarda en 'puntos' la cantidad de puntos correspondientes se segun el fundador JEBEDIAH +100, LOS ALIENS, SR. BURNS, LOS MAGIOS -20 
*/
void dar_puntos_por_fundador(char *fundador, int *puntos){
  for (int i = 0 ; i < INTENTOS_MAXIMO ;i++){
    printf("---cuentas con %i intento(s)---\n", INTENTOS_MAXIMO-i);
    pedir_fundador(fundador);
    *puntos += puntos_por_fundador(fundador);

    if (*fundador == JEBEDIAH_SPRINGFIELD){
      i = INTENTOS_MAXIMO;
    }
  }
}

/*
PRE: -
POST: guarda en 'secreto' true la respuesta ingresada por el usuario
*/
void pedir_mantener_secreto(bool *secreto){
  char respuesta_dada = '-';
  
  printf("¿Promete mantener en secreto la existencia de los Magios? \n[S] Si \n[N] No\n");
  scanf(" %c", &respuesta_dada);
  
  while ( !((respuesta_dada == NO) || (respuesta_dada == SI)) ){
    printf("---'%c' No es una opción valida recordá seleccionar [S] para Si ó [N] para No---\n", respuesta_dada);
    printf("¿Promete matener en secreto la existencia de los Masgios? \n[S] Si \n[N] No\n");
    scanf(" %c", &respuesta_dada);
  }
  *secreto = (respuesta_dada == SI);
}

/*
PRE: se debe ingresar solo numero enteros
POST: guarda en mes, anio la fecha ingresada por el aspirante
*/
void pedir_fecha_nacimiento(int *anio_dado, int *mes_dado){
  printf("¿Cuál es su fecha de nacimiento?(formato:yyyy/mm)\nFecha:");
  scanf("%d/%d", anio_dado, mes_dado);
  
  while (!(es_fecha_valida(mes_dado, anio_dado)) || ((*anio_dado == ANIO_ACTUAL) && (*mes_dado > MES_ACTUAL) ) || ((*anio_dado == ANIO_LIMITE) && (*mes_dado < MES_LIMITE))){
    printf("---la fecha es incorrecta ó fuera del rango permitido, ejemplo fecha:2000/08---\n");
    printf("¿Cuál es su fecha de nacimiento?(formato:yyyy/mm)\nFecha:");
    scanf("%d/%d", anio_dado, mes_dado);
  }
}

/*
PRE: pide al aspirante un numero entero entre 0(DONAS_MIN), 12(DONAS_MAX) de donas que sacrificará.
POST: guarda en 'numero_de_donas' el numero de donas sacrificadas por el aspirante
*/
void pedir_donas(int *donas_dadas){
	printf("¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	scanf(" %i", donas_dadas);
	
	while ( !(numero_donas_validas(donas_dadas)) ){
	  printf("---el número no es valido---\n¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	  scanf(" %i", donas_dadas);
	}
}

/*
PRE: se debe ingresar solo numero enteros
POST: imprime por pantalla el grado de magio obtenido por el aspirante segun los puntos
*/
void dar_grado_de_magio(int puntos){
  if (puntos < PTOS_GRADO_RECHAZADO){
    printf("-RECHAZADO- \n");
  } else if (puntos >= PTOS_GRADO_RECHAZADO && puntos <= PTOS_GRADO_ASPIRANTE){
    printf("-ASPIRANTE- \n");
  } else if (puntos > PTOS_GRADO_ASPIRANTE && puntos <= PTOS_GRADO_MAGIO_NOVATO){
    printf("-MAGIO NOVATO- \n");
  } else if (puntos > PTOS_GRADO_MAGIO_NOVATO && puntos < PTOS_GRADO_MAGIO){
    printf("-MAGIO-\n");
  } else if (puntos >= PTOS_GRADO_LIDER_SUPREMO){
    printf("-LIDER SUPREMO- \n");
  }
}

int main() {
  char fundador = '-';
  bool secreto = false; 
  int mes = 0;
  int anio = 0;
  int numero_donas = 0;
  int puntos = 0;

  printf("---Prueba de iniciación de Magia---\nObjetivo: determinar si una persona es digna de ingresar a la sociedad secreta.\n\n");

  dar_puntos_por_fundador(&fundador, &puntos);
  if (!(fundador == JEBEDIAH_SPRINGFIELD)){
    printf("-RECHAZADO-\n");
    return 0;
  }
  printf("Acumulaste %i puntos\n\n", puntos);

  pedir_mantener_secreto(&secreto);
  puntos += puntos_por_silencio(&secreto);  
  printf("Acumulaste %i puntos\n\n", puntos);

  pedir_fecha_nacimiento(&anio, &mes);
  int edad = edad_del_aspirante(&anio, &mes);
  if (edad < 18){
    printf("-RECHAZADO-\n");
    return 0;
  } else {
    puntos += puntos_por_edad(&edad);
  }
  printf("Edad: %i\n", edad);
  printf("Acumulaste %i puntos\n\n", puntos);
  
  pedir_donas(&numero_donas);
  puntos += puntos_por_donas(&numero_donas);
  printf("Acumulaste %i puntos\n\n", puntos);

  printf("Con las respuestas brindadas determinamos que tu estado es:");
  dar_grado_de_magio(puntos);

  return 0;
}
