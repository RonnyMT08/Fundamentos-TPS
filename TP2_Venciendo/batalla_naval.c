// gcc *.c oponente.o -o batalla_naval -std=c99 -Wall -Wconversion -Werror -lm
// ./batalla_naval <archivo_barcos> <archivo_reporte>

const int MIN_ARGUMENTOS = 1;
const int MAX_ARGMUMENTOS = 3;
const char EXITO = 0;
const char ERROR = -1;

int main(int argc, char* argv){
    if (argc < MIN_ARGUMENTOS || argc > MAX_ARGMUMENTOS){
        printf ("Error en la cantidad de argumentos");
        return ERROR;
    }
    
    printf("inicialización correcta");
    return EXITO;
}