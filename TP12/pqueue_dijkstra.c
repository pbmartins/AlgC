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

    if (!(pdim))
        return NULL;

    if ((PQueue = (PtPQueue) malloc (sizeof(struct pqueue))) == NULL)
        return NULL;

    if ((PQueue->Heap = (VERTEX *) calloc (pdim, sizeof(VERTEX))) == NULL) {
        free (PQueue);
        return NULL;
    }

    PQueue->HeapSize = pdim;
    PQueue->NumElem = 0;

    return PQueue;
}


int PQueueDestroy (PtPQueue *ppqueue)
{
    PtPQueue tmp = *ppqueue;

    if (tmp == NULL)
        return NO_PQUEUE;

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

    if (pelem == NULL)
        return NULL_PTR;

    for (i = ppqueue->NumElem; i > 0 && ppqueue->Heap[(i-1)/2].Cost > pelem->Cost;
            i = (i-1)/2) {
        ppqueue->Heap[i] = ppqueue->Heap[(i-1)/2];
    }

    ppqueue->Heap[i] = *pelem;
    ppqueue->NumElem++;
    return OK;
}


int PQueueDeleteMin (PtPQueue ppqueue, VERTEX *pelem)
{
    unsigned int i, son;

    if (ppqueue == NULL)
        return NO_PQUEUE;

    if (!ppqueue->NumElem)
        return PQUEUE_EMPTY;

    if (pelem == NULL)
        return NULL_PTR;

    *pelem = ppqueue->Heap[0];
    ppqueue->NumElem--;

    for (i = 0; 2*i+1 <= ppqueue->NumElem; i = son) {
        son = 2 * i+1;

        if (son < ppqueue->NumElem
                && ppqueue->Heap[son].Cost > ppqueue->Heap[son + 1].Cost) son++;

        if (ppqueue->Heap[son].Cost < ppqueue->Heap[ppqueue->NumElem].Cost)
            ppqueue->Heap[i] = ppqueue->Heap[son];
        else
            break;
    }

    ppqueue->Heap[i] = ppqueue->Heap[ppqueue->NumElem];
    return OK;
}


int PQueueDecrease (PtPQueue ppqueue, VERTEX *pelem)
{
    unsigned int i;

    if (ppqueue == NULL)
        return NO_PQUEUE;

    if (!ppqueue->NumElem)
        return PQUEUE_EMPTY;

    if (pelem == NULL)
        return NULL_PTR;

    for (i = 0; i <= ppqueue->NumElem; i++)
        if (ppqueue->Heap[i].Vertex == pelem->Vertex)
            break;

    if (i > ppqueue->NumElem)
        return NO_ELEM;

    for (; i > 0 && ppqueue->Heap[(i-1)/2].Cost > pelem->Cost;
            i = (i-1)/2) {
        ppqueue->Heap[i] = ppqueue->Heap[(i-1)/2];
    }

    ppqueue->Heap[i] = *pelem;
    return OK;
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

