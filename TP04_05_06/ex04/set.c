/*******************************************************************************

 Ficheiro de implementa��o do Tipo de Dados Abstracto SET (set.c).
 A estrutura de dados de suporte do conjunto � uma estrutura, constitu�da pelo
 campo Cardinal para armazenar o n�mero de elementos existentes no conjunto e o
 campo Head que � a cabe�a da lista biligada dos elementos do conjunto. A lista
 de elementos do conjunto, que s�o caracteres alfab�ticos mai�sculos, est�
 ordenada por ordem crescente.

 Nome :                                                     NMec:
 
 Implementation file of the abstract data type SET (set.c). The data
 structure to store the set is composed with the integer field Cardinal for 
 keeping the number of elements in the set and the pointer field Head, that 
 represents the start point of the double-link list, for storing the set's
 elements, that are uppercase characters stored by increasing order.

*******************************************************************************/

#include <stdio.h>  
#include <stdlib.h>
#include <ctype.h>

#include "set.h"    /* Ficheiro de interface do TDA - ADT Interface file */

/************ Defini��o da Estrutura de Dados Interna do Conjunto ************/

typedef struct node *PtNode;

struct node    /* defini��o de um n� da lista biligada - node of a double-link list */
{
  char *PtElem;  /* ponteiro para o elemento - pointer to the element */
  PtNode PtNext;  /* ponteiro para o n� seguinte - pointer to the next element */
  PtNode PtPrev;  /* ponteiro para o n� anterior - pointer to the preview element */
};

struct set
{
  unsigned int Cardinal;   /* cardinal do conjunto - set's cardinal */
  PtNode Head;    /* cabe�a da lista dos elementos do conjunto - pointer to set's list of elements */
};

/*********************** Controlo Centralizado de Erro ************************/

static unsigned int Error = OK;  /* inicializa��o do erro */

static char *ErrorMessages[] = {
                                 "sem erro - Without Error",
                                 "conjunto(s) inexistente(s) - Set(s) do not exist",
                                 "memoria esgotada - Out of memory",
                                 "ficheiro inexistente - File does not exist",
                                 "elemento inexistente no conjunto - Element does not exist",
                                 "elemento existente no conjunto - Element already exists",
                                };

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** N�mero de mensagens de erro previstas no m�dulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/************************ Alus�o �s Fun��es Auxiliares ************************/

static int ValidSets (PtSet, PtSet);
static PtNode CreateElement (char);
static void DestroyElement (PtNode *);
static PtNode PosInsert (PtSet, char);
static PtNode PosDelete (PtSet, char);

/*************************** Defini��o das Fun��es ****************************/

void SetClearError (void)
{
  Error = OK;
}

int SetError (void)
{
  return Error;
}

char *SetErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;  /* n�o h� mensagem de erro - no error message */
}

PtSet SetCreate (void)
{
    PtSet newSet;
    
    /* Allocate memory to struct */
    if ((newSet = (PtSet) malloc(sizeof(struct set))) == NULL) {
        Error = NO_MEM;
        return NULL;
    }

    /* Initialize struct fields */
    newSet->Cardinal = 0;
    newSet->Head = NULL;

    Error = OK;
    return newSet;
}

void SetDestroy (PtSet *pset)
{
    PtSet tmp = *pset;
    unsigned int i;
    PtNode previousHead;

    /* Verifies if set exists */
    if (tmp == NULL) {
        Error = NO_SET;
        return;
    }
    
    /* Destroy all nodes */
    /*for (i = 0; i < tmp->Cardinal; i++)
        SetDeleteElement(tmp, *(tmp->Head)->PtElem);
    */

    for (i = 0; i < tmp->Cardinal; i++) {
        previousHead = tmp->Head;
        tmp->Head = previousHead->PtNext;
        DestroyElement(&previousHead);
    }

    /* Free the memory occupied by the struct */
    free(tmp);

    Error = OK;
}

