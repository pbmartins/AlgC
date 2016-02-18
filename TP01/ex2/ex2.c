#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int armstrong(unsigned int alg);

int main() {
    unsigned int n;
    int Test;
    do {
        printf("Numero de algarismos? "); 
        Test = scanf("%d", &n);
        scanf("%*[^\n]"); 
        scanf("%*c");
    } while (Test == 0);
    fprintf(stdout, "Numeros de Armstrong com %d algarismos:\n", n);
    int numOp = armstrong(n);
    fprintf(stdout, "Numero de multiplicacoes: %d\n", numOp);
    return 0;
}

int armstrong(unsigned int alg) {
    int numOp = 0;
    int min = pow(10.0, (double) alg - 1.0);
    int max = 10 * min;
    numOp += alg;
    for (int i = min; i < max; i++) {
        int n = 0;
        for (int j = i; j >= 1; j /= 10) {
            n += pow((double)(j % 10) , (double) alg);
            numOp += alg;
        }
        if (n == i)
            fprintf(stdout, "%d\n", n);
    }
}
