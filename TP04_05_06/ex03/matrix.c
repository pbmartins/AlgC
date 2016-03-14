/*******************************************************************************

 Ficheiro de implementa��o do Tipo de Dados Abstracto MATRIZ (matrix.c).
 A estrutura de dados de suporte da matriz � uma estrutura, constitu�da pelo
 campo NL para armazenar o n�mero de linhas da matriz, o campo NC para armazenar
 o n�mero de colunas da matriz e o campo de tipo ponteiro para ponteiro Matrix
 para armazenar os seus NLxNC elementos reais, que v�o ser armazenados numa
 sequ�ncia bidimensional atribu�da dinamicamente.

 Nome : Pedro de Bastos Martins                             NMec: 76551

 Implementation file of the abstract data type MATRIX (matrix.c). The data
 structure to store the matrix is composed with the integer field NL for keeping
 the matrix's number of lines, the integer field NC for keeping the matrix's number
 of columns and the pointer field Matrix, that represents the allocated bi-dimensional
 array in dynamic memory, for storing the matrix's NLxNC real elements.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"  /* Ficheiro de interface do TDA - ADT Interface file */

/************* Defini��o da Estrutura de Dados Interna da MATRIZ **************/

struct matrix
{
  unsigned int NL;  /* numero de linhas da matriz - matrix's number of lines */
  unsigned int NC;  /* numero de colunas da matriz - matrix's number of columns */
  double **Matrix;  /* ponteiro para a matriz a ser alocada - pointer to matrix's elements */
};

/*********************** Controlo Centralizado de Erro ************************/

static unsigned int Error = OK;  /* inicializa��o do erro */

static char *ErrorMessages[] = {
                                 "sem erro - Without error",
                                 "matriz(es) inexistente(s) - Matrix(Matrices) do not exist",
                                 "memoria esgotada - Out of memory",
                                 "ficheiro inexistente - File does not exist",
                                 "dimensao errada - Wrong size",
                                 "elemento inexistente na matriz - Element does not exist",
                                 "matrizes com dimensoes diferentes - Matrices with different sizes",
                                 "matrizes nao encadeadas - Matrices not chained",
								 "ponteiro nulo - Null pointer",
								 "matriz nao quadrada - Matrix not square",
								 "linha inexistente - Line does not exist",
								 "coluna inexistente - Column does not exist"
                               };

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** N�mero de mensagens de erro previstas no m�dulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/************************ Alus�o �s Fun��es Auxiliares ************************/

static int EqualSizeMatrices (PtMatrix, PtMatrix);
static int ChainedMatrices (PtMatrix, PtMatrix);
static double Determinant (double *, unsigned int, unsigned int);

/*************************** Defini��o das Fun��es ****************************/

void MatrixClearError (void)
{
  Error = OK;
}

int MatrixError (void)
{
  return Error;
}

