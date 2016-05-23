/******* Implementação da Fila com Prioridade com um Amontoado Binário ********/
/***** Fila com Prioridade orientada aos mínimos Nome: pqueue_dijkstra.c  *****/

#include <stdio.h>
#include <stdlib.h>

#include "pqueue_dijkstra.h"  /* Interface */

/******* Definição do Estrutura de Dados da Fila com Prioridade ********/

struct pqueue  /* definição da Fila com Prioridade */
{
  unsigned int HeapSize;  /* capacidade de armazenamento da Fila - capacity of the priority queue */
  unsigned int NumElem;  /* número de elementos armazenados na Fila - number of elements stored in the priority queue */
  VERTEX *Heap;  /* ponteiro para o monte a alocar dinamicamente - pointer to the priority queue array */
};

/********************** Definição dos Subprogramas *********************/

PtPQueue PQueueCreate (unsigned int pdim)
{
    PtPQueue PQueue;
    unsigned int i, j;

    if (!(pdim))
        return NULL;

    if ((PQueue = (PtQueue) malloc (sizeof(struct pqueue))) == NULL)
        return NULL;

    if ((PQueue->Heap = (VERTEX *) calloc (pdim, sizeof(VERTEX))) == NULL) {
        free (PQueue);
        return NULL;
    }

    for (i = 0; i < pdim; i++) {
        if ((PQueue->Heap[i] = (VERTEX) malloc (sizeof(struct VERTEX))) == NULL) {
            for (j = 0; j < i; j++)
                free(PQueue->Heap[j]);
            free(PQueue->Heap);
            free(PQueue);
        }

        PQueue->Heap[i]->Vertex = 0;
        PQueue->Heap[i]->Cost = 0;
    }

    PQueue->HeapSize = pdim;
    PQueue->NumElem = 0;

    return PQueue;
}


int PQueueDestroy (PtPQueue *ppqueue)
{
    PtPQueue tmp = *ppqueue;
    unsigned int i;

    if (tmp == NULL)
        return NO_PQUEUE;

    for (i = 0; i < tmp->HeapSize; i++)
        free(tmp->Heap[i]);

    free(tmp->Heap);
    free(tmp);

    *ppqueue = NULL;

    return OK;
}


int PQueueInsert (PtPQueue ppqueue, VERTEX *pelem)
{
    unsigned int i;

    if (ppqueue == NULL)
        return NO_PQUEUE;

    if (ppqueue->NumElem == ppqueue->HeapSize)
        return PQUEUE_FULL;

    for (i = ppqueue->NumElem; i > 0 && ppqueue->Heap[(i-1)/2]->Vertex < pelem; i = (i-1)/2) {

    }

}


int PQueueDeleteMin (PtPQueue ppqueue, VERTEX *pelem)
{
}


int PQueueDecrease (PtPQueue ppqueue, VERTEX *pelem)
{
}


int PQueueIsEmpty (PtPQueue ppqueue)
{
  if (ppqueue == NULL) return NO_PQUEUE;
  if (ppqueue->NumElem == 0) return PQUEUE_EMPTY;
  return OK;  
}

int PQueueDisplay (PtPQueue ppqueue)
{
  int I;

  if (ppqueue == NULL) return NO_PQUEUE;
  if (ppqueue->NumElem == 0) return PQUEUE_EMPTY;

  for (I = 0; I < ppqueue->NumElem; I++)
    printf ("(%d-%d)  ", ppqueue->Heap[I].Vertex, ppqueue->Heap[I].Cost);
  printf ("\n");

  return OK;
}