PtSet SetCopy (PtSet pset)
{
    PtSet copy;
    unsigned int i;
    PtNode currentNode;

    /* Verifies if set exists */
    if (pset == NULL) {
        Error = NO_SET;
        return NULL;
    }

    /* Call the constructor to create a new set */
    if ((copy = SetCreate()) == NULL)
        return NULL;

    /* Copy all nodes and cardinal info */
    currentNode = pset->Head;
    for (i = 0; i < pset->Cardinal; i++) {
        if (SetInsertElement(copy, *(currentNode->PtElem)) == 0 && Error == NO_MEM) {
            SetDestroy(&copy);
            break;
        }
        currentNode = currentNode->PtNext;
    }

    Error = OK;
    return copy;
}

int SetCardinal (PtSet pset)
{
  if (pset == NULL)  /* verifica se o conjunto existe - verifies is the set exists */
  {
    Error = NO_SET; return 0;
  }

  Error = OK;
  return pset->Cardinal;  /* devolve o cardinal - returns the cardinal */
}

int SetInsertElement (PtSet pset, char pelem)
{
  PtNode NoTmp, NoIns;

  if (pset == NULL)  /* verifica se o conjunto existe - verifies if the set exists */
  {
    Error = NO_SET; return 0;
  }

  /* conjunto vazio ou elemento menor do que o primeiro elemento do conjunto */
  /* empty set or element "smaller" than the set's first element */
  if (pset->Head == NULL || pelem < *pset->Head->PtElem)
  {
    /* inser��o sem sucesso, porque n�o � poss�vel criar o elemento */
    /* insertion without success due to lack of memory to create the element */
    if ((NoTmp  = CreateElement (pelem)) == NULL) return 0;

    /* liga��o do elemento �  cabe�a da lista - linking the element in list's head */
    NoTmp->PtNext = pset->Head; pset->Head = NoTmp;
    if (NoTmp->PtNext != NULL) NoTmp->PtNext->PtPrev = NoTmp;
  }
  else
  {
    /* procurar se o elemento existe ou a sua posi��o de inser��o caso n�o exista */
    /* search for the element or its insertion position */
    if ((NoIns = PosInsert (pset, pelem)) == NULL) return 0; 
    /* inser��o sem sucesso, porque o elemento j� existe no conjunto */
    /* insertion without success because element already exists in the set */

    if ((NoTmp = CreateElement (pelem)) == NULL) return 0;
    /* inser��o sem sucesso, porque n�o � poss�vel criar o elemento */
    /* insertion without success due to lack of memory to create the element */

    /* inserir � frente de outro n� - insertion in front of another node */
    NoTmp->PtNext = NoIns->PtNext; 
    if (NoTmp->PtNext != NULL) NoTmp->PtNext->PtPrev = NoTmp;
    NoTmp->PtPrev = NoIns; NoIns->PtNext = NoTmp;
  }

  pset->Cardinal++;  /* incrementa o cardinal do conjunto - cardinal increment */
  Error = OK;
  return 1;  /* inser��o com sucesso - insertion with success */
}

int SetDeleteElement (PtSet pset, char pelem)
{
  PtNode NoRem;

  if (pset == NULL)  /* verifica se o conjunto existe - verifies if the set exists */
  {
    Error = NO_SET; return 0;
  }

  /* procurar se o elemento existe - search for the element */
  if ((NoRem = PosDelete (pset, pelem)) == NULL) return 0; 
  /* remo��o sem sucesso, porque o elemento ainda n�o existe no conjunto */
  /* deletion without success because element does not exist in the set*/

  if (NoRem == pset->Head)  /* primeiro elemento da lista - first element of the list */
  {
    if (NoRem->PtNext != NULL) NoRem->PtNext->PtPrev = NULL;
    pset->Head = NoRem->PtNext;
  }
  else  /* noutra posi��o da lista - in another position of the list */
  {
    NoRem->PtPrev->PtNext = NoRem->PtNext;
    if (NoRem->PtNext != NULL) NoRem->PtNext->PtPrev = NoRem->PtPrev;
  }

  DestroyElement (&NoRem);  /* liberta a mem�ria ocupada pelo elemento  - frees the memory */
  pset->Cardinal--;  /* decrementa o cardinal do conjunto - cardinal decrement */
  Error = OK;
  return 1;  /* remo��o com sucesso - deletion with success */
}