char *MatrixErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;  /* n�o h� mensagem de erro - no error message */
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
    if ((newMatrix->Matrix = (double **) calloc(pnl, sizeof(double*))) == NULL) {
        free(newMatrix);
        Error = NO_MEM;
        return NULL;
    }

    for (i = 0; i < pnl; i++) {
        if ((newMatrix->Matrix[i] = (double *) calloc(pnc, sizeof(double))) == NULL) {
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

void MatrixDestroy (PtMatrix *pmat)
{
    PtMatrix tmpMatrix = *pmat;
    unsigned int i;

    /* Check is matrix pointer exists */
    if (tmpMatrix == NULL) {
        Error = NO_MATRIX;
        return ;
    }

    /* Free memory associated with the int* pointer and the struct */
    for (i = 0; i < tmpMatrix->NL; i++)
        free(tmpMatrix->Matrix[i]);
    free (tmpMatrix->Matrix);
    free (tmpMatrix);

    /* NULL pointer and OK error message */
    Error = OK;
    *pmat = NULL;
}

PtMatrix MatrixCopy (PtMatrix pmat)
{
    PtMatrix matrixCopy;
    unsigned int i, j;

    /* Verifies if matrix exists */
    if (pmat == NULL) {
        Error = NO_MATRIX;
        return NULL;
    }

    /* Call matrix constructor */
    if ((matrixCopy = MatrixCreate(pmat->NL, pmat->NC)) == NULL)
        return NULL;

    /* Copy elements to new matrix */
    for (i = 0; i < pmat->NL; i++)
        for (j = 0; j < pmat->NC; j++)
            matrixCopy->Matrix[i][j] = pmat->Matrix[i][j];

    return matrixCopy;
}

void MatrixSize (PtMatrix pmat, unsigned int *pnl, unsigned int *pnc)
{
  /* verifica se a matriz existe - verifies if matrix exist */
  if (pmat == NULL)
  {
    Error = NO_MATRIX;
    *pnl = *pnc = 0;
  }
  else
  {
    Error = OK;
    *pnl = pmat->NL;
    *pnc = pmat->NC;
  }
}

void MatrixModifyElement (PtMatrix pmat, unsigned int pl, unsigned int pc, double pval)
{
  /* verifica se a matriz existe - verifies if matrix exist */
  if (pmat == NULL) { Error = NO_MATRIX; return; }

  /* valida��o do elemento pretendido - validating the element position */
  pl--;
  if (pl >= pmat->NL) { Error = BAD_ROW; return; }
  pc--;
  if (pc >= pmat->NC) { Error = BAD_COLUMN; return; }

  Error = OK;
  /* escrita do valor na componente pretendida da matriz - storing the new value in the element */
  pmat->Matrix[pl][pc] = pval;
}

double MatrixObserveElement (PtMatrix pmat, unsigned int pl, unsigned int pc)
{
  /* verifica se a matriz existe - verifies if matrix exist */
  if (pmat == NULL) { Error = NO_MATRIX; return 0.0; }

  /* valida��o do elemento pretendido - validating the element position */
  pl--;
  if (pl >= pmat->NL) { Error = BAD_ROW; return 0.0; }
  pc--;
  if (pc >= pmat->NC) { Error = BAD_COLUMN; return 0.0; }


  Error = OK;
  /* devolve o valor armazenado na componente pretendida da matriz - returning the element value */
  return pmat->Matrix[pl][pc];
}

PtMatrix MatrixTranspose (PtMatrix pmat)
{
    PtMatrix transposedMatrix;
    unsigned int i, j;

    /* Verifies if matrix exists */
    if (pmat == NULL) {
        Error = NO_MATRIX;
        return NULL;
    }

    /* Call matrix constructor */
    if ((transposedMatrix = MatrixCreate(pmat->NC, pmat->NL)) == NULL)
        return NULL;

    for (i = 0; i < pmat->NL; i++)
        for (j = 0; j < pmat->NC; j++)
            transposedMatrix->Matrix[j][i] = pmat->Matrix[i][j];

    return transposedMatrix;
}

PtMatrix MatrixAddition (PtMatrix pmat1, PtMatrix pmat2)
{
    PtMatrix add;
    unsigned int i, j;

    /* Verifies if matrices have the same dimensions */
    if (!EqualSizeMatrices(pmat1, pmat2))
        return NULL;

    /* Call the constructor */
    if ((add = MatrixCreate(pmat1->NL, pmat1->NC)) == NULL)
        return NULL;

    /* Sum the elements */
    for (i = 0; i < add->NL; i++)
        for (j = 0; j < add->NC; j++)
            add->Matrix[i][j] = pmat1->Matrix[i][j] + pmat2->Matrix[i][j];

    return add;
}

PtMatrix MatrixSubtraction (PtMatrix pmat1, PtMatrix pmat2)
{
    PtMatrix sub;
    unsigned int i, j;

    /* Verifies if matrices have the same dimensions */
    if (!EqualSizeMatrices(pmat1, pmat2))
        return NULL;

    /* Call the constructor */
    if ((sub = MatrixCreate(pmat1->NL, pmat1->NC)) == NULL)
        return NULL;

    /* Subtract the elements */
    for (i = 0; i < sub->NL; i++)
        for (j = 0; j < sub->NC; j++)
            sub->Matrix[i][j] = pmat1->Matrix[i][j] - pmat2->Matrix[i][j];

    return sub;
}

PtMatrix MatrixMultiplication (PtMatrix pmat1, PtMatrix pmat2)
{
    PtMatrix mult;
    unsigned int i, j, k;

    /* Verifies if matrices have the same dimensions */
    if (!ChainedMatrices(pmat1, pmat2))
        return NULL;

    /* Call the constructor */
    if ((mult = MatrixCreate(pmat1->NL, pmat2->NC)) == NULL)
        return NULL;

    /* Do the operations over the elements */
    for (i = 0; i < pmat1->NL; i++) {
        for (j = 0; j < pmat2->NC; j++) {
            mult->Matrix[i][j] = 0;
            for (k = 0; k < pmat1->NC; k++)
                mult->Matrix[i][j] += pmat1->Matrix[i][k] * pmat2->Matrix[k][j];
        }
    }

    return mult;
}

PtMatrix MatrixMultByScalar (PtMatrix pmat, double pvalue)
{
    PtMatrix multScalar;
    unsigned int i, j;

    /* Verifies if matrix exists */
    if (pmat == NULL) {
        Error = NO_MATRIX;
        return NULL;
    }

    /* Call the constructor */
    if ((multScalar = MatrixCreate(pmat->NL, pmat->NC)) == NULL)
        return NULL;

    /* Do the operations over the elements */
    for (i = 0; i < multScalar->NL; i++)
        for (j = 0; j < multScalar->NC; j++)
            multScalar->Matrix[i][j] = pmat->Matrix[i][j] * pvalue;

    return multScalar;
}

double MatrixDeterminant (PtMatrix pmat)
{
    double determinant = 0;
    double* tmpMatrix;
    unsigned int i, j, k;

    /* validar se a matriz existe e � quadrada - verify if matrix exist and is square */
    if (!MatrixIsSquare(pmat)) {
        Error = NO_SQUARE;
        return 0.0;
    }

    /* Create a temporary unidimensional double pointer array (as if it was the matrix) to use it on Determinant function */
    /* preparar a invoca��o da fun��o interna - prepare for invoking the internal function */
    if ((tmpMatrix = (double*) calloc(pmat->NL * pmat->NC, sizeof(double))) == NULL) {
        Error = NO_MEM;
        return -1.0;
    }

    for (i = 0, k = 0; i < pmat->NL; i++)
        for (j = 0; j < pmat->NC; j++)
            tmpMatrix[k++] = pmat->Matrix[i][j];

    /* Call the Determinant function */
    determinant = Determinant(tmpMatrix, pmat->NL, pmat->NC);

    /* libertar a mem�ria din�mica alocada e devolver o resultado - free the memory and return the result */
    free(tmpMatrix);

    return determinant;
}

int MatrixIsSquare (PtMatrix pmat)
{
	if (pmat == NULL) { Error = NO_MATRIX; return 0; }

	Error = OK;
	return pmat->NL == pmat->NC;
}

int MatrixIsSymmetrical (PtMatrix pmat)
{
    unsigned int i, j;

    /* Verifies if matrix exists and it is squared */
    if (!MatrixIsSquare(pmat))
        return 0;

    /* Verifies if the values are the same */
    /* There's no need to verify the main diagonal values (when i == j) */
    for (i = 1; i < pmat->NL; i++)
        for (j = 0; j < i; j++)
            if (pmat->Matrix[i][j] != pmat->Matrix[j][i])
                return 0;

    return 1;
}

int MatrixEquals (PtMatrix pmat1, PtMatrix pmat2)
{
  unsigned int l, c;

  /* valida��o das matrizes - validating the existence of the two matrices */
  if (!EqualSizeMatrices (pmat1, pmat2)) return 0;

  Error = OK;
  /* compara��o dos elementos das duas matrizes - comparing the respective elements */
  for (l = 0; l < pmat1->NL; l++)
    for (c = 0; c < pmat1->NC; c++)
      if (pmat1->Matrix[l][c] != pmat2->Matrix[l][c]) return 0;

  return 1;  /* as matrizes s�o iguais - the two matrices are equal */
}

PtMatrix MatrixSubMatrix (PtMatrix pmat, unsigned int pl, unsigned int pc)
{
    PtMatrix subMatrix;
    unsigned int i, j, l, c;

    /* Verifies if matrix exists */
    if (pmat == NULL) {
        Error = NO_MATRIX;
        return NULL;
    }

    /* Verifies if values of pl and pc are within NL anc NC */
    if (pl < 1 || pl > pmat->NL) {
        Error = BAD_ROW;
        return NULL;
    }

    if (pc < 1 || pc > pmat->NC) {
        Error = BAD_COLUMN;
        return NULL;
    }

    /* Calls the constructor of the submatrix */
    if ((subMatrix = MatrixCreate(pmat->NL - 1, pmat->NC - 1)) == NULL)
        return NULL;

    /* Copy the elements to submatrix */
    for (i = 0, l = 0; i < pmat->NL; i++) {
        if (i == pl - 1)
            continue;
        for (j = 0, c = 0; j < pmat->NC; j++) {
            if (j == pc - 1)
                continue;
            subMatrix->Matrix[l][c] = pmat->Matrix[i][j];
            c++;
        }
        l++;
    }

    return subMatrix;
}

void MatrixStoreFile (PtMatrix pmat, char *pnomef)
{
  FILE *PtF; unsigned int l, c;

  /* verifica se a matriz existe - verifies if matrix exists */
  if (pmat == NULL) { Error = NO_MATRIX; return ; }

  /* abertura com validacao do ficheiro para escrita - opening the text file for writing */
  if ((PtF = fopen (pnomef, "w")) == NULL) { Error = NO_FILE; return ; }

  /* escrita da dimens�o da matriz no ficheiro - writing the matrix's size */
  fprintf (PtF, "%d\t%d\n", pmat->NL, pmat->NC);

  /* escrita dos elementos da matriz no ficheiro - writing the matrix's elements */
  for (l= 0; l < pmat->NL; l++)
  {
    for (c = 0; c < pmat->NC; c++) fprintf (PtF, "%lf\t", pmat->Matrix[l][c]);
    fprintf (PtF, "\n");
  }

  Error = OK;
  fclose (PtF);  /* fecho do ficheiro - closing the text file */
}

PtMatrix MatrixCreateFile (char *pnomef)
{
  PtMatrix Mat; FILE *PtF; unsigned int NL, NC, l, c;

  /* abertura com validacao do ficheiro para leitura - opening the text file for reading */
  if ( (PtF = fopen (pnomef, "r")) == NULL)
  { Error = NO_FILE; return NULL; }

  /* leitura da dimens�o da matriz do ficheiro - reading the matrix's size from the text file */
  fscanf (PtF, "%d%d", &NL, &NC);
  if ((NL < 1) || (NC < 1)) { Error = BAD_SIZE; fclose (PtF); return NULL; }

  /* cria��o da matriz nula - creating an empty matrix */
  if ((Mat = MatrixCreate (NL, NC)) == NULL)
  { fclose (PtF); return NULL; }

  /* leitura dos elementos da matriz do ficheiro - reading the matrix's elements from the text file */
  for (l = 0; l < NL; l++)
  {
    for (c = 0; c < NC; c++) fscanf (PtF, "%lf", &Mat->Matrix[l][c]);
    fscanf (PtF, "%*[^\n]"); fscanf (PtF, "%*c");
  }

  fclose (PtF);  /* fecho do ficheiro - closing the text file */
  return Mat;  /* devolve a matriz criada - returning the new matrix */
}

/*******************************************************************************
 Fun��o auxiliar que verifica se as duas matrizes podem ser operadas (com excep��o
 da opera��o de multiplica��o), ou seja, se existem e se t�m a mesma dimens�o.
 Devolve 1 em caso afirmativo e 0 em caso contr�rio. Valores de erro: OK, NO_MATRIX
 ou DIF_SIZE.

 Auxiliary function to verify if the two matrices can be operated (with the exception
 of the multiplication), that is, if they exist and have the same size. Returns 1
 in affirmative case and 0 otherwise. Error codes: OK, NO_MATRIX or DIF_SIZE.
*******************************************************************************/
static int EqualSizeMatrices (PtMatrix pmat1, PtMatrix pmat2)
{
  /* verifica se as duas matrizes existem - verifies if the two matrices exist */
  if ((pmat1 == NULL) || (pmat2 == NULL)) { Error = NO_MATRIX; return 0; }

  /* verifica se a dimens�o das duas matrizes � igual - verifies if they have the same size */
  if ((pmat1->NL != pmat2->NL) || (pmat1->NC != pmat2->NC))
  { Error = DIF_SIZE; return 0; }

  Error = OK;
  return 1;  /* as duas matrizes existem e t�m a mesma dimens�o - they exist and have the same size */
}

/*******************************************************************************
 Fun��o auxiliar que verifica se as duas matrizes podem ser multiplicadas, ou
 seja, se existem e se s�o encadeadas (o n�mero de colunas da primeira � igual ao
 n�mero de linhas da segunda). Devolve 1 em caso afirmativo e 0 em caso contr�rio.
 Valores de erro: OK, NO_MATRIX ou NO_CHAINED.

 Auxiliary function to verify if the two matrices can be multiplied, that is, if they
 exist and are chained (the number of columns of the first is equal to the number
 of lines of the second). Returns 1 in affirmative case and 0 otherwise. Error
 codes: OK, NO_MATRIX or DIF_SIZE.
*******************************************************************************/
static int ChainedMatrices (PtMatrix pmat1, PtMatrix pmat2)
{
  /* verifica se as duas matrizes existem - verifies if the two matrices exist */
  if ((pmat1 == NULL) || (pmat2 == NULL)) { Error = NO_MATRIX; return 0; }

  /* verifica se as duas matrizes s�o encadeadas - verifies if they are chained */
  if (pmat1->NC != pmat2->NL) { Error = NO_CHAINED; return 0; }

  Error = OK;
  return 1;  /* as duas matrizes existem e s�o encadeadas - they exist and are chained */
}

/*******************************************************************************
 Fun��o auxiliar que calcula o determinante de uma matriz quadrada usando o
 algoritmo de elimina��o de Gauss. Transforma a matriz numa matriz triangular
 superior. Recebe um array unidimensional que armazena os elementos da matriz,
 linha a linha.

 Auxiliary function that calculates the determinant of a square matrix. It uses
 the Gauss elimination algorithm that transforma the matrix in a superior triangular
 matrix. It receives a one-dimension array that stores the matrix elements, line by line.
*******************************************************************************/
static double Determinant (double *pmatrix, unsigned int psize, unsigned int pn)
{
	int AuxCol; unsigned int NC, NL, LC = pn-1; double Elem;

	if (pn == 1) return *pmatrix;	/* condi��o de paragem - stop recursion condition */
	else
	{   /* procurar coluna com �ltimo elemento n�o nulo - search for column with last element not zero */
		AuxCol = LC;
 		while (AuxCol >= 0 && *(pmatrix+LC*psize+AuxCol) == 0) AuxCol--;

		if (AuxCol >= 0)	/* se existir tal coluna - if such column exists */
		{
			if (AuxCol != LC)	/* se n�o for a �ltima coluna - if it is not the last column */
				for (NL = 0; NL < pn; NL++)	/* trocar as colunas - change columns */
				{
					Elem = *(pmatrix+NL*psize+LC);
					*(pmatrix+NL*psize+LC) = *(pmatrix+NL*psize+AuxCol);
					*(pmatrix+NL*psize+AuxCol) = -Elem;
				}

			/* dividir a �ltima coluna pelo �ltimo elemento - divide the last column by the last element */
			for (NL = 0; NL < LC; NL++) *(pmatrix+NL*psize+LC) = *(pmatrix+NL*psize+LC) / *(pmatrix+LC*psize+LC);

			/* subtrair todas as colunas menos a �ltima pela �ltima coluna */
			/* multiplicada pelo �ltimo elemento da coluna a processar */
			/* process all other columns in order to eliminate the last element of the column */
			for (NC = 0; NC < LC; NC++)
				for (NL = 0; NL < LC; NL++)
					*(pmatrix+NL*psize+NC) = *(pmatrix+NL*psize+NC) - (*(pmatrix+NL*psize+LC) * *(pmatrix+LC*psize+NC));

			/* invoca��o recursiva para a matriz de dimens�o N-1 */
			/* invoca��o recursiva para a matriz de dimens�o N-1 */
			return *(pmatrix+LC*psize+LC) * Determinant (pmatrix, psize, pn-1);
		}
		else return 0.0;
	}
}

