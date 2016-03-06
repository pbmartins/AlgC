/*******************************************************************************

 Graphical program for simulate the ADT VECTOR

 Author : António Manuel Adrego da Rocha    Date : February 2015

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "vector.h"  /* ADT Interface file */


#define MAX_VECTORS 13
#define MAX_OPTIONS 10

void Menu (void);
void ReadOption (int *);
void ReadVectorIndex (int *, char *);
int ActiveVector (PtVector *, int);
int NotActiveVector (PtVector *, int);
void ReadFileName (char *);
void WriteErrorMessage (char *);
void WriteMessageResult (char *, int, int, double);
PtVector ReadVector (void); 
void WriteVector (PtVector);

int main (void)
{
  PtVector VectorArray[MAX_VECTORS];
  int Option, Index, Vector1, Vector2, Vector3, Equals;
  double ScalarProduct;
  char FileName[21];

  for (Index = 0; Index < MAX_VECTORS; Index++) VectorArray[Index] = NULL;
  
  do
  {
    /* cleaning the screen and presenting the menu */
    Menu ();

    /* cleaning the previous error */
    VectorClearError ();

    /* presenting the active vectors */
    for (Index = 0; Index < MAX_VECTORS; Index++)
      if (VectorArray[Index] != NULL)
      {
        printf ("\e[1m\e[%d;47f", 5+Index); WriteVector (VectorArray[Index]);
        printf ("\e[0m");
      }

    /* reading the user option */
    ReadOption (&Option);

    switch (Option)
    {
        case 1 :  ReadVectorIndex (&Vector1, "vector");
                  if (ActiveVector (VectorArray, Vector1)) break;
                  VectorArray[Vector1] = ReadVector ();
                  if (VectorError ()) WriteErrorMessage ("The creation");
                  break;

        case 2 :  ReadVectorIndex (&Vector1, "vector");
                  if (ActiveVector (VectorArray, Vector1)) break;
                  ReadFileName (FileName);
                  VectorArray[Vector1] = VectorCreateFile (FileName);
                  if (VectorError ()) WriteErrorMessage ("The reading");
                  break;

        case 3 :  ReadVectorIndex (&Vector1, "first vector");
                  ReadVectorIndex (&Vector2, "second vector");
                  do
                  {
                      ReadVectorIndex (&Vector3, "sum vector");
                  } while (Vector3 == Vector1 || Vector3 == Vector2);
                  if (ActiveVector (VectorArray, Vector3)) break;
                  VectorArray[Vector3] = VectorAddition (VectorArray[Vector1], VectorArray[Vector2]);
                  if (VectorError ()) WriteErrorMessage ("The addition");
                  break;

        case 4 :  ReadVectorIndex (&Vector1, "first vector");
                  ReadVectorIndex (&Vector2, "second vector");
                  do
                  {
                      ReadVectorIndex (&Vector3, "difference vector");
                  } while (Vector3 == Vector1 || Vector3 == Vector2);
                  if (ActiveVector (VectorArray, Vector3)) break;
                  VectorArray[Vector3] = VectorSubtraction (VectorArray[Vector1], VectorArray[Vector2]);
                  if (VectorError ()) WriteErrorMessage ("The subtraction");
                  break;

        case 5 :  ReadVectorIndex (&Vector1, "first vector");
                  ReadVectorIndex (&Vector2, "second vector");
                  do
                  {
                      ReadVectorIndex (&Vector3, "product vector");
                  } while (Vector3 == Vector1 || Vector3 == Vector2);
                  if (ActiveVector (VectorArray, Vector3)) break;
                  VectorArray[Vector3] = VectorMultiplication (VectorArray[Vector1], VectorArray[Vector2]);
                  if (VectorError ()) WriteErrorMessage ("The vector product");
                  break;

        case 6 :  ReadVectorIndex (&Vector1, "first vector");
                  ReadVectorIndex (&Vector2, "second vector");
                  ScalarProduct = VectorScalarMultiplication (VectorArray[Vector1], VectorArray[Vector2]);
                  if (VectorError ()) WriteErrorMessage ("The scalar product");
                  else WriteMessageResult ("Scalar product of vectors", Vector1, Vector2, ScalarProduct);
                  break;

        case 7 :  ReadVectorIndex (&Vector1, "first vector");
                  ReadVectorIndex (&Vector2, "second vector");
                  Equals = VectorEquals (VectorArray[Vector1], VectorArray[Vector2]);
                  if (VectorError ()) WriteErrorMessage ("The comparation");
                  else
                  {
                    if (Equals) printf("\e[25;1f| \e[1mThe vectors %d and %d are equal", Vector1, Vector2);
                    else printf("\e[25;1f| \e[1mThe vectors %d and %d are not equal", Vector1, Vector2);
                    printf("\e[0m\e[26;1f| Press a key to continue ");
                    scanf ("%*[^\n]"); scanf ("%*c");
                  }
                  break;

        case 8 :  ReadVectorIndex (&Vector1, "origin vector");
                  if (NotActiveVector (VectorArray, Vector1)) break;
                  do
                  {
                      ReadVectorIndex (&Vector2, "destination vector");
                  } while (Vector2 == Vector1);
                  if (ActiveVector (VectorArray, Vector2)) break;
                  VectorArray[Vector2] = VectorCopy (VectorArray[Vector1]);
                  if (VectorError ()) WriteErrorMessage ("The copy");
                  break;

        case 9 :  ReadVectorIndex (&Vector1, "vector");
                  if (NotActiveVector (VectorArray, Vector1)) break;
                  ReadFileName (FileName);
                  VectorStoreFile (VectorArray[Vector1], FileName);
                  if (VectorError ()) WriteErrorMessage ("The storing");
                  break;
        
        case 10 : ReadVectorIndex (&Vector1, "vector");
                  if (NotActiveVector (VectorArray, Vector1)) break;
                  VectorDestroy (&VectorArray[Vector1]);
                  if (VectorError ()) WriteErrorMessage ("The elimination");
                  break;
    }

  } while (Option != 0);

  for (Index = 0; Index < MAX_VECTORS; Index++) 
    if (VectorArray[Index] != NULL) VectorDestroy (&VectorArray[Index]);

  printf ("\e[28;1f");

  return EXIT_SUCCESS;
}

