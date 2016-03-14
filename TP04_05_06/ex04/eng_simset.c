/*******************************************************************************

 Graphical program for simulate the ADT SET


 Author : António Manuel Adrego da Rocha    Date : March 2015

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "set.h"  /* ADT Interface file */

#define MAX_SETS 14 
#define MAX_OPTIONS 11

void Menu (void);
void ReadOption (int *);
void ReadSetIndex (int *, char *);
int ActiveSet (PtSet *, int);
int NotActiveSet (PtSet *, int);
void ReadFileName (char *);
void WriteErrorMessage (char *);
void ReadElement (char *);
void ReadNextInquiry (char *, char *);
PtSet ReadSet (void);
void WriteSet (PtSet);

int main (void)
{
  PtSet SetArray[MAX_SETS];
  int Option, Index, Set1, Set2, Set3, Equals;
  char FileName[21], Character;

  for (Index = 0; Index < MAX_SETS; Index++) SetArray[Index] = NULL;
  
  do
  {
    /* cleaning the screen and presenting the menu */
    Menu ();

    /* cleaning the previous error */
    SetClearError ();

    /* presenting the active sets */
    for (Index = 0; Index < MAX_SETS; Index++)
      if (SetArray[Index] != NULL)
      {
        printf ("\e[1m\e[%d;46f", 5+Index); WriteSet (SetArray[Index]);
        printf ("\e[0m");
      }

    /* reading the user option */
    ReadOption (&Option);

    switch (Option)
    {
        case 1 :  ReadSetIndex (&Set1, "set");
                  if (ActiveSet (SetArray, Set1)) break;
                  SetArray[Set1] = ReadSet ();
                  if (SetError ()) WriteErrorMessage ("The creation");
                  break;

        case 2 :  ReadSetIndex (&Set1, "set");
                  if (ActiveSet (SetArray, Set1)) break;
                  ReadFileName (FileName);
                  SetArray[Set1] = SetCreateFile (FileName);
                  if (SetError ()) WriteErrorMessage ("The reading");
                  break;

        case 3 :  ReadSetIndex (&Set1, "set");
                  if (NotActiveSet (SetArray, Set1)) break;
                  do
                  {
                    ReadElement (&Character);
                    if (Character != '#')
                    {
                      SetClearError ();
                      SetInsertElement (SetArray[Set1], Character);
                      if (SetError ())
                      {
                        WriteErrorMessage ("The insertion");
                        if (SetError () == NO_MEM) break;
                      }
                    }
                    ReadNextInquiry (&Character, "insert");
                  } while (Character == 'y');
                  break;
          
        case 4 :  ReadSetIndex (&Set1, "set");
                  if (NotActiveSet (SetArray, Set1)) break;
                  do
                  {
                    ReadElement (&Character);
                    if (Character != '#')
                    {
                      SetClearError ();
                      SetDeleteElement (SetArray[Set1], Character);
                      if (SetError ()) WriteErrorMessage ("The deletion");
                    }
                    ReadNextInquiry (&Character, "delete"); 
                  } while (Character == 'y');
                  break;

        case 5 :  ReadSetIndex (&Set1, "first set");
                  ReadSetIndex (&Set2, "second set");
                  do
                  {
                      ReadSetIndex (&Set3, "Reunion set");
                  } while (Set3 == Set1 || Set3 == Set2);
                  if (ActiveSet (SetArray, Set3)) break;
                  SetArray[Set3] = SetReunion (SetArray[Set1], SetArray[Set2]);
                  if (SetError ()) WriteErrorMessage ("The reunion");
                  break;

        case 6 :  ReadSetIndex (&Set1, "first set");
                  ReadSetIndex (&Set2, "second set");
                  do
                  {
                      ReadSetIndex (&Set3, "Intersection set");
                  } while (Set3 == Set1 || Set3 == Set2);
                  if (ActiveSet (SetArray, Set3)) break;
                  SetArray[Set3] = SetIntersection (SetArray[Set1], SetArray[Set2]);
                  if (SetError ()) WriteErrorMessage ("The intersection");
                  break;

        case 7 :  ReadSetIndex (&Set1, "first set");
                  ReadSetIndex (&Set2, "second set");
                  do
                  {
                      ReadSetIndex (&Set3, "Symmetric Difference set");
                  } while (Set3 == Set1 || Set3 == Set2);
                  if (ActiveSet (SetArray, Set3)) break;
                  SetArray[Set3] = SetSymmetricDifference (SetArray[Set1], SetArray[Set2]);
                  if (SetError ()) WriteErrorMessage ("the symmetric difference");
                  break;

        case 8 :  ReadSetIndex (&Set1, "first set");
                  ReadSetIndex (&Set2, "second set");
                  Equals = SetEquals (SetArray[Set1], SetArray[Set2]);
                  if (SetError ()) WriteErrorMessage ("The comparation");
                  else
                  {
                    if (Equals) printf("\e[26;1f| \e[1mThe sets %d e %d are equal", Set1, Set2);
                    else printf("\e[26;1f| \e[1mThe sets %d e %d are not equal", Set1, Set2);
                    printf("\e[0m\e[27;1f| Press a key to continue ");
                    scanf ("%*[^\n]"); scanf ("%*c");
                  }
                  break;

        case 9 :  ReadSetIndex (&Set1, "origin set");
                  if (NotActiveSet (SetArray, Set1)) break;
                  do
                  {
                      ReadSetIndex (&Set2, "destination set");
                  } while (Set2 == Set1);
                  if (ActiveSet (SetArray, Set2)) break;
                  SetArray[Set2] = SetCopy (SetArray[Set1]);
                  if (SetError ()) WriteErrorMessage ("The copy");
                  break;

        case 10 :  ReadSetIndex (&Set1, "set");
                  if (NotActiveSet (SetArray, Set1)) break;
                  ReadFileName (FileName);
                  SetStoreFile (SetArray[Set1], FileName);
                  if (SetError ()) WriteErrorMessage ("The storing");
                  break;

        case 11 : ReadSetIndex (&Set1, "set");
                  if (NotActiveSet (SetArray, Set1)) break;
                  SetDestroy (&SetArray[Set1]);
                  if (SetError ()) WriteErrorMessage ("The elimination");
                  break;
    }

  } while (Option != 0);

  for (Index = 0; Index < MAX_SETS; Index++) 
    if (SetArray[Index] != NULL) SetDestroy (&SetArray[Index]);

  printf ("\e[29;1f");

  return 0;
}

