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
const int PTOS_DECIR_SECRETO = -300;

const int ANIO_ACTUAL = 2026;
const int MES_ACTUAL = 03;
const int ANIO_LIMITE = 1926;
const int MES_LIMITE = 03; 

const int EDAD_MINIMA = 18;
const int INDICE_PTOS_POR_EDAD = 2;

const int DONAS_MAX = 12;
const int DONAS_MIN = 0;

const int RANG_DONAS_NULO = 0;
const int RANG_DONAS_BAJO = 3;
const int RANG_DONAS_MEDIO = 6;
const int RANG_DONAS_ALTO = 9;
const int RANG_DONAS_MAXIMO = 12;

const int PTOS_DONAS_RANG_NULO = -100;
const int PTOS_DONAS_RANG_BAJO = 10;
const int PTOS_DONAS_RANG_MEDIO = 40;
const int PTOS_DONAS_RANG_ALTO = 70;
const int PTOS_DONAS_RANG_MAXIMO = 120;

const int PTOS_GRADO_RECHAZADO = 0;
const int PTOS_GRADO_ASPIRANTE = 150;
const int PTOS_GRADO_MAGIO_NOVATO = 250;
const int PTOS_GRADO_MAGIO = 349;
const int PTOS_GRADO_LIDER_SUPREMO = 350;

/*
PRE: el fundador debe ser JEBEDIAH SPRINGFIEL, LOS ALIENS, SR. BRUNS O LOS MAGIOS. Los intentos deben ser inicializados previamente
POST: devuelve los puntos por acreditar por seleccionar un de fundador.
*/
int calcula_puntos_fundador(char fundador, int intentos){
  if ( (intentos == 0) && (fundador == JEBEDIAH_SPRINGFIELD) ){
    return PTOS_FUNDADOR_CORRECTO;
  } else {
    return (PTOS_FUNDADOR_CORRECTO - ((intentos)*(PTOS_FUNDADOR_INCORRECTO)));
  }
}

/*
PRE: el secreto debe ser true(guardado) o false(no guardado)
POST: devuelve los puntos obtenidos por guardar o no guardar el silencio, en  (no decir el secreto)
*/
int calcula_puntos_guardar_secreto(bool secreto){
  if (secreto == true){
    return (PTOS_GUARDAR_SECRETO);
  } else {
    return (PTOS_DECIR_SECRETO);
  }
}

/*
  PRE: la fecha ingresada tiene que ser enteros año, mes
  POST: devuelve la edad
*/
int calcula_edad(int anio, int mes){
  int edad = ANIO_ACTUAL - anio;
  if (MES_ACTUAL < mes) {
      edad -= 1;
  }
  return edad;
}

/*
PRE: la edad tiene que ser un numero entero
POST: devuelve los puntos acreditados segun el multiplo(INDICE_PTOS_POR_EDAD) definido (edad*multiplo) del aspirante
*/
int calcula_puntos_edad(int edad){
  return (INDICE_PTOS_POR_EDAD*(edad));
}

/*
PRE: el numero de donas deberá ser un entero.
POST: devuelve los puntos obtenidos según el rango en el que se encuentre el numero de donas.
*/
int calcula_puntos_donas(int numero_donas){
  if ( numero_donas == RANG_DONAS_NULO ){
    return PTOS_DONAS_RANG_NULO;
  } else if (numero_donas > RANG_DONAS_NULO && numero_donas <= RANG_DONAS_BAJO){
    return PTOS_DONAS_RANG_BAJO;
  } else if (numero_donas > RANG_DONAS_BAJO && numero_donas <= RANG_DONAS_MEDIO){
    return PTOS_DONAS_RANG_MEDIO;
  } else if (numero_donas > RANG_DONAS_MEDIO && numero_donas <= RANG_DONAS_ALTO){
    return PTOS_DONAS_RANG_ALTO;
  } else if (numero_donas > RANG_DONAS_ALTO && numero_donas <= RANG_DONAS_MAXIMO){
    return PTOS_DONAS_RANG_MAXIMO;
  } else {
    return PTOS_DONAS_RANG_NULO;
  }
} 

