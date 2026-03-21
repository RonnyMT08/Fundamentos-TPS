#include <stdio.h>
int main(){
    int A = 12;
    int B = 4;
    int variable_intercambio = 0;
    printf("los valores son:\n A=%i, B=%i\n", A, B);

    variable_intercambio = A+B;
    A = variable_intercambio-A;
    B = variable_intercambio-B;
    printf("los valores son:\n A=%i, B=%i\n", A, B);

return 0;
}
