#include <stdio.h>
#include <stdbool.h>
/*declaraciones*/
#define MAX 1000

/* 
PRE: se debe ingresar un numero entero.
POST: devuelve un numero entero
*/
int pide_numero(){
    int numero_dado;
    printf("Ingrese un número entero:\n");
    scanf("%i", numero_dado);
    while (!isdigit(numero_dado));
        printf("Estimado usuario '%c' no es un número entero", numero_dado);
        scanf("%i", numero_dado);
    return numero_dado;
}

/*
PRE: -
POST: devuelve el indice de posicion del numero buscado
*/
void busca_elemento_vector(int* vector, int elemento){
    bool elemento_encontrado = false;
    int i = 0;
    int posicion;
    while (elemento_encontrado == false){
        if (vector[i] == elemento){
            elemento_encontrado = true;
            posicion = i;
        } else {
            i++;
        }
    }
    return posicion;
}

/*
PRE: se debe ingresar un numero entero
POST: 
*/
void pedir_posicion(){
    int posicion;
    printf("Indique la posición que desea eliminar:\n");
    scanf("%i", posicion);
}

/*
PRE:
POST:
*/
void crear_vector(int* vector, int tope){
    for (int i = 0; i < tope; i++){
        vector[i] = (4+i);
    }
}

/*Inserccion ordenada*/
void agregar_numero_vector(int* vector, int numero, int* tope){
    vector[(*tope)] = numero;
    (*tope) ++;
}

void mostrar_vector(int* vector, int tope){
    for (int i = 0; i < tope; i++){
        printf("el valor es: %i\n", vector[i]);
    }
}
void eliminar_numero_vector(int* vector, int posicion, int* tope){
    for (int i = posicion; i < *tope; i++ ){
        vector[i] = vector[i + 1]; 
    }
    (*tope)--;
}


/*
void ordenar_vector(int* vector, int tope){
    int indice_min = 0;
    int numero_minimo = vector[indice_min];
    int num_aux = 0;
    for (int i = 0; i < tope; i++){
        numero_minimo = vector[i];
        for (int j = i+1; j < tope; j++){
            if (vector[i] > vector[j]){
                numero_minimo = j;
            }
        } 
    }
}
*/
/*metodo burbujeo*/
void ordenar_burbujeo(int* vector, int tope){
    for (int i = 0; i < tope; i++){
        //con bectores
    }
}
/*metodo insercion*/
void insercion (int* vector,  int tope){

}
/*metodo Quicksort*/
void insercion (int* vector,  int tope){
    
}


int main(){
    int vector_alumnos[MAX];
    int tope = 6;
    int numero, posicion;
    crear_vector(vector_alumnos, tope);

    printf("---EL VECTOR ES---\n");
    mostrar_vector(vector_alumnos, tope);
    pedir_numero(&numero);

    printf("---VECTOR ACUTALIZADO---\n");
    agregar_numero_vector(vector_alumnos, numero, &tope);
    mostrar_vector(vector_alumnos, tope);

    printf("---VECTOR ACUTALIZADO(elemento eliminado)---\n");
    pedir_posicion(&posicion);
    eliminar_numero_vector(vector_alumnos, posicion, &tope);
    mostrar_vector(vector_alumnos, tope);

    printf("---ELEMENTO BUSCADO---\n");
    int elemento;
    pedir_numero(&elemento);
    buscar_elemento_vector(vector_alumnos, elemento, &posicion);
    if (elemento == vector_alumnos[posicion]){
        printf("el elemento %i existe.\n", elemento);
    } else {
        printf("El elemento %i no existe.\n", elemento);
    }

    printf("---ORDENAR VECTOR---");
    ordenar_vector(vector_alumnos, tope);
    mostrar_vector(vector_alumnos, tope);

    return 0;
}