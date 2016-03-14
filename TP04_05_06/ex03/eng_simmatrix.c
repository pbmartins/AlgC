/*******************************************************************************

 Graphical program for simulate the ADT MATRIX

 Author : António Manuel Adrego da Rocha    Date : March 2016
 
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "matrix.h"  /* ADT Interface file */

#define MAX_MATRICES 19
#define MAX_OPTIONS 16

void Menu (void);
void ReadOption (int *);
void ReadMatrixIndex (int *, char *);
int ActiveMatrix (PtMatrix *, int);
int NotActiveMatrix (PtMatrix *, int);
void ReadFilename (char *);
void ReadMatrixSize (unsigned int *, unsigned int *);
void WriteErrorMessage (char *);
void WriteMessageResult (char *, double);
PtMatrix ReadMatrix (int, int);
void WriteMatrix (PtMatrix *, int);
void ChangeMatrix (PtMatrix);
double ReadValue (void);
unsigned int ReadRow (char *, unsigned int);
unsigned int ReadColumn (char *, unsigned int);

int main (void)
{
  PtMatrix MatrixArray[MAX_MATRICES];
  int Option, l, Matrix1, Matrix2, Matrix3, Equals, Sym;
  unsigned int NL, NC, L, C;
  char FileName[21]; double Value;

  for (l = 0; l < MAX_MATRICES; l++) MatrixArray[l] = NULL;
  
  do
  {

    /* cleaning the screen and presenting the menu */
    Menu ();

    /* cleaning the previous error */
    MatrixClearError ();

    /* presenting the active matrices */
    for (l = 0; l < MAX_MATRICES; l++)
      if (MatrixArray[l] != NULL)
      {
        MatrixSize (MatrixArray[l], &NL, &NC);
        printf ("\e[1m\e[%d;47f Matriz %d x %d", 5+l, NL, NC);
        printf ("\e[0m");
      }

    /* reading the user option */
    ReadOption (&Option);

    switch (Option)
    {
        case 1 :  ReadMatrixIndex (&Matrix1, "matrix");
                  if (ActiveMatrix (MatrixArray, Matrix1)) break;
                  ReadMatrixSize (&NL, &NC);
                  MatrixArray[Matrix1] = ReadMatrix (NL, NC);
                  if (MatrixError ()) WriteErrorMessage ("The creation");
                  break;

        case 2 :  ReadMatrixIndex (&Matrix1, "matrix");
                  if (ActiveMatrix (MatrixArray, Matrix1)) break;
                  ReadFilename (FileName);
                  MatrixArray[Matrix1] = MatrixCreateFile (FileName);
                  if (MatrixError ()) WriteErrorMessage ("The reading");
                  break;

        case 3 :  ReadMatrixIndex (&Matrix1, "origin matrix");
                  do
                  {
                      ReadMatrixIndex (&Matrix2, "destination matrix");
                  } while (Matrix2 == Matrix1);
                  if (ActiveMatrix (MatrixArray, Matrix2)) break;
                  MatrixArray[Matrix2] = MatrixTranspose (MatrixArray[Matrix1]);
                  if (MatrixError ()) WriteErrorMessage ("The transposition");
                  break;

        case 4 :  ReadMatrixIndex (&Matrix1, "first matrix");
                  ReadMatrixIndex (&Matrix2, "second matrix");
                  do
                  {
                      ReadMatrixIndex (&Matrix3, "sum matrix");
                  } while (Matrix3 == Matrix1 || Matrix3 == Matrix2);
                  if (ActiveMatrix (MatrixArray, Matrix3)) break;
                  MatrixArray[Matrix3] = MatrixAddition (MatrixArray[Matrix1], MatrixArray[Matrix2]);
                  if (MatrixError ()) WriteErrorMessage ("The addition");
                  break;

        case 5 :  ReadMatrixIndex (&Matrix1, "first matrix");
                  ReadMatrixIndex (&Matrix2, "second matrix");
                  do
                  {
                      ReadMatrixIndex (&Matrix3, "difference matrix");
                  } while (Matrix3 == Matrix1 || Matrix3 == Matrix2);
                  if (ActiveMatrix (MatrixArray, Matrix3)) break;
                  MatrixArray[Matrix3] = MatrixSubtraction (MatrixArray[Matrix1], MatrixArray[Matrix2]);
                  if (MatrixError ()) WriteErrorMessage ("The subtraction");
                  break;

        case 6 :  ReadMatrixIndex (&Matrix1, "first matrix");
                  ReadMatrixIndex (&Matrix2, "second matrix");
                  do
                  {
                      ReadMatrixIndex (&Matrix3, "product matrix");
                  } while (Matrix3 == Matrix1 || Matrix3 == Matrix2);
                  if (ActiveMatrix (MatrixArray, Matrix3)) break;
                  MatrixArray[Matrix3] = MatrixMultiplication (MatrixArray[Matrix1], MatrixArray[Matrix2]);
                  if (MatrixError ()) WriteErrorMessage ("The multiplication");
                  break;

        case 7 :  ReadMatrixIndex (&Matrix1, "matrix");
                  Value = MatrixDeterminant (MatrixArray[Matrix1]);
                  if (MatrixError ()) WriteErrorMessage ("The determinant calculation");
                  else WriteMessageResult ("The determinant is equal to", Value);
                  break;

        case 8 :  ReadMatrixIndex (&Matrix1, "first matrix");
                  ReadMatrixIndex (&Matrix2, "second matrix");
                  Equals = MatrixEquals (MatrixArray[Matrix1], MatrixArray[Matrix2]);
                  if (MatrixError ()) WriteErrorMessage ("The comparation");
                  else
                  {
                    if (Equals) printf("\e[31;1f| \e[1mThe matrices %d e %d are equal", Matrix1, Matrix2);
                    else printf("\e[31;1f| \e[1mThe matrices %d e %d are not equal", Matrix1, Matrix2);
                    printf("\e[0m\e[32;1f| Press a key to continue ");
                    scanf ("%*[^\n]"); scanf ("%*c");
                  }
                  break;

        case 9 :  ReadMatrixIndex (&Matrix1, "origin matrix");
                  if (NotActiveMatrix (MatrixArray, Matrix1)) break;
                  do
                  {
                      ReadMatrixIndex (&Matrix2, "destination matrix");
                  } while (Matrix2 == Matrix1);
                  if (ActiveMatrix (MatrixArray, Matrix2)) break;
                  MatrixArray[Matrix2] = MatrixCopy (MatrixArray[Matrix1]);
                  if (MatrixError ()) WriteErrorMessage ("The copy");
                  break;

        case 10 : ReadMatrixIndex (&Matrix1, "matrix");
                  if (NotActiveMatrix (MatrixArray, Matrix1)) break;
                  ReadFilename (FileName);
                  MatrixStoreFile (MatrixArray[Matrix1], FileName);
                  if (MatrixError ()) WriteErrorMessage ("The storing");
                  break;

        case 11 : ReadMatrixIndex (&Matrix1, "matrix");
                  if (NotActiveMatrix (MatrixArray, Matrix1)) break;
                  ChangeMatrix (MatrixArray[Matrix1]);
                  break;

        case 12 : ReadMatrixIndex (&Matrix1, "matrix");
                  if (NotActiveMatrix (MatrixArray, Matrix1)) break;
                  WriteMatrix (MatrixArray, Matrix1);
                  printf("\e[1m\e[31;1f| Matrix %d                     ", Matrix1);
                  printf("\e[0m\e[32;1f| Press a key to continue ");
                  scanf ("%*[^\n]"); scanf ("%*c");
                  break;

        case 13 : ReadMatrixIndex (&Matrix1, "matrix");
                  if (NotActiveMatrix (MatrixArray, Matrix1)) break;
                  MatrixDestroy (&MatrixArray[Matrix1]);
                  if (MatrixError ()) WriteErrorMessage ("The elimination");
                  break;

        case 14 : ReadMatrixIndex (&Matrix1, "origin matrix");
                  if (NotActiveMatrix (MatrixArray, Matrix1)) break;
                  do
                  {
                      ReadMatrixIndex (&Matrix2, "destination matrix");
                  } while (Matrix2 == Matrix1);
                  if (ActiveMatrix (MatrixArray, Matrix2)) break;
                  Value = ReadValue ();
                  MatrixArray[Matrix2] = MatrixMultByScalar (MatrixArray[Matrix1], Value);
                  if (MatrixError ()) WriteErrorMessage ("The multiplication by a scalar");
                  break;

        case 15 : ReadMatrixIndex (&Matrix1, "matrix");
                  if (NotActiveMatrix (MatrixArray, Matrix1)) break;
                  Sym = MatrixIsSymmetrical (MatrixArray[Matrix1]);
                  if (MatrixError ()) WriteErrorMessage ("The verification of symmetrical");
                  else
                  {
                    if (Sym) printf("\e[31;1f| \e[1mThe matrix %d is symmetrical", Matrix1);
                    else printf("\e[31;1f| \e[1mThe matrix %d is not symmetrical", Matrix1);
                    printf("\e[0m\e[32;1f| Press a key to continue ");
                    scanf ("%*[^\n]"); scanf ("%*c");
                  }                 
                  break;

        case 16 : ReadMatrixIndex (&Matrix1, "origin matrix");
                  MatrixSize (MatrixArray[Matrix1], &NL, &NC);
                  do
                  {
                      ReadMatrixIndex (&Matrix2, "destination matrix");
                  } while (Matrix2 == Matrix1);
                  if (ActiveMatrix (MatrixArray, Matrix2)) break;
		          L = ReadRow ("to eliminate", NL);
		          C = ReadColumn ("to eliminate", NC);
                  MatrixArray[Matrix2] = MatrixSubMatrix (MatrixArray[Matrix1], L, C);
                  if (MatrixError ()) WriteErrorMessage ("The submatrix creation");
                  break;
    }

  } while (Option != 0);

  for (l = 0; l < MAX_MATRICES; l++) 
    if (MatrixArray[l] != NULL) MatrixDestroy (&MatrixArray[l]);

  printf ("\e[34;1f");

  return 0;
}

void Menu (void)
{
  system ("clear");

  printf("\e[2;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[3;1f|                                  Graphical Program for Simulate Operations with Matrices                                 |\n");
  printf("\e[4;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[5;1f|  1 - Read matrix (keyboard)    | Matrix  0 =                |                                                            |\n");
  printf("\e[6;1f|  2 - Read matrix (from file)   | Matrix  1 =                |                                                            |\n");
  printf("\e[7;1f|  3 - Transpose matrix          | Matrix  2 =                |                                                            |\n");
  printf("\e[8;1f|  4 - Add matrices              | Matrix  3 =                |                                                            |\n");
  printf("\e[9;1f|  5 - Subtract matrices         | Matrix  4 =                |                                                            |\n");
  printf("\e[10;1f|  6 - Multiply matrices         | Matrix  5 =                |                                                            |\n");
  printf("\e[11;1f|  7 - Determinant calculation   | Matrix  6 =                |                                                            |\n");
  printf("\e[12;1f|  8 - Compare matrices          | Matrix  7 =                |                                                            |\n");
  printf("\e[13;1f|  9 - Copy a matrix             | Matrix  8 =                |                                                            |\n");
  printf("\e[14;1f| 10 - Store a matrix            | Matrix  9 =                |                                                            |\n");
  printf("\e[15;1f| 11 - Change a matrix           | Matrix 10 =                |                                                            |\n");
  printf("\e[16;1f| 12 - Display a matrix          | Matrix 11 =                |                                                            |\n");
  printf("\e[17;1f| 13 - Erase a matrix            | Matrix 12 =                |                                                            |\n");
  printf("\e[18;1f| 14 - Multiply by scalar        | Matrix 13 =                |                                                            |\n");
  printf("\e[19;1f| 15 - Symmetrical verification  | Matrix 14 =                |                                                            |\n");
  printf("\e[20;1f| 16 - Submatrix creation        | Matrix 15 =                |                                                            |\n");
  printf("\e[21;1f|  0 - Terminate program         | Matrix 16 =                |                                                            |\n");
  printf("\e[22;1f|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Matrix 17 =                |                                                            |\n");
  printf("\e[23;1f| Option ->                      | Matrix 18 =                |                                                            |\n");
  printf("\e[24;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[25;1f|                                               Window for Data Acquisition                                                |\n");
  printf("\e[26;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[27;1f|                                                                                                                          |\n");
  printf("\e[28;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[29;1f|                                           Window for Error Messages and Results                                          |\n");
  printf("\e[30;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[31;1f|                                                                                                                          |\n");
  printf("\e[32;1f|                                                                                                                          |\n");
  printf("\e[33;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void ReadOption (int *poption)
{
  do
  {  
    printf("\e[23;1f| Option ->                      |");
    printf("\e[23;13f"); scanf ("%d", poption);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*poption < 0 || *poption > MAX_OPTIONS);
}

void ReadMatrixIndex (int *pnm, char *pmsg)
{
  int MsgLen = strlen (pmsg);

  do
  {
    *pnm = -1;
    printf("\e[27;1f| Index of %s ->                               ", pmsg);
    printf("\e[27;%df", 16+MsgLen); scanf ("%d", pnm);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*pnm < 0 || *pnm >= MAX_MATRICES);
}

int ActiveMatrix (PtMatrix pmatarray[], int pnmat)
{
  char Character;

  if (pmatarray[pnmat] != NULL)
  {
    do
    {
      printf("\e[1m\e[31;1f| The matrix %d already exist!                     ", pnmat);
      printf("\e[0m\e[32;1f| Wish to erase it (y/n)? ");
      scanf ("%c", &Character); Character = tolower (Character);
      scanf ("%*[^\n]"); scanf ("%*c");
    } while (Character != 'n' && Character != 'y');

    if (Character == 'y') { MatrixDestroy (&pmatarray[pnmat]); return 0; }
    else  return 1;
  }
  else return 0;
}

int NotActiveMatrix (PtMatrix pmatarray[], int pnmat)
{
  if (pmatarray[pnmat] == NULL)
  {
    printf("\e[1m\e[31;1f| The matrix %d does not exist!                     ", pnmat);
    printf("\e[0m\e[32;1f| Press a key to continue ");
    scanf ("%*[^\n]"); scanf ("%*c");
    return 1;
  }
  else return 0;
}

void ReadMatrixSize (unsigned int *pnl, unsigned int *pnc)
{
  do
  {
    *pnl = 0;
    printf("\e[27;1f| Matrix number of lines ->                               ");
    printf("\e[27;29f"); scanf ("%d", pnl);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*pnl < 1 || *pnl > 10);

  do
  {
    *pnc = 0;
    printf("\e[27;1f| Matrix number of columns ->                             ");
    printf("\e[27;31f"); scanf ("%d", pnc);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*pnc < 1 || *pnc > 10);
}

void ReadFilename (char *pfname)
{
  int Status;

  do
  {
    printf("\e[27;1f| Filename ->                               ");
    printf("\e[27;15f"); Status = scanf ("%20[^\n]", pfname);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (Status == 0);
}

void WriteErrorMessage (char *pmsg)
{
  printf("\e[31;1f| %s of matrices was not executed because -> \e[1m%s", pmsg, MatrixErrorMessage ());
  printf("\e[0m\e[32;1f| Press a key to continue ");
  scanf ("%*[^\n]"); scanf ("%*c");
}

void WriteMessageResult (char *pmsg, double pval)
{
  printf("\e[1m\e[31;1f| %s \e[1m%f", pmsg, pval);
  printf("\e[0m\e[32;1f| Press a key to continue ");
  scanf ("%*[^\n]"); scanf ("%*c");
}

PtMatrix ReadMatrix (int pnl, int pnc)
{
  PtMatrix Mat; unsigned int L, C; int Status; double Element;
    
  /* creating the empty matrix */
  if ((Mat = MatrixCreate (pnl, pnc)) == NULL) return NULL;

  /* reading the matrix's elements from keyboard */
  for (L = 1; L <= pnl; L++)
  {
    for (C = 1; C <= pnc; C++)
    {
      do
      {
        printf("\e[27;1f| Matrix[%d,%d]?                                          ", L, C);
        printf("\e[27;16f"); Status = scanf ("%lf", &Element);
        scanf ("%*[^\n]"); scanf ("%*c");
      } while (Status == 0);
      MatrixModifyElement (Mat, L, C, Element);
    }
  }

  return Mat;  /* returning the new matrix */
}

void WriteMatrix (PtMatrix pmatarray[], int pnm)
{
  unsigned int L, C, NL, NC;

  MatrixSize (pmatarray[pnm], &NL, &NC);

  printf("\e[1m");
  for (L = 1; L <= NL; L++)
  { /* writing on screen the matrix's components */
    printf("\e[%d;65f ", 6+L);
    for (C = 1; C <= NC; C++)
     printf ("%6.1f ", MatrixObserveElement (pmatarray[pnm], L, C));
  }
  printf("\e[0m");
}

void ChangeMatrix (PtMatrix pmat)
{
  unsigned int L, C, NL, NC; char Character; int Status; double Element;

  MatrixSize (pmat, &NL, &NC);

  do
  {
    do
    {
      L = -1;
      printf("\e[27;1f| Line number of the matrix's element ->                    ");
      printf("\e[27;42f"); scanf ("%d", &L);
      scanf ("%*[^\n]"); scanf ("%*c");
    } while (L < 1 || L > NL);
        
    do
    {
      C = -1;
      printf("\e[27;1f| Column number of the matrix's element ->                    ");
      printf("\e[27;44f"); scanf ("%d", &C);
      scanf ("%*[^\n]"); scanf ("%*c");
    } while (C < 1 || C > NC);
        
    do
    {
      printf("\e[27;1f| Matrix[%d,%d]?                                          ", L, C);
      printf("\e[27;16f"); Status = scanf ("%lf", &Element);
      scanf ("%*[^\n]"); scanf ("%*c");
    } while (Status == 0);

    MatrixModifyElement (pmat, L, C, Element);

    do
    {
      printf("\e[27;1f| Do you wish to change more matrix's elements (y/n)?     ");
      printf("\e[27;55f"); scanf ("%c", &Character); Character = tolower (Character);
      scanf ("%*[^\n]"); scanf ("%*c");
    } while (Character != 'n' && Character != 's');
  } while (Character == 's');
}

double ReadValue (void)
{
  int Status; double Value;
  
  do
  {
	printf("\e[27;1f| Scalar value to multiply ->                         ");
	printf("\e[27;31f"); Status = scanf ("%lf", &Value);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (Status == 0);

  return Value;
}

unsigned int ReadRow (char *pmsg, unsigned int psize)
{
  unsigned int Row = 0;

  do
  {
    printf("\e[27;1f| Line number (%s) of matrix ->                    ", pmsg);
    printf("\e[27;43f"); scanf ("%d", &Row);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (Row < 1 || Row > psize);
  
  return Row;
}

unsigned int ReadColumn (char *pmsg, unsigned int psize)
{
  unsigned int Col = 0;

  do
  {
    printf("\e[27;1f| Column number (%s) of matrix ->                  ", pmsg);
    printf("\e[27;45f"); scanf ("%d", &Col);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (Col < 1 || Col > psize);
  
  return Col;
}
