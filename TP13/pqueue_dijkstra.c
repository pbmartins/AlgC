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

    if(pdim == 0)
        return NULL;

    /* memory alocation for the priority queue */
    if((PQueue = (PtPQueue) malloc(sizeof(struct pqueue))) == NULL)
        return NULL;

    /* memory alocation for the element's array */
    if((PQueue->Heap = (VERTEX*) calloc(pdim, sizeof(VERTEX))) == NULL) {
        free(PQueue);
        return NULL;
    }

    /* store the priority queue dimension */
    PQueue->HeapSize = pdim;  
    /* initialize the number of elements */
    PQueue->NumElem = 0;  

    /* returning the new priority queue */
    return PQueue;  
}


int PQueueDestroy (PtPQueue *ppqueue)
{
    PtPQueue TmpPQueue = *ppqueue;

    if(TmpPQueue == NULL)
        return NO_PQUEUE;

    /* free the element's array */
    free(TmpPQueue->Heap);  
    /* free the priority queue */
    free(TmpPQueue);  

    /* priority queue pointing to null */
    *ppqueue = NULL;  

    return OK;
}


int PQueueInsert (PtPQueue ppqueue, VERTEX *pelem)
{
    unsigned int i;

    if(ppqueue == NULL)
        return NO_PQUEUE;
    if(ppqueue->NumElem == ppqueue->HeapSize)
        return PQUEUE_FULL;
    if(pelem == NULL)
        return NULL_PTR;

    for(i = ppqueue->NumElem; i > 0 && ppqueue->Heap[(i-1) / 2].Cost > pelem->Cost; i = (i-1) / 2)
        ppqueue->Heap[i] = ppqueue->Heap[(i-1) / 2];

    ppqueue->Heap[i] = *pelem;  /* store the new element in the heap */
    ppqueue->NumElem++;  /* increase the number of elements of the heap */

    return OK;
}


int PQueueDeleteMin (PtPQueue ppqueue, VERTEX *pelem)
{
    unsigned int i, Son;  /* position of the father and the son */

    if(ppqueue == NULL)
        return NO_PQUEUE;
    if(ppqueue->NumElem == 0)
        return PQUEUE_EMPTY;
    if(pelem == NULL)
        return NULL_PTR;

    *pelem = ppqueue->Heap[0];  /* copy the minimum element from the heap */
    ppqueue->NumElem--;  /* decrease the number of elements of the heap */

    /* adjust the heap, ascending the sons smaller than the element */
    for(i = 0; 2*i+1 <= ppqueue->NumElem; i = Son) {
        Son = 2*i+1;  /* position of the first son */

        /* evaluate the smallest son */
        if(Son < ppqueue->NumElem && ppqueue->Heap[Son].Cost > ppqueue->Heap[Son+1].Cost)
            Son++;

        /* ascend the smallest son if smallest than the last element */
        if(ppqueue->Heap[Son].Cost < ppqueue->Heap[ppqueue->NumElem].Cost)
            ppqueue->Heap[i] = ppqueue->Heap[Son];
        else break;
    }

    /* store the last element in the empty position */
    ppqueue->Heap[i] = ppqueue->Heap[ppqueue->NumElem];

    return OK;
}


int PQueueDecrease (PtPQueue ppqueue, VERTEX *pelem)
{
    unsigned int i;

    if(ppqueue == NULL)
        return NO_PQUEUE;
    if(ppqueue->NumElem == 0)
        return PQUEUE_EMPTY;
    if(pelem == NULL)
        return NULL_PTR;

    /* look for the element */  
    for(i = 0; i < ppqueue->NumElem; i++)
	    if(ppqueue->Heap[i].Vertex == pelem->Vertex)
            break;

    if(i == ppqueue->NumElem)
        return NO_ELEM;

    for(; i > 0 && ppqueue->Heap[(i-1) / 2].Cost > pelem->Cost; i = (i-1) / 2)
        ppqueue->Heap[i] = ppqueue->Heap[(i-1) / 2];

    ppqueue->Heap[i] = *pelem;  /* store the new value */

    return OK;
}


int PQueueIsEmpty (PtPQueue ppqueue)
{
    if(ppqueue == NULL)
        return NO_PQUEUE;
    if(ppqueue->NumElem == 0)
        return PQUEUE_EMPTY;
    return OK;  
}

int PQueueDisplay (PtPQueue ppqueue)
{
    int I;

    if(ppqueue == NULL)
        return NO_PQUEUE;
    if(ppqueue->NumElem == 0)
        return PQUEUE_EMPTY;

    for (I = 0; I < ppqueue->NumElem; I++)
        printf ("(%d-%d)  ", ppqueue->Heap[I].Vertex, ppqueue->Heap[I].Cost);
    printf ("\n");

    return OK;
}
