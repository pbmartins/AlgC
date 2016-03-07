/*******************************************************************************

 Ficheiro de implementação do Tipo de Dados Abstrato POLINOMIO (polynomial.c).
 A estrutura de dados de suporte do polinómio é uma estrutura, constituída pelo
 campo de tipo inteiro Degree para armazenar o grau do polinómio e o campo de
 tipo ponteiro Poly, para representar a sequência atribuída dinamicamente, que
 vai armazenar os seus coeficientes reais.
 
 Nome : Pedro de Bastos Martins                                NMec: 76551

 Implementation file of the abstract data type Poly (polynomial.c). The data
 structure to store the polynomial is composed with the integer field Degree for
 keeping the polynomial's degree and the pointer field Poly, that represents the
 allocated array in dynamic memory, for storing the polynomial's real coefficients.

*******************************************************************************/

#include <stdio.h>  
#include <stdlib.h>
#include <math.h>

#include "polynomial.h"    /* Ficheiro de interface do TDA - ADT Interface file */

/************ Definição da Estrutura de Dados Interna do POLINOMIO ************/

struct matrix
{
    unsigned int NL;  /* Numero de linhas */
    unsigned int NC; /* Numero de colunas */
    int **Matrix;      /* ponteiro para os coeficientes do polinómio - pointer to polynomial's coefficients */
};

/*********************** Controlo Centralizado de Erro ************************/

static unsigned int Error = OK;  /* inicialização do erro */

static char *ErrorMessages[] = {
                                 "sem erro - Without Error",
                                 "polinomio(s) inexistente(s) - Polynomial(s) do not exist",
                                 "memoria esgotada - Out of memory",
                                 "ficheiro inexistente - File does not exist",
                                 "grau do polinomio errado - Wrong degree",
                                 "coeficiente inexistente no polinomio - Coefficient does not exist"
                               };

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** Número de mensagens de erro previstas no módulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/************************ Alusão às Funções Auxiliares ************************/

static int ValidPolys (PtPoly, PtPoly);
static void ReducePoly (PtPoly);

/*************************** Definição das Funções ****************************/

void PolyClearError (void)
{
  Error = OK;
}

int PolyError (void)
{
  return Error;
}

char *PolyErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;    /* não há mensagem de erro - no error message */
}

PtMatrix MatrixCreate (unsigned int pnl, unsigned int pnc)
{
    PtMatrix newMatrix;
    unsigned int i, l;

    if (pnl == 0 || pnc == 0) {
        Error = BAD_SIZE;
        return NULL;
    }

    /* Alloc memory to struct */
    if ((newMatrix = (PtMatrix) malloc(sizeof(struct matrix))) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    /* Allocate memory to matrix array */
    if ((newMatrix->Matrix = (int **) calloc(pnl, sizeof(int*))) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    for (i = 0; i < pnc; i++) {
        if ((newMatrix->Matrix[i] = (int *) calloc(pnc, sizeof(int))) == NULL) {
            for (l = 0; l < i; l++)
                free(newMatrix->Matrix[l]);
            free(newMatrix->Matrix);
            free(newMatrix);
            Error = NO_MEM;
            return NULL;
        } 
    }
    
    /* Save number of columns and lines of matrix */
    newMatrix->NL = pnl;
    newMatrix->NC = pnc;

    /* Return vector and OK error code */
    Error = OK;
    return newMatrix;
}

void MatrixDestroy (PtMatrix *pmatrix)
{
    PtMatrix tmpMatrix = *pmatrix;
    unsigned int i;

    /* Check is matrix pointer exists */
    if (tmpMatrix == NULL) {
        Error = NO_MATRIX;
        return ;
    }

    /* Free memory associated with the int* pointer and the struct */
    for (i = 0; i < tmpMatrix->NL; i++)
        free(tmpMatrix->Matrix[i]; 
    free (tmpMatrix->Matrix);
    free (tmpMatrix);

    /* NULL pointer and OK error message */
    Error = OK;
    *pmatrix = NULL;
}

PtPoly PolyCopy (PtPoly ppoly)
{
    PtPoly polyCopy;
    unsigned int i;
    
    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return NULL;
    }

    /* Invoques Poly constructor to copy the struct */
    if ((polyCopy = PolyCreate(ppoly->Degree)) == NULL)
        return NULL;

    /* Copy the double array */
    for (i = 0; i <= ppoly->Degree; i++)
        polyCopy->Poly[i] = ppoly->Poly[i];

    return polyCopy;
}

int PolyDegree (PtPoly ppoly)
{
    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return 0;
    }
    
    /* Returns polynomial degree */
    Error = OK;
    return ppoly->Degree; 
}

void PolyModifyCoefficient (PtPoly ppoly, unsigned int ppos, double pvalue)
{
    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return ;
    }
    
    /* Verifies if the index is valid */
    if (ppos > ppoly->Degree) {
        Error = BAD_INDEX;
        return ;
    }

    /* Storing the new coefficient */
    Error = OK;
    ppoly->Poly[ppos] = pvalue;   
    
}

