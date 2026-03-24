#include <stdio.h>
#include <stdbool.h>

const char RECHAZADO = 'R';
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
  if ( *numero_donas == 0 ){
    return 100;
  } else if (*numero_donas > 0 && *numero_donas <= 3){
    return 10;
  } else if (*numero_donas > 3 && *numero_donas <= 6){
    return 40;
  } else if (*numero_donas > 6 && *numero_donas <= 9){
    return 70;
  } else if (*numero_donas > 9 && *numero_donas <= 12){
    return 120;
  } else {
    return 0;
  }
} 

/*
PRE: el fundador debe ser una opcion de fundador incorrecta 
POST: devuelve true si la el fundador son: LOS ALIENS, MAGIOS OSR o BURNS. False en caso contrario
*/
bool es_fundador_valido(char *fundador_dado){
  return ((*fundador_dado == LOS_ALIENS) || (*fundador_dado == LOS_MAGIOS) || (*fundador_dado == SR_BURNS));
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
POST: guarda el estado_del_aspirante a 'ARECHAZADO' si la respuesta dada es inccorrecta.  
*/
void pedir_fundador(char *fundador_dado, int *puntos, char *estado_del_aspirante){
  int numero_intentos = 3;
  int contador = 0;
    printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns \n---cuentas con %d intento(s) más---\n", numero_intentos );
    scanf(" %c", fundador_dado);
    
    while ( !(*fundador_dado == JEBEDIAH_SPRINGFIELD) && (contador < numero_intentos - 1) ){
      printf("---'%c' No es una opción correcta---\n", *fundador_dado);
      
      if (es_fundador_valido(fundador_dado)){
        contador ++;
        *puntos += puntos_por_fundador(fundador_dado);
      }
      printf("¿Quién fundó realmente Springfield? \n[J] Jebediah Springfield \n[A] Los aliens \n[S] Los Magios \n[B] Sr. Burns \n---cuentas con %i intento(s) más---\n", numero_intentos - contador);
      scanf(" %c", fundador_dado);
    }
    if (*fundador_dado == JEBEDIAH_SPRINGFIELD){
      puntos += puntos_por_fundador(fundador_dado);
    } else if (contador == 2){
      *estado_del_aspirante = RECHAZADO;
    }
}

/*
PRE:-
POST: guarda el estado_del_aspirante a 'RECHAZADO' si revela la existencia de los magios.
*/
void pedir_mantener_secreto(bool *secreto){
  char respuesta_dada = '-';
  
  printf("¿Promete mantener en secreto la existencia de los Magios? \n[S] Si \n[N] No\n");
  scanf(" %c", &respuesta_dada);
  
  while ( !((respuesta_dada == NO) || (respuesta_dada == SI)) ){
    printf("---'%c' No es una opción valida---\n", respuesta_dada);
    printf("¿Promete matener en secreto la existencia de los Masgios? \n[S] Si \n[N] No\n");
    scanf(" %c", &respuesta_dada);
  }
  *secreto = (respuesta_dada == SI);
}


/*
PRE: se debe ingresar solo numero enteros
POST: guarda en edad_del_aspirante
*/
void pedir_fecha_nacimiento(int *edad){
  int anio_dado = 0;
  int mes_dado = 0;

  printf("¿Cuál es su fecha de nacimiento?(formato:yyyy/mm)\n Fecha:");
  scanf(" %i/ %i", &anio_dado, &mes_dado);
  
  if ( es_fecha_valida(&mes_dado, &anio_dado)){
    *edad = ANIO_ACTUAL - anio_dado;
  } else {
    *edad = 0;
  }
}

/*
PRE: pide al aspirante un numero entero entre 0(DONAS_MIN), 12(DONAS_MAX) de donas que sacrificará.
POST: valida y actualiza el numero de donas sacrificadas.
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
    printf("-RECHAZADO-");
    return 0;
  }
  
  pedir_mantener_secreto(&secreto);
  puntos += puntos_por_silencio(&secreto); 

  pedir_fecha_nacimiento(&edad_aspirante);
  printf("tienes %i años\n", edad_aspirante);
  if (edad_aspirante < 18){
    printf("-RECHAZADO-");
    return 0;
  } else {
    puntos += puntos_por_edad(&edad_aspirante);
  }
  
  pedir_donas(&numero_donas);
  puntos += puntos_por_donas(&numero_donas);

  printf("puntos obtenidos: %i\n", puntos);
  dar_grado_de_magio(puntos);

  return 0;
}