void Menu (void)
{
  system ("clear");

  printf("\e[2;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[3;1f|                                  Graphical Program for Simulate Operations with Vectors                                  |\n");
  printf("\e[4;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[5;1f|  1 - Read vector (keyboard)   | Vector  0 =                                                                              |\n");
  printf("\e[6;1f|  2 - Read vector (from file)  | Vector  1 =                                                                              |\n");
  printf("\e[7;1f|  3 - Add vectors              | Vector  2 =                                                                              |\n");
  printf("\e[8;1f|  4 - Subtract vectors         | Vector  3 =                                                                              |\n");
  printf("\e[9;1f|  5 - Multiply vectors         | Vector  4 =                                                                              |\n");
  printf("\e[10;1f|  6 - Scalar vector product    | Vector  5 =                                                                              |\n");
  printf("\e[11;1f|  7 - Comparare vectors        | Vector  6 =                                                                              |\n");
  printf("\e[12;1f|  8 - Copy a vector            | Vector  7 =                                                                              |\n");
  printf("\e[13;1f|  9 - Store a vector           | Vector  8 =                                                                              |\n");
  printf("\e[14;1f| 10 - Erase a vector           | Vector  9 =                                                                              |\n");
  printf("\e[15;1f|  0 - Terminate program        | Vector 10 =                                                                              |\n");
  printf("\e[16;1f|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Vector 11 =                                                                              |\n");
  printf("\e[17;1f| Option ->                     | Vector 12 =                                                                              |\n");
  printf("\e[18;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[19;1f|                                               Window for Data Acquisition                                                |\n");
  printf("\e[20;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[21;1f|                                                                                                                          |\n");
  printf("\e[22;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[23;1f|                                           Window for Error Messages and Results                                          |\n");
  printf("\e[24;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[25;1f|                                                                                                                          |\n");
  printf("\e[26;1f|                                                                                                                          |\n");
  printf("\e[27;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void ReadOption (int *poption)
{
  do
  {  
    printf("\e[17;1f| Option ->                     |");
    printf("\e[17;13f"); scanf ("%d", poption);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*poption < 0 || *poption > MAX_OPTIONS);
}

void ReadVectorIndex (int *pnv, char *pmsg)
{
  unsigned int MsgLen = strlen (pmsg);

  do
  {
    *pnv = -1;
    printf("\e[21;1f| Index of %s ->                               ", pmsg);
    printf("\e[21;%df", 16+MsgLen); scanf ("%d", pnv);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*pnv < 0 || *pnv >= MAX_VECTORS);
}

int ActiveVector (PtVector pvecarray[], int pnvec)
{
  char Character;

  if (pvecarray[pnvec] != NULL)
  {
    do
    {
      printf("\e[1m\e[25;1f| The vector %d already exist!                     ", pnvec);
      printf("\e[0m\e[26;1f| Wish to erase it (y/n)? ");
      scanf ("%c", &Character); Character = tolower (Character);
      scanf ("%*[^\n]"); scanf ("%*c");
    } while (Character != 'n' && Character != 'y');

    if (Character == 'y') { VectorDestroy (&pvecarray[pnvec]); return 0; }
    else  return 1;
  }
  else return 0;
}

int NotActiveVector (PtVector pvecarray[], int pnvec)
{
  if (pvecarray[pnvec] == NULL)
  {
    printf("\e[1m\e[25;1f| The vector %d does not exist!                     ", pnvec);
    printf("\e[0m\e[26;1f| Press a key to continue ");
    scanf ("%*[^\n]"); scanf ("%*c");
    return 1;
  }
  else return 0;
}

void ReadFileName (char *pfname)
{
  int Status;

  do
  {
    printf("\e[21;1f| Filename ->                               ");
    printf("\e[21;15f"); Status = scanf ("%20[^\n]", pfname);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (Status == 0);
}

void WriteErrorMessage (char *pmsg)
{
  printf("\e[25;1f| %s of vectors was not executed because -> \e[1m%s", pmsg, VectorErrorMessage ());
  printf("\e[0m\e[26;1f| Press a key to continue ");
  scanf ("%*[^\n]"); scanf ("%*c");
}

void WriteMessageResult (char *pmsg, int pv1, int pv2, double pval)
{
  printf("\e[1m\e[25;1f| %s %d and %d = \e[1m%f", pmsg, pv1, pv2, pval);
  printf("\e[0m\e[26;1f| Press a key to continue ");
  scanf ("%*[^\n]"); scanf ("%*c");
}

PtVector ReadVector (void)
{
  PtVector Vector; unsigned int I, Dim; double Component;

  do
  {
    printf("\e[21;1f| Size of vector ->                               ");
    printf("\e[21;21f"); scanf ("%d", &Dim);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (Dim < 1 || Dim > 15);
    
  /* creating the empty vector */
  if ((Vector = VectorCreate (Dim)) == NULL) return NULL;

  /* reading the vector's components from keyboard */
  for (I = 0; I < Dim; I++)
  {
    printf("\e[21;1f| Vector(%d) ->                               ", I);
    printf("\e[21;16f"); scanf ("%lf", &Component);
    scanf ("%*[^\n]"); scanf ("%*c");
    VectorModifyComponent (Vector, I, Component);
  }

  return Vector;  /* returning the new vector */
}

void WriteVector (PtVector pv)
{
  int I, Dim = VectorSize (pv);

  if (pv != NULL && Dim != 0)
  { /* writing on screen the vector's elements */
    printf ("( ");
    for (I = 0; I < Dim-1; I++) printf ("%0.1f , ", VectorObserveComponent (pv, I));
    printf ("%0.1f )\n", VectorObserveComponent (pv, I));
  }
}