char SetObserveElement (PtSet pset, unsigned int ppos)
{
  PtNode NoTmp; unsigned int I;

  if (pset == NULL)  /* verifica se o conjunto existe - verifies if the set exists */
  {
    Error = NO_SET; return 0;
  }

  /* verifica se a posi��o � v�lida - verifies if the position is valid */
  if (ppos > pset->Cardinal)
  {
    Error = NO_ELEM; return 0;
  }

  /* varre a lista at� � posi��o pretendida - iterates the list until the required position */
  NoTmp = pset->Head;
  for (I = 1; I < ppos; I++) NoTmp = NoTmp->PtNext;

  if (NoTmp == NULL) return ' ';
  else return *NoTmp->PtElem;  /* devolve o elemento - returns the element */
}

int SetSearchElement (PtSet pset, char pelem)
{
  if (pset == NULL)  /* verifica se o conjunto existe - verifies if the set exists */
  {
    Error = NO_SET; return 0;
  }

  if (PosDelete (pset, pelem) != NULL) return 1;  /* o elemento existe - the element exists */

  Error = OK;
  return 0;  /* o elemento n�o existe - the element does not exist */
}

PtSet SetReunion (PtSet pset1, PtSet pset2)
{
    PtSet reunion;
    unsigned int i;
    PtNode currentNode;

    /* Verify sets */
    if (!ValidSets(pset1, pset2))
        return NULL;

    /* Calls the constructor */
    if ((reunion = SetCreate()) == NULL)
        return NULL;

    /* Copy all nodes and cardinal info */
    currentNode = pset1->Head;
    for (i = 0; i < pset1->Cardinal; i++) {
        if (SetInsertElement(reunion, *(currentNode->PtElem)) == 0) {
            if (Error == NO_MEM) {
                SetDestroy(&reunion);
                return NULL;
            }
        }
        currentNode = currentNode->PtNext;
    }

    currentNode = pset2->Head;
    for (i = 0; i < pset2->Cardinal; i++) {
        if (SetInsertElement(reunion, *(currentNode->PtElem)) == 0) {
            if (Error == NO_MEM) {
                SetDestroy(&reunion);
                return NULL;
            }
        }
        currentNode = currentNode->PtNext;
    }
    
    Error = OK;
    return reunion;
}

PtSet SetIntersection (PtSet pset1, PtSet pset2)
{
    PtSet intersection;
    unsigned int i;
    PtNode currentNode;    

    /* Verify sets */
    if (!ValidSets(pset1, pset2))
        return NULL;

    /* Calls the constructor */
    if ((intersection = SetCreate()) == NULL)
        return NULL;

    /* Verifies the existance of the elements of pset1 in pset2 */
    currentNode = pset1->Head;
    for (i = 0; i < pset1->Cardinal; i++) {
        if (SetSearchElement(pset2, *(currentNode->PtElem))) {
            if (SetInsertElement(intersection, *(currentNode->PtElem)) == 0 && Error == NO_MEM) {
                SetDestroy(&intersection);
                return NULL; 
            }
        }
        currentNode = currentNode->PtNext;
    }

    Error = OK;
    return intersection;
}

PtSet SetSymmetricDifference (PtSet pset1, PtSet pset2)
{
    PtSet diff;
    unsigned int i;
    PtNode currentNode;    

    /* Verify sets */
    if (!ValidSets(pset1, pset2))
        return NULL;

    /* Calls the constructor */
    if ((diff = SetCreate()) == NULL)
        return NULL;

    /* Verifies the existance of the elements of pset1 in pset2 (and vice-versa) and save those who aren't commun */
    currentNode = pset1->Head;
    for (i = 0; i < pset1->Cardinal; i++) {
        if (!SetSearchElement(pset2, *(currentNode->PtElem))) {
            if (SetInsertElement(diff, *(currentNode->PtElem)) == 0 && Error == NO_MEM) {
                SetDestroy(&diff);
                return NULL; 
            }
        }
        currentNode = currentNode->PtNext;
    }

    currentNode = pset2->Head;
    for (i = 0; i < pset2->Cardinal; i++) {
        if (!SetSearchElement(pset1, *(currentNode->PtElem))) {
            if (SetInsertElement(diff, *(currentNode->PtElem)) == 0 && Error == NO_MEM) {
                SetDestroy(&diff);
                return NULL; 
            }
        }
        currentNode = currentNode->PtNext;
    }

    Error = OK;
    return diff;
}

