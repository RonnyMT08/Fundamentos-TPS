#include <stdio.h>

int main(){
    int vector[10]; /*defino un vector*/
    vector[0] = 4; /*inicializo el vector con un unico valor*/
    int tope = 0; 
    for (int i = 0; i < 6; i++){
        vector[i] = i ;
        tope ++;
    } 
    
    for (int i = 0; i < tope; i++){
        printf("en la posición %i el valor toma %i\n", i, vector[i]);
    }

    return 0;
}