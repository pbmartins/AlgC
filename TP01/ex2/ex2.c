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
    int pot[10] = {pow(0.0, (double) alg), pow(1.0, (double) alg), pow(2.0, (double) alg), pow(3.0, (double) alg), pow(4.0, (double) alg), 
        pow(5.0, (double) alg), pow(6.0, (double) alg), pow(7.0, (double) alg), pow(8.0, (double) alg), pow(9.0, (double) alg)};
    int numOp = 10 * alg;
    int min = pow(10.0, (double) alg - 1.0);
    int max = 10 * min;
    numOp += alg;
    for (int i = min; i < max; i++) {
        int n = 0;
        for (int j = i; j >= 1; j /= 10)
            n += pot[j % 10];
        if (n == i)
            fprintf(stdout, "%d\n", n);
    }
    return numOp;
}