double PolyObserveCoefficient (PtPoly ppoly, unsigned int ppos)
{
    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return 0.0;
    }

    /* Verifies if the index is valid */
    if (ppos > ppoly->Degree) {
        Error = BAD_INDEX;
        return 0.0;
    }
    
    /* Return the element that was requested */
    Error = OK;
    return ppoly->Poly[ppos];
}

int PolyIsNull (PtPoly ppoly)
{
    unsigned int i;

    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return 0;
    }

    /* Verifies if all coeficients are 0 */
    Error = OK;
    for (i = 0; i <= ppoly->Degree; i++)
        if (ppoly->Poly[i] != 0)
            return 0;

    return 1;
}

PtPoly PolySymmetrical (PtPoly ppoly)
{
    PtPoly symmetrical;
    unsigned int i;

    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return NULL;
    }

    /* Create polynomial */
    if ((symmetrical = PolyCreate(ppoly->Degree)) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    /* Store coefficients */
    for (i = 0; i <= ppoly->Degree; i++)
        symmetrical->Poly[i] = -ppoly->Poly[i];
 
    return symmetrical;
}

PtPoly PolyAddition (PtPoly ppoly1, PtPoly ppoly2)
{
    PtPoly add;
    unsigned int i;

    /* Check validity of polynomials */    
    if (!ValidPolys(ppoly1, ppoly2))
        return NULL;

    /* Creation of the new polynomial */
    if ((add = PolyCreate((ppoly1->Degree >= ppoly2->Degree) ? ppoly1->Degree : ppoly2->Degree)) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    /* Sum of the indexes */
    for (i = 0; i <= add->Degree; i++)
        add->Poly[i] = ppoly1->Poly[i] + ppoly2->Poly[i];

    /* Reduce poly if necessary */
    ReducePoly(add);

    return add;
}

PtPoly PolySubtraction (PtPoly ppoly1, PtPoly ppoly2)
{
    PtPoly sub;
    unsigned int i;

    /* Check validity of polynomials */    
    if (!ValidPolys(ppoly1, ppoly2))
        return NULL;

    /* Creation of the new polynomial */
    if ((sub = PolyCreate((ppoly1->Degree >= ppoly2->Degree) ? ppoly1->Degree : ppoly2->Degree)) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    /* Sum of the indexes */
    for (i = 0; i <= sub->Degree; i++)
        sub->Poly[i] = ppoly1->Poly[i] - ppoly2->Poly[i];

    /* Reduce poly if necessary */
    ReducePoly(sub);

    return sub;
}

PtPoly PolyMultiplication (PtPoly ppoly1, PtPoly ppoly2)
{
    PtPoly mult;
    unsigned int i, j;

    /* Check validity of polynomials */    
    if (!ValidPolys(ppoly1, ppoly2))
        return NULL;

    /* Creation of the new polynomial */
    if ((mult = PolyCreate(ppoly1->Degree + ppoly2->Degree)) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    /* Sum of the indexes */
    for (i = 0; i <= ppoly1->Degree; i++)
        for (j = 0; j <= ppoly2->Degree; j++)
            mult->Poly[i + j] += ppoly1->Poly[i] * ppoly2->Poly[j];

    /* Reduce poly if necessary */
    ReducePoly(mult);

   return mult;
}

int PolyEquals (PtPoly ppoly1, PtPoly ppoly2)
{
    unsigned int i;

    /* Check validity of polynomials */    
    if (!ValidPolys(ppoly1, ppoly2))
        return 0;

    /* Check if they have the same degree */
    if (ppoly1->Degree != ppoly2->Degree)
        return 0;

    /* Sum of the indexes */
    for (i = 0; i <= ppoly1->Degree; i++)
        if (ppoly1->Poly[i] != ppoly2->Poly[i])
            return 0;

    return 1;
}

void PolyStoreFile (PtPoly ppoly, char *pnomef)
{
    FILE *pfile;
    unsigned int i;

    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return ;
    }

    /* Opening file */
    if ((pfile = fopen(pnomef, "w")) == NULL) {
        Error = NO_FILE;
        return ;
    }

    /* Printing values on file */
    fprintf(pfile, "%u\n", ppoly->Degree);

    for (i = 0; i <= ppoly->Degree; i++)
        fprintf(pfile, "%lf\n", ppoly->Poly[i]);

    /* Version with more detail
    for (i = ppoly->Degree; i >= 0; i--) {
        if (ppoly-Poly[i] != 0)
            fprintf(pfile, "%lf * x^%u", ppoly->Poly[i], i);
        if ((i > 1) || ((i == 1) && (ppoly->Poly[0] != 0))
            fprintf(pfile, " + ");
    }
    fprintf(pfile, "\n");
    */
    Error = OK;
    fclose(pfile);    
}

PtPoly PolyCreateFile (char *pnomef)
{
    PtPoly newPoly;
    FILE *pfile;
    unsigned int degree, i;

    /* Opening file */
    if ((pfile = fopen(pnomef, "r")) == NULL) {
        Error = NO_FILE;
        return NULL;
    }

    /* Read polynomial degree */
    fscanf(pfile, "%u", &degree);
    printf("degree = %u\n", degree);
    if (degree < 0) {
        Error = BAD_DEGREE;
        return NULL;
    }
    
    /* Create new polynomial */
    if ((newPoly = PolyCreate(degree)) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    /* Read and store values of polynomial */
    for (i = 0; i <= degree; i++)
        fscanf(pfile, "%lf", newPoly->Poly + i);

    fclose(pfile);
    return newPoly;
}

double PolyEvaluation (PtPoly ppoly, double px)
{
    double result;
    unsigned int i;

    /* Verifies if polynomial exists */
    if (ppoly == NULL) {
        Error = NO_POLY;
        return 0.0;
    }
    
    result = ppoly->Poly[0];
    for (i = 1; i <= ppoly->Degree; i++)
         result += ppoly->Poly[i] * pow(px, i);

    Error = OK;
    return result;
}

/*******************************************************************************
 Função auxiliar que verifica se os dois polinómios existem. Devolve 1 em caso
 afirmativo e 0 em caso contrário. Valores de erro: OK ou NO_POLY.
  
 Auxiliary function to verify if the two polynomials exist. Returns 1 in 
 affirmative case and 0 otherwise. Error codes: OK or NO_POLY. 
*******************************************************************************/

static int ValidPolys (PtPoly ppoly1, PtPoly ppoly2)
{
  /* verifica se os dois polinómios existem - verifies if the two polynomials exist */
  if ((ppoly1 == NULL) || (ppoly2 == NULL)) { Error = NO_POLY; return 0; }

  Error = OK;
  return 1;  /* os dois polinómios existem - they exist */
}

/*******************************************************************************
 Função auxiliar que reduz o polinómio ao menor grau possível. Valores de erro:
 NO_POLY.
  
 Auxiliary function to reduce the polynomial to its least degree. Error codes: NO_POLY. 
  
*******************************************************************************/

static void ReducePoly (PtPoly ppoly)
{
  unsigned int Degree;

  /* verifica se o polinómio existe - verifies if the polynomial exists */
  if (ppoly == NULL) { Error = NO_POLY; return ; }

  Degree = ppoly->Degree;

  while (Degree > 0 && ppoly->Poly[Degree] == 0.0) Degree--;

  if (Degree != ppoly->Degree)
     ppoly->Poly = (double *) realloc (ppoly->Poly, (Degree+1) * sizeof (double));

  ppoly->Degree = Degree;
  Error = OK;
}