void Menu (void)
{
  system ("clear");

  printf("\e[2;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[3;1f|                         Graphical Program for Simulate Operations with Sets                          |\n");
  printf("\e[4;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[5;1f|  1 - Read set (keyboard)        | Set  0 =                                                           |\n");
  printf("\e[6;1f|  2 - Read set (from file)       | Set  1 =                                                           |\n");
  printf("\e[7;1f|  3 - Insert element             | Set  2 =                                                           |\n");
  printf("\e[8;1f|  4 - Delete element             | Set  3 =                                                           |\n");
  printf("\e[9;1f|  5 - Set reunion                | Set  4 =                                                           |\n");
  printf("\e[10;1f|  6 - Set intersection           | Set  5 =                                                           |\n");
  printf("\e[11;1f|  7 - Set symmetric difference   | Set  6 =                                                           |\n");
  printf("\e[12;1f|  8 - Comparare sets             | Set  7 =                                                           |\n");
  printf("\e[13;1f|  9 - Copy a set                 | Set  8 =                                                           |\n");
  printf("\e[14;1f| 10 - Store a set                | Set  9 =                                                           |\n");
  printf("\e[15;1f| 11 - Erase a set                | Set 10 =                                                           |\n");
  printf("\e[16;1f|  0 - Terminate program          | Set 11 =                                                           |\n");
  printf("\e[17;1f|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Set 12 =                                                           |\n");
  printf("\e[18;1f| Option ->                       | Set 13 =                                                           |\n");
  printf("\e[19;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[20;1f|                                     Window for Data Acquisition                                      |\n");
  printf("\e[21;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[22;1f|                                                                                                      |\n");
  printf("\e[23;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[24;1f|                                 Window for Error Messages and Results                                |\n");
  printf("\e[25;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  printf("\e[26;1f|                                                                                                      |\n");
  printf("\e[27;1f|                                                                                                      |\n");
  printf("\e[28;1f~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void ReadOption (int *poption)
{
  do
  {  
    printf("\e[18;1f| Option ->                       |");
    printf("\e[18;13f"); scanf ("%d", poption);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*poption < 0 || *poption > MAX_OPTIONS);
}

void ReadSetIndex (int *pns, char *pmsg)
{
  int MsgLen = strlen (pmsg);

  do
  {
    *pns = -1;
    printf("\e[22;1f| Index of %s ->                               ", pmsg);
    printf("\e[22;%df", 16+MsgLen); scanf ("%d", pns);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*pns < 0 || *pns >= MAX_SETS);
}

int ActiveSet (PtSet psetarray[], int pnset)
{
  char Character;

  if (psetarray[pnset] != NULL)
  {
    do
    {
      printf("\e[1m\e[26;1f| The vector %d already exist!                     ", pnset);
      printf("\e[0m\e[27;1f| Wish to erase it (y/n)? ");
      scanf ("%c", &Character); Character = tolower (Character);
      scanf ("%*[^\n]"); scanf ("%*c");
    } while (Character != 'n' && Character != 'y');

    if (Character == 'y') { SetDestroy (&psetarray[pnset]); return 0; }
    else  return 1;
  }
  else return 0;
}

int NotActiveSet (PtSet psetarray[], int pnset)
{
  if (psetarray[pnset] == NULL)
  {
    printf("\e[1m\e[26;1f| The set %d does not exist!                        ", pnset);
    printf("\e[0m\e[27;1f| Press a key to continue ");
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
    printf("\e[22;1f| Filename ->                               ");
    printf("\e[22;15f"); Status = scanf ("%20[^\n]", pfname);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (Status == 0);
}

void WriteErrorMessage (char *pmsg)
{
  printf("\e[26;1f| %s of sets was not executed because -> \e[1m%s", pmsg, SetErrorMessage ());
  printf("\e[0m\e[27;1f| Press a key to continue ");
  scanf ("%*[^\n]"); scanf ("%*c");
}

void ReadElement (char *pc)
{
  do
  {
    printf("\e[22;1f| Element for the set [A-Z or # to stop] ->                               ");
    printf("\e[22;45f"); scanf ("%c", pc); *pc = toupper (*pc);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while ((*pc < 'A' || *pc > 'Z') && (*pc != '#'));
}

void ReadNextInquiry (char *pcar, char *pmsg)
{
  do
  {
    printf("\e[22;1f| Wish to %s more elements in the set (y/n)?           ", pmsg);
    printf("\e[22;50f"); scanf ("%c", pcar); *pcar = tolower (*pcar);
    scanf ("%*[^\n]"); scanf ("%*c");
  } while (*pcar != 'n' && *pcar != 'y');
}

PtSet ReadSet (void)
{
  PtSet Set; char Character;

  /* creating the empty set */    
  if ((Set = SetCreate ()) == NULL) return NULL;
  
  /* reading the set's elements from keyboard */
  ReadElement (&Character);
  while (Character != '#')
  {
    SetInsertElement (Set, Character);
    if (SetError ())
    {
      WriteErrorMessage ("The insertion");
      if (SetError () == NO_MEM) break;
    }
    ReadElement (&Character);
  }

  return Set;  /* returning the new set */
}

void WriteSet (PtSet pset)
{
  unsigned int Cardinal, I; 

  if (pset == NULL) return;  /* verifies if the set exists */

  Cardinal = SetCardinal (pset);

  printf ("{ ");
  /* writing on screen the set's elements */
  for (I = 1; I <= Cardinal; I++) printf ("%c ", SetObserveElement (pset, I));
  printf ("}\n");
}