/*
PRE: el fundador_dado tiene que ser un caracter
POST: devuelve true si el fundador_dado es LOS MAGIOS, SR. BRUNS, LOS ALIENS. False en caso contrario
*/
bool es_fundador_incorrecto(char fundador_dado){
  return ((fundador_dado == LOS_ALIENS) || (fundador_dado == LOS_MAGIOS) || (fundador_dado == SR_BURNS));
}

/*
PRE:el fundador_dado tiene que ser un caracter
POST: devuelve true si el fundador es JEBADIAH SPRINGFIELD, False en caso contrario
*/
bool es_fundador_correcto(char fundador_dado){
  return (fundador_dado == JEBEDIAH_SPRINGFIELD);
}

/*
  PRE: el mes_dado, anio_dado tienen que ser numeros enteros
  POST: devuelve true si la fecha(año/mes) es una fecha entre 1926/03 - 2026/03. False en caso contrario
*/
bool es_fecha_valida(int mes_dado, int anio_dado){
  return (((anio_dado >= ANIO_LIMITE) && (anio_dado <= ANIO_ACTUAL)) && ((mes_dado <= 12) && (mes_dado > 0)));
}

/*
PRE: la edad deberá ser un numero entero.
POST: devuelve true si la edad es mayor a 18, False en caso contrario
*/
bool es_mayor_edad(int edad){
  return (edad >= EDAD_MINIMA);
}

/*
  PRE: las donas_dadas debera ser un numero entero
  POST: devuelve true si las el numero de donas esta entre 0-12. False en caso contrario
*/
bool es_numero_donas(int donas_dadas){
  return ((donas_dadas >= DONAS_MIN) && (donas_dadas <= DONAS_MAX));
}

/*
PRE: el fundador_dado debe ser un caracter J(hebediah springfield), A(los aliens), S(los magios), B(señor burns, en otro caso no cuenta como respuesta
POST: guarda en 'fundador' la respuesta ingresado por el usuario, evalua los intentos que utilizados. 
*/
void pedir_fundador(char *fundador_dado, int *intentos ){
  printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los Aliens \n[S] Los Magios \n[B] Sr. Burns \n" );
  scanf(" %c", fundador_dado);

  while ( ( !es_fundador_correcto(*fundador_dado)) && ((*intentos + 1 < INTENTOS_MAXIMO) || !(es_fundador_incorrecto(*fundador_dado))) ){
    if (es_fundador_incorrecto(*fundador_dado)){
      *intentos += 1;
      printf("---La respuesta es incorrecta. Cuentas con %i intento(s) mas---\n\n", INTENTOS_MAXIMO -(*intentos));  
    } else if (!es_fundador_incorrecto(*fundador_dado)) {
      printf("---'%c' No es una opción, si crees que el fundador es Jebediah Springfield elegí [J], Los aliens elegí [A], Los Magios [S] o El Sr. Burns [B]---\n\n", *fundador_dado);
    }

    printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los Aliens \n[S] Los Magios \n[B] Sr. Burns \n" );
    scanf(" %c", fundador_dado);
  }
}

/*
PRE: -
POST: muestra por pantalla los puntos acumulados 
*/
void mostrar_puntos(int puntos){
  printf("---Acumulaste %i puntos---\n\n", puntos);
}

/*
PRE: la respuesta validas son S ó N. en caso contrario no cuenta como respuesta
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
PRE: se debe ingresar anio (entre 1926-2026), mes(entre 0-12)solo numero enteros, 01,02 valen como enteros
POST: guarda en mes, anio la fecha ingresada por el aspirante
*/
void pedir_fecha_nacimiento(int *anio_dado, int *mes_dado){
  printf("¿Cuál es su fecha de nacimiento?(formato:yyyy/mm)\nFecha:");
  scanf("%d/%d", anio_dado, mes_dado);
  
  while (!(es_fecha_valida(*mes_dado, *anio_dado)) || ((*anio_dado == ANIO_ACTUAL) && (*mes_dado > MES_ACTUAL) ) || ((*anio_dado == ANIO_LIMITE) && (*mes_dado < MES_LIMITE))){
    printf("---la fecha es incorrecta ó fuera del rango permitido, ejemplo fecha:2000/08---\n");
    printf("¿Cuál es su fecha de nacimiento?(formato:yyyy/mm)\nFecha:");
    scanf("%d/%d", anio_dado, mes_dado);
  }
}