int SetEquals (PtSet pset1, PtSet pset2)
{
    unsigned int i;
    PtNode set1Node, set2Node;

    /* Verify sets */
    if (!ValidSets(pset1, pset2))
        return -1;

    /* Verify if the sets have same size */
    if (pset1->Cardinal != pset2->Cardinal)
        return 0;

    /* Verify if the elements of the sets are the same */
    set1Node = pset1->Head;
    set2Node = pset2->Head;
    for (i = 0; i < pset1->Cardinal; i++) {
        if (*(set1Node->PtElem) != *(set2Node->PtElem))
            return 0;
        set1Node = set1Node->PtNext;
        set2Node = set2Node->PtNext;
    }    

    return 1;
}

void SetStoreFile (PtSet pset, char *pnomef)
{
  FILE *PtF; PtNode NoTmp;

  /* verifica se o vector existe - verifies if set exists */
  if (pset == NULL) { Error = NO_SET; return ; }

  /* abertura com validacao do ficheiro para escrita - opening the text file for writing */
  if ((PtF = fopen (pnomef, "w")) == NULL)
  { Error = NO_FILE; return ; }

  /* escrita do cardinal do conjunto no ficheiro - - writing the set's cardinal */
  fprintf (PtF, "%u\n", pset->Cardinal);

  /* escrita dos elementos do conjunto no ficheiro - writing the set's elements */
  for (NoTmp = pset->Head; NoTmp != NULL; NoTmp = NoTmp->PtNext)
  fprintf (PtF, "%c\n", *NoTmp->PtElem);

  Error = OK;
  fclose (PtF);  /* fecho do ficheiro - closing the text file */
}

PtSet SetCreateFile (char *pnomef)
{
  PtSet Set; FILE *PtF; unsigned int Cardinal, I; char Elemento;

  /* abertura com validacao do ficheiro para leitura - opening the text file for reading */
  if ( (PtF = fopen (pnomef, "r")) == NULL)
  { Error = NO_FILE; return NULL; }

  /* leitura do cardinal do conjunto do ficheiro - reading the set's cardinal from the text file */
  fscanf (PtF, "%u", &Cardinal); fscanf (PtF, "%*c");

  /* cria��o do set vazio - creating an empty set */
  if ((Set = SetCreate ()) == NULL)
  { fclose (PtF); return NULL; }

  /* leitura dos elementos do conjunto do ficheiro - reading the set's elements from the text file */
  for (I = 0; I < Cardinal ; I++)
  {
    fscanf (PtF, "%c", &Elemento); fscanf (PtF, "%*c");
    if (!SetInsertElement (Set, Elemento))
      {
        SetDestroy (&Set); fclose (PtF); return NULL; 
      }
  }

  fclose (PtF);  /* fecho do ficheiro - closing the text file */
  Error = OK;
  return Set;  /* devolve o set criado - returning the new set */
}

/********************** Defini��o das Fun��es Auxiliares **********************/

/*******************************************************************************
 Fun��o auxiliar que verifica se os dois ponteiros s�o v�lidos. Devolve 1 em
 caso afirmativo e 0 em caso contr�rio.
 
 Auxiliary function to verify if the two sets can be operated, that is, if they
 exist. Returns 1 in affirmative case and 0 otherwise. Error codes: OK or NO_SET. 
*******************************************************************************/
static int ValidSets (PtSet pset1, PtSet pset2)
{
  /* verifica se os dois conjuntos existem - verifies if the two sets exist */
  if ((pset1 == NULL) || (pset2 == NULL)) { Error = NO_SET; return 0; }

  Error = OK;
  return 1;  /* os dois conjuntos existem - they exist */
}

