#include <stdio.h>
#include <stdlib.h>

/* alus�o da fun��o que implementa o algoritmo pretendido */
/* allusion to the function that implements the algorithm */
int MaxRepetition (int [], int);

/* vari�vel global para contar as opera��es de compara��o executadas pelo algoritmo */
/* global variable for counting the comparations executed by the algorithm */
int Count = 0;

int main (void)
{
    /* declara��o dos arrays de teste - usar o pretendido para cada execu��o */
    /* declaration of the test arrays - use each one for each execution */

    /* int Array[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; */
    /* int Array[] = { 1, 1, 1, 1, 1, 3, 1, 1, 1, 1 }; */
    /* int Array[] = { 1, 1, 1, 4, 1, 1, 4, 1, 1, 1 }; */
    /* int Array[] = { 1, 1, 1, 1, 1, 1, 2, 5, 1, 9 }; */
    /* int Array[] = { 1, 1, 1, 1, 9, 1, 2, 5, 1, 9 }; */
    /* int Array[] = { 1, 1, 1, 9, 1, 2, 8, 3, 7, 1 }; */
    /* int Array[] = { 1, 1, 1, 9, 5, 2, 8, 1, 9, 9 }; */
    /* int Array[] = { 1, 1, 3, 9, 5, 2, 8, 7, 9, 9 }; */
    /* int Array[] = { 1, 1, 6, 0, 5, 2, 8, 7, 9, 9 }; */
    /* int Array[] = { 1, 4, 6, 0, 5, 2, 8, 7, 9, 9 }; */
    /* int Array[] = { 1, 3, 6, 0, 5, 2, 8, 7, 11, 9 }; */
  
    int NElem = sizeof (Array) / sizeof (int); int Result;

    /* invoca��o do algoritmo pretendido */
    /* algorithm invocation */    
	Result = MaxRepetition (Array, NElem);

	/* apresenta��o do resultado e do n�mero de compara��es executadas pelo algoritmo */
	/* presenting the result and the number of comparations executed by the algorithm */
	fprintf (stdout, "Maxima repeticao de elementos = %12u e custou %3d comparacoes\n", Result, Count);

    exit (EXIT_SUCCESS);
}

/* implementa��o do algoritmo pretendido */
/* acrescentar a contagem das opera��es de compara��o executadas pelo algoritmo usando a vari�vel global */

/* implementation of the pretended algorithm */
/* do not forget to count the comparations using the global variable */

int MaxRepetition (int array[], int n) {
    if (n < 2)
        return -1;
    
    int repetitions[n][2];
    int atrib = 0, max = 0;

    for(int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < atrib; j++) {
            Count++;
            if (repetitions[j][0] == array[i]) {
                Count++;
                if (repetitions[j][1] + 1 > max)
                    max = repetitions[j][1] + 1;
                atrib--;
                break;
            }
        }
        atrib++;
        repetitions[j][0] = array[i];
        repetitions[j][1]++;
    }
    return max;
}
