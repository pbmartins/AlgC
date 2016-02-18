#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static unsigned int factorial[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

int factoriao(unsigned int limit);

int main() {
    int n = pow(10.0, 6.0);
    fprintf(stdout, "Factorioes ate %d:\n", n);
    int numOp = 6 + factoriao(n);
    fprintf(stdout, "Numero de multiplicacoes: %d\n", numOp);
    return 0;
}

int factoriao(unsigned int limit) {
    int numOp = 0;

    for (int i = 0; i < limit; i++) {
        int n = 0;
        for (int j = i; j >= 1; j /= 10) {
            numOp += 2;
            n += factorial[j % 10];
        }
        if (n == i)
            fprintf(stdout, "%d\n", n);
    }
    return numOp;
}