/*******************************************************************************
 Fun��o auxiliar que cria o elemento do conjunto. Come�a por criar um n� da
 lista biligada e depois cria o elemento, para onde copia o valor que se
 pretende armazenar. Devolve a refer�ncia do n� criado ou NULL, caso n�o consiga
 criar o n� ou o elemento, por falta de mem�ria. Valores de erro: NO_MEM.
 
 Auxiliary function that creates the element. Starts by creating the list node
 and the element storing the character. Returns a reference to the new node or 
 NULL if there isn't enough memory. Error codes: OK or NO_MEM.
*******************************************************************************/
static PtNode CreateElement (char pelem)
{
  PtNode NoTmp;

  if ((NoTmp = (PtNode) malloc (sizeof (struct node))) == NULL)
  {
    Error = NO_MEM; return NULL;
  }

  if ((NoTmp->PtElem = malloc (sizeof (char))) == NULL)
  {
    free (NoTmp);
    Error = NO_MEM; return NULL;
  }

  *NoTmp->PtElem = pelem;  /* copia o valor - copies the element */
  NoTmp->PtNext = NULL;  /* aponta para a frente para NULL - next node is NULL */
  NoTmp->PtPrev = NULL;  /* aponta para a tr�s para NULL - preview node is NULL */

  Error = OK;
  return NoTmp;  /* devolver a refer�ncia do n� criado - returns the new node */
}

/*******************************************************************************
 Fun��o auxiliar que elimina o elemento do conjunto, indicando para esse feito
 o n� onde o elemento est� dependurado. Come�a por verificar se o n� � valido,
 ap�s o que liberta a mem�ria ocupada pelo elemento e pelo n� da lista. Como esta
 fun��o � correctamente invocada, ela n�o precisa de actualizar a vari�vel de erro.

 Auxiliary function that destroys the element freeing the memory used by the node
 and the element. This function is correctly used so does not update de error.
*******************************************************************************/
static void DestroyElement (PtNode *pnode)
{
  if (*pnode == NULL) return;
  
  free ((*pnode)->PtElem);
  free (*pnode);
  *pnode = NULL;
}

/*******************************************************************************
 Fun��o auxiliar que pesquisa um elemento no conjunto, para determinar a sua
 posi��o para eventual remo��o do conjunto. Devolve o endere�o do n� onde deve
 ser feita a remo��o, ou NULL caso o elemento ainda n�o exista no conjunto.
 
 Auxiliary function that searches the list for an element deletion. Returns 
 a pointer to the element's node or a NULL pointer if the element does not exist.
*******************************************************************************/
static PtNode PosDelete (PtSet pset, char pelem)
{
  PtNode Tmp;

  if (pset->Head == NULL) return NULL;  /* conjunto vazio - empty set */

  for (Tmp = pset->Head; Tmp != NULL; Tmp = Tmp->PtNext)
    if (*Tmp->PtElem == pelem) return Tmp;  /* elemento encontrado - element found */

  Error = NO_ELEM; 
  return NULL;  /* elemento n�o encontrado - element not found */
}

/*******************************************************************************
 Fun��o auxiliar que pesquisa um elemento no conjunto, para determinar a sua
 posi��o para eventual coloca��o no conjunto. Devolve o endere�o do n� � frente
 do qual deve ser feita a inser��o, ou NULL caso o elemento j� exista no
 conjunto. A fun��o � invocada sabendo � partida que o conjunto n�o est� vazio
 ou que o elemento n�o vai ser inserido no in�cio do conjunto.
 
 Auxiliary function that searches the list for an element insertion. Returns 
 a pointer to the node in front of which the element should be inserted if the 
 element does not exist in list or a NULL pointer if the element already exists 
 in the list. The function is invoked knowing that the list is not empty or that
 the element is not in the head of the list.
*******************************************************************************/
static PtNode PosInsert (PtSet pset, char pelem)
{
  PtNode Tmp, NodePrev;

  for (Tmp = pset->Head; Tmp != NULL; NodePrev = Tmp, Tmp = Tmp->PtNext)
    if (*Tmp->PtElem >= pelem) break;

  /* se o elemento n�o foi encontrado ent�o entra � frente do n� anterior */
  /* if the element is not found then it will be indicated the preview node */
  if (Tmp == NULL || *Tmp->PtElem > pelem) return NodePrev;
  else
  {
    Error = REP_ELEM; 
    return NULL;  /* o elemento foi encontrado - element found */
  }
}