/*
PRE: se debe ingresar un numero entero entre 0(DONAS_MIN), 12(DONAS_MAX) de donas que sacrificará.
POST: guarda en 'numero_de_donas' el numero de donas sacrificadas por el aspirante
*/
void pedir_donas(int *donas_dadas){
	printf("¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	scanf(" %i", donas_dadas);
	
	while ( !(es_numero_donas(*donas_dadas)) ){
	  printf("---el número no es valido---\n¿Cuántas donas estaría dispuesto a sacrificar para el Número Uno?\ndispones de [0] cero donas a [12] doce donas\n");
	  scanf(" %i", donas_dadas);
	}
}

/*
PRE: se debe ingresar solo numero enteros entre
POST: imprime por pantalla el grado de magio obtenido por el aspirante segun los puntos RECHAZAFO < 0, ASPIRANTE (0-150), MAGIO NOVATO(151-250), MAGIO(251-349), LIDER SUPREMO(<= 350) 
*/
void mostrar_grado_magio(int puntos){
  if (puntos < PTOS_GRADO_RECHAZADO){
    printf("-RECHAZADO- \n");
  } else if (puntos >= PTOS_GRADO_RECHAZADO && puntos <= PTOS_GRADO_ASPIRANTE){
    printf("-ASPIRANTE- \n");
  } else if (puntos > PTOS_GRADO_ASPIRANTE && puntos <= PTOS_GRADO_MAGIO_NOVATO){
    printf("-MAGIO NOVATO- \n");
  } else if (puntos > PTOS_GRADO_MAGIO_NOVATO && puntos <= PTOS_GRADO_MAGIO){
    printf("-MAGIO-\n");
  } else if (puntos >= PTOS_GRADO_LIDER_SUPREMO){
    printf("-LIDER SUPREMO- \n");
  }
}

int main() {
  char fundador = '-';
  
  int intentos_usados = 0;
  int anio_nacido = 0;
  int mes_nacido = 0;
  int donas_dadas = 0;
  int total_puntos = 0;
  
  bool secreto_guardado = false; 
  
  printf("---Prueba de iniciación de Magia---\nObjetivo: determinar si una persona es digna de ingresar a la sociedad secreta.\n\n");
  
  pedir_fundador(&fundador, &intentos_usados);
  if (!(fundador == JEBEDIAH_SPRINGFIELD)){
    printf("-RECHAZADO-\n");
    return 0;
  } else {
    total_puntos += calcula_puntos_fundador(fundador, intentos_usados);
    mostrar_puntos(total_puntos);
  }

  pedir_mantener_secreto(&secreto_guardado);
  if (secreto_guardado == true || secreto_guardado == false){
    total_puntos += calcula_puntos_guardar_secreto(secreto_guardado);  
    mostrar_puntos(total_puntos);
  }

  pedir_fecha_nacimiento(&anio_nacido, &mes_nacido);
  int edad = calcula_edad(anio_nacido, mes_nacido);
  if (es_mayor_edad(edad)){
    total_puntos += calcula_puntos_edad(edad);
    printf("Edad: %i\n", edad);
  } else {
    printf("-RECHAZADO-\n");
    return 0;
  }
  mostrar_puntos(total_puntos);
  
  pedir_donas(&donas_dadas);
  total_puntos += calcula_puntos_donas(donas_dadas);
  mostrar_puntos(total_puntos);

  printf("Con las respuestas brindadas determinamos que tu estado es:");
  mostrar_grado_magio(total_puntos);

  return 0;
}
