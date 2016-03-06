#include <stdio.h>
#include <stdlib.h>

/* alusão da função que implementa o algoritmo pretendido */
/* allusion to the function that implements the algorithm */
int MaxRepetition (int [], int);

/* variável global para contar as operações de comparação executadas pelo algoritmo */
/* global variable for counting the comparations executed by the algorithm */
int Count = 0;

int main (void)
{
    /* declaração dos arrays de teste - usar o pretendido para cada execução */
    /* declaration of the test arrays - use each one for each execution */

    /* int Array[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; */
    /* int Array[] = { 1, 1, 1, 1, 1, 3, 1, 1, 1, 1 }; */
    /* int Array[] = { 1, 1, 1, 4, 1, 1, 4, 1, 1, 1 }; */
    int Array[] = { 1, 1, 1, 1, 1, 1, 2, 5, 1, 9 };
    /* int Array[] = { 1, 1, 1, 1, 9, 1, 2, 5, 1, 9 }; */
    /* int Array[] = { 1, 1, 1, 9, 1, 2, 8, 3, 7, 1 }; */
    /* int Array[] = { 1, 1, 1, 9, 5, 2, 8, 1, 9, 9 }; */
    /* int Array[] = { 1, 1, 3, 9, 5, 2, 8, 7, 9, 9 }; */
    /* int Array[] = { 1, 1, 6, 0, 5, 2, 8, 7, 9, 9 }; */
    /* int Array[] = { 1, 4, 6, 0, 5, 2, 8, 7, 9, 9 }; */
    /* int Array[] = { 1, 3, 6, 0, 5, 2, 8, 7, 11, 9 }; */

    int NElem = sizeof (Array) / sizeof (int); int Result;

    /* invocação do algoritmo pretendido */
    /* algorithm invocation */
	Result = MaxRepetition (Array, NElem);

	/* apresentação do resultado e do número de comparações executadas pelo algoritmo */
	/* presenting the result and the number of comparations executed by the algorithm */
	fprintf (stdout, "Maxima repeticao de elementos = %12u e custou %3d comparacoes\n", Result, Count);

    exit (EXIT_SUCCESS);
}

/* implementação do algoritmo pretendido */
/* acrescentar a contagem das operações de comparação executadas pelo algoritmo usando a variável global */

/* implementation of the pretended algorithm */
/* do not forget to count the comparations using the global variable */

int MaxRepetition(int array[], int n) {
    if (n < 2)
        return -1;
    /* Repetitions array: */
    /* -> repetitions[0] = element of array */
    /* -> repetitions[1] = number of repetitions of that element */
    int repetitions[n][2];
    int max = 1, secondMax = 0, maxValue = array[0], attrib = 1;
    int attributed = 0;
    repetitions[0][0] = array[0];
    repetitions[0][1] = 1;

    for (int i = 1; i < n; i++) {
        /* Iterate over repetitions array */
        int j;
        for (j = 0; j < attrib; j++) {
            Count++;
            printf("Repetitions value = %d, number of rep = %d\n", repetitions[j][0], repetitions[j][1]);
            if (array[i] == repetitions[j][0]) {
                repetitions[j][1]++;
                attributed++;
                break;
            }
        }
        if (!attributed) {
            attrib++;
            repetitions[j][0] = array[i];
            repetitions[j][1] = 1;
        }
        attributed = 0;

        /* Check if the number of repetitions surpasses max */
        Count++;
        if (repetitions[j][1] > max) {
            /* If the previous max value is different from the new, the second max will be the previous max */
            Count++;
            if (repetitions[j][0] != maxValue)
                secondMax = max;
            max = repetitions[j][1];
            maxValue = repetitions[j][0];
        } else if (repetitions[j][1] > secondMax) {
            Count++;
            secondMax = repetitions[j][1];
        }

        printf("max = %d, maxvalue = %d, secondmax = %d\n", max, maxValue, secondMax);
        /* Check if there is any possibility of existing a number with more repetitions than max */
        if (i >= n / 2 - 1 && i != n - 1) {
            Count++;
            if (secondMax + n - i - 1 <= max)
                break;
        }
    }
    return max;
}
