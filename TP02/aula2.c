#include <stdio.h>
#include <stdlib.h>

/* alus�o da fun��o que implementa o algoritmo */
/* allusion to the function that implements the algorithm */
int CountElements (int [], int);

/* vari�vel global para contar as opera��es aritm�ticas executadas pelo algoritmo */
/* global variable for counting the arithmetic operations executed by the algorithm */
int Count = 0;

int main (void)
{
    /* declara��o dos arrays de teste - usar o pretendido para cada execu��o */
    /* declaration of the test arrays - use each one for each execution */

    /* int Array[] = { 10, 3, 15, 7, 9, 20, 11, 25, 27, 29 }; */
    /* int Array[] = { 10, 3, 15, 7, 25, 33, 20, 55, 27, 29 }; */
    /* int Array[] = { 10, 3, 15, 9, 25, 12, 20, 55, 27, 29 }; */
    /* int Array[] = { 10, 3, 15, 9, 25, 12, 50, 55, 27, 29 }; */
    /* int Array[] = { 10, 3, 15, 9, 25, 12, 50, 23, 27, 47 }; */
    /* int Array[] = { 10, 3, 15, 9, 25, 12, 50, 24, 100, 47 }; */
    /* int Array[] = { 10, 3, 15, 9, 25, 12, 50, 24, 100, 48 }; */
    /* int Array[] = { 10, 3, 10, 9, 20, 12, 40, 24, 80, 48 }; */
    int Array[] = { 10, 3, 10, 3, 20, 6, 40, 12, 80, 24 };
 
    int NElem = sizeof (Array) / sizeof (int); int Result;

    /* invoca��o do algoritmo */
    /* algorithm invocation */  
	Result = CountElements (Array, NElem);

	/* apresenta��o do resultado e do n�mero de opera��es aritm�ticas executadas pelo algoritmo */
	/* presenting the result and the number of arithmetic operations executed by the algorithm */
	fprintf (stdout, "No de elementos = %12u e custou %3d operacoes\n", Result, Count);

    exit (EXIT_SUCCESS);
}

/* implementa��o do algoritmo pretendido */
/* acrescentar a contagem das opera��es aritm�ticas executadas pelo algoritmo usando a vari�vel global */

/* implementation of the pretended algorithm */
/* do not forget to count the arithmetic operations using the global variable */

int CountElements (int array[], int n) {
    int countElem = 0;
    int sums[] = {0, 0};
    for (int i = 2; i < n; i++) {
        sums[i % 2] += array[i - 2];
        if (sums[i % 2] == array[i])
            countElem++;
        Count++;
    }
    return countElem;
}
