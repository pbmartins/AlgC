/************ Implementa��o do D�grafo Din�mico - digraph.c ************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "digraph.h"	/* interface do d�grafo */
#include "pqueue_dijkstra.h"	/* interface da fila com prioridade de Dijkstra */

/************** Defini��o do Estrutura de Dados do D�grafo *************/

typedef struct binode *PtBiNode;
typedef struct vertex *PtVertex;
typedef struct edge *PtEdge;

struct binode	/* defini��o de um bin� gen�rico - definition of the generic double linked list node */
{
	unsigned int Number;	/* n�mero do v�rtice ou da aresta - number od the vertes or edge */
	PtBiNode PtPrev;	/* ponteiro para o n� anterior da lista - pointer to preview list node */
	PtBiNode PtNext;	/* ponteiro para o n� seguinte da lista - pointer to next list node */
	PtBiNode PtAdj;	/* ponteiro para a lista de adjac�ncias - pointer to the adjacency list of edges */
	void *PtElem;	/* ponteiro para o elemento da lista - pointer to the element (vertex or edge) of the list */
	unsigned int Visit;	/* marca��o de v�rtice visitado - field for marquing the visited vertex */
};

struct vertex	/* defini��o de um v�rtice - definition of the vertex */
{
	unsigned int InDeg;	/* semigrau incidente do v�rtice - in degree */
	unsigned int OutDeg;	/* semigrau emergente do v�rtice - out degree */
};

struct edge	/* defini��o de uma aresta - definition of the edge */
{
	int Cost;	/* custo da aresta - edge cost */
};

struct digraph	/* defini��o do d�grafo - definition of the digraph/graph */
{
	PtBiNode Head;	/* ponteiro para a cabe�a do d�grafo - pointer to digraph/graph head of the vertexes list */
	unsigned int NVertexes;	/* n�mero de v�rtices do d�grafo - number of vertexes */
	unsigned int NEdges;	/* n�mero de arestas do d�grafo - number of edges */
	unsigned int Type;	/* tipo d�grafo (1) / grafo (0) - type of digraph (1) / graph (0) */
};

/***************** Prot�tipos dos Subprogramas Internos ****************/

static PtVertex CreateVertex (void);
static PtEdge CreateEdge (int);
static PtBiNode CreateBiNode (unsigned int);
static void DestroyBiNode (PtBiNode *);
static PtBiNode InPosition (PtBiNode, unsigned int);
static PtBiNode OutPosition (PtBiNode, unsigned int);
static int InsertEdge (PtBiNode, PtBiNode, int);
static void DeleteEdge (PtBiNode, PtBiNode);
static int DijkstraPQueue (PtDigraph pdig, unsigned int pv, unsigned int pvpred[], int pcost[]);

/********************** Defini��o dos Subprogramas *********************/

PtDigraph Create (unsigned int ptype)
{
	PtDigraph Digraph;

	if ((Digraph = (PtDigraph) malloc (sizeof (struct digraph))) == NULL)
		return NULL;	/* alocar mem�ria para o d�grafo */
	Digraph->Head = NULL;	/* inicializa a cabe�a do d�grafo */
	Digraph->NVertexes = 0;	/* inicializa o n�mero de v�rtices */
	Digraph->NEdges = 0;	/* inicializa o n�mero de arestas */
	Digraph->Type = ptype;	/* inicializa o tipo d�grafo/grafo */

	return Digraph;	/* devolve a refer�ncia do d�grafo criado */
}

int Destroy (PtDigraph *pdig)
{
	PtDigraph TmpDigraph = *pdig; PtBiNode Vertex, Edge;

	if (TmpDigraph == NULL) return NO_DIGRAPH;

	while (TmpDigraph->Head != NULL)	/* libertar a mem�ria dos v�rtices */
	{
		Vertex = TmpDigraph->Head;	/* v�rtice da cabe�a do d�grafo */
		TmpDigraph->Head = TmpDigraph->Head->PtNext;	/* atualizar cabe�a */

		while (Vertex->PtAdj != NULL)	/* libertar a mem�ria das arestas */
		{
			Edge = Vertex->PtAdj;	/* cabe�a da lista das arestas */
			Vertex->PtAdj = Vertex->PtAdj->PtNext;	/* atualizar cabe�a */
			free (Edge->PtElem);	/* libertar a mem�ria da aresta */
			free (Edge);	/* libertar o bin� da lista de arestas */
		}

		free (Vertex->PtElem);	/* libertar a mem�ria do v�rtice */
		free (Vertex);	/* libertar o bin� da lista de v�rtices */
	}

	free (TmpDigraph);	/* libertar a mem�ria ocupada pelo d�grafo */
	*pdig = NULL;	/* colocar a refer�ncia do d�grafo a NULL */

	return OK;
}

PtDigraph Copy (PtDigraph pdig)
{
  PtDigraph Copy; PtBiNode Vert, PEdge; PtEdge Edge;

  if (pdig == NULL) return NULL;

  Copy = Create (pdig->Type);

  /* inserir os v�rtices */
  for (Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext)
    if (InVertex (Copy, Vert->Number)) { Destroy (&Copy); return NULL; }

  /* inserir as arestas */
  for (Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext)
    for (PEdge = Vert->PtAdj; PEdge != NULL; PEdge = PEdge->PtNext)
    {
      Edge = (PtEdge) PEdge->PtElem;
      if (InEdge (Copy, Vert->Number, PEdge->Number, Edge->Cost))
      { Destroy (&Copy); return NULL; }
    }

  return Copy;  /* devolver a refer�ncia do Digrafo criado */
}

int InVertex (PtDigraph pdig, unsigned int pv)
{
	PtBiNode Insert, Node;	/* posi��o de inser��o e novo v�rtice */

	if (pdig == NULL) return NO_DIGRAPH;

					/* criar o bin� e o v�rtice */
	if ((Node = CreateBiNode (pv)) == NULL) return NO_MEM;
	if ((Node->PtElem = CreateVertex ()) == NULL)
	{ free (Node); return NO_MEM; }

					/* determinar posi��o de coloca��o e inserir o v�rtice */
	if (pdig->Head == NULL || pdig->Head->Number > pv)
	{				/* inser��o � cabe�a do d�grafo */
		Node->PtNext = pdig->Head; pdig->Head = Node;
		if (Node->PtNext != NULL) Node->PtNext->PtPrev = Node;
	}
	else
	{				/* inser��o � frente do n� de inser��o */
		if ((Insert = InPosition (pdig->Head, pv)) == NULL)
		{			/* inser��o sem sucesso, porque o v�rtice j� existe */
			DestroyBiNode (&Node); return REP_VERTEX;
		}
		Node->PtNext = Insert->PtNext;
		if (Node->PtNext != NULL) Node->PtNext->PtPrev = Node;
		Node->PtPrev = Insert; Insert->PtNext = Node;
	}

	pdig->NVertexes++;	/* atualizar o n�mero de v�rtices */
	return OK;
}

int OutVertex (PtDigraph pdig, unsigned int pv)
{
	PtBiNode Vertex, Edge, Delete;	/* posi��o de remo��o do v�rtice */

	if (pdig == NULL) return NO_DIGRAPH;
	if (pdig->NVertexes == 0) return DIGRAPH_EMPTY;

					/* determinar posi��o de remo��o do v�rtice */
	if ((Delete = OutPosition (pdig->Head, pv)) == NULL)
		return NO_VERTEX;

	while (Delete->PtAdj != NULL)	/* remover a lista de adjac�ncias */
	{	/* atualizar semigrau incidente da cabe�a da lista das arestas */
		((PtVertex) Delete->PtAdj->PtAdj->PtElem)->InDeg--;
		Edge = Delete->PtAdj;
		Delete->PtAdj = Delete->PtAdj->PtNext;	/* atualizar cabe�a */
		DestroyBiNode (&Edge);	/* destruir bin� com aresta */
		pdig->NEdges--;	/* atualizar o n�mero de arestas */
	}
					/* remo��o do v�rtice */
	if (Delete == pdig->Head)
	{				/* remo��o do v�rtice da cabe�a do d�grafo */
		if (Delete->PtNext != NULL) Delete->PtNext->PtPrev = NULL;
		pdig->Head = Delete->PtNext;
	}
	else
	{				/* remo��o de outro v�rtice do d�grafo */
		Delete->PtPrev->PtNext = Delete->PtNext;
		if (Delete->PtNext != NULL) Delete->PtNext->PtPrev = Delete->PtPrev;
	}
	DestroyBiNode (&Delete);	/* destruir bin� com v�rtice */
	pdig->NVertexes--;	/* atualizar o n�mero de v�rtices */

					/* remover as arestas incidentes */
	Vertex = pdig->Head;	/* v�rtice da cabe�a do d�grafo */
	while (Vertex != NULL)
	{				/* remover a aresta deste v�rtice para o v�rtice removido */
		if ((Edge = OutPosition (Vertex->PtAdj, pv)) != NULL)
		{
			if (Edge == Vertex->PtAdj)
			{		/* remo��o da aresta da cabe�a da lista das arestas */
				if (Edge->PtNext != NULL) Edge->PtNext->PtPrev = NULL;
				Vertex->PtAdj = Vertex->PtAdj->PtNext;
			}
			else
			{		/* remo��o de outra aresta da lista das arestas */
				Edge->PtPrev->PtNext = Edge->PtNext;
				if (Edge->PtNext != NULL) Edge->PtNext->PtPrev = Edge->PtPrev;
			}
			((PtVertex) Vertex->PtElem)->OutDeg--;	/* atualizar semigrau */
			DestroyBiNode (&Edge);	/* destruir bin� com aresta */
			if (pdig->Type) pdig->NEdges--;	/* atualizar o n�mero de arestas */
		}
		Vertex = Vertex->PtNext;	/* v�rtice seguinte do d�grafo */
	}
	return OK;
}

int InEdge (PtDigraph pdig, unsigned int pv1, unsigned int pv2, int pcost)
{
	PtBiNode V1, V2;	/* posi��o dos v�rtices adjacentes */

	if (pdig == NULL) return NO_DIGRAPH;
	if (pdig->NVertexes == 0) return NO_VERTEX;	/* sem v�rtices */
	if (pv1 == pv2) return REP_EDGE;	/* lacetes proibidos */

			/* verificar se os v�rtices existem e se a aresta j� existe */
	if ((V1 = OutPosition (pdig->Head, pv1)) == NULL)
		return NO_VERTEX;	/* v�rtice emergente inexistente */
	if (V1->PtAdj != NULL && OutPosition (V1->PtAdj, pv2) != NULL)
		return REP_EDGE;	/* aresta existente */
	if ((V2 = OutPosition (pdig->Head, pv2)) == NULL)
		return NO_VERTEX;	/* v�rtice incidente inexistente */

					/* inserir a aresta v1-v2 */
	if (InsertEdge (V1, V2, pcost) != OK) return NO_MEM;
	if (!pdig->Type)	/* se � grafo, inserir tamb�m a aresta v2-v1 */
		if (InsertEdge (V2, V1, pcost) != OK)
		{		/* se a aresta v2-v1 n�o foi inserida, remover a aresta v1-v2 */
			DeleteEdge (V1, V2); return NO_MEM;
		}

	pdig->NEdges++;	/* incrementar o n�mero de arestas */
	return OK;
}

int OutEdge (PtDigraph pdig, unsigned int pv1, unsigned int pv2)
{
	PtBiNode V1, V2;	/* posi��o dos v�rtices adjacentes */

	if (pdig == NULL) return NO_DIGRAPH;
	if (pdig->NVertexes == 0) return NO_VERTEX;
	if (pdig->NEdges == 0 || pv1 == pv2) return NO_EDGE;

			/* verificar se os v�rtices e a aresta existem */
	if ((V1 = OutPosition (pdig->Head, pv1)) == NULL)
		return NO_VERTEX;	/* v�rtice emergente inexistente */
	if (V1->PtAdj == NULL || OutPosition (V1->PtAdj, pv2) == NULL)
		return NO_EDGE;	/* aresta inexistente */
	if ((V2 = OutPosition (pdig->Head, pv2)) == NULL)
		return NO_VERTEX;	/* v�rtice incidente inexistente */

 
	DeleteEdge (V1, V2);	/* remover a aresta v1-v2 */
					/* se � grafo, remover tamb�m a aresta v2-v1 */
	if (!pdig->Type) DeleteEdge (V2, V1);

	pdig->NEdges--;	/* decrementar o n�mero de arestas */
	return OK;
}

int Type (PtDigraph pdig, unsigned int *pty)
{
	if (pdig == NULL) return NO_DIGRAPH;
	if (pty == NULL) return NULL_PTR;

	*pty = pdig->Type;
	return OK;
}

int VertexNumber (PtDigraph pdig, unsigned int *pnv)
{
	if (pdig == NULL) return NO_DIGRAPH;
	if (pnv == NULL) return NULL_PTR;

	*pnv = pdig->NVertexes;
	return OK;
}

int EdgeNumber (PtDigraph pdig, unsigned int *pne)
{
	if (pdig == NULL) return NO_DIGRAPH;
	if (pne == NULL) return NULL_PTR;

	*pne = pdig->NEdges;
	return OK;
}

int GetVertexList (PtDigraph pdig, unsigned int ppos, char *pvlist)
{
	PtBiNode PVert, PEdge; PtEdge Edge; char NodeList[20];

	if (pdig == NULL) return NO_DIGRAPH;
	if (ppos > pdig->NVertexes) return NO_VERTEX;
	if (pvlist == NULL) return NULL_PTR;

	ppos--;
	for (PVert = pdig->Head; ppos > 0; ppos--) PVert = PVert->PtNext;

	sprintf (NodeList, "Vertice %2d > ", PVert->Number);
	strcpy (pvlist, NodeList);

	for (PEdge = PVert->PtAdj; PEdge != NULL; PEdge = PEdge->PtNext)
	{
		sprintf (NodeList, "%2d (", PEdge->Number);
		strcat (pvlist, NodeList);
		Edge = (PtEdge) PEdge->PtElem;
		sprintf (NodeList, "%2d) ", Edge->Cost);
		strcat (pvlist, NodeList);
	}

	return OK;
}

PtDigraph CreateFile (char *pfilename)
{
  PtDigraph Digraph; FILE *PtF;
  unsigned int Type, NVertexes, NEdges, V, E, Vert1, Vert2; int Status, Cost;

  /* abertura com valida��o do ficheiro para leitura */
  if ( (PtF = fopen (pfilename, "r")) == NULL) return NULL;

  /* leitura do tipo de digrafo/grafo e cria��o do vazio */
  fscanf (PtF, "%d", &Type); fscanf (PtF, "%*c");

  /* leitura do n�mero de v�rtices e de arestas do ficheiro e cria��o do digrafo vazio */
  fscanf (PtF, "%d %d", &NVertexes, &NEdges); fscanf (PtF, "%*c");

  if ((Digraph = Create (Type)) == NULL) { fclose (PtF); return NULL; }

  /* leitura dos v�rtices do ficheiro */
  for (V = 0; V < NVertexes ; V++)
    {
      fscanf (PtF, "%d", &Vert1); fscanf (PtF, "%*c");
      if (InVertex (Digraph, Vert1))
      { Destroy (&Digraph); fclose (PtF); return NULL; }
    }

  /* leitura das arestas do ficheiro */
  for (E = 0; E < NEdges ; E++)
    {
      Status = fscanf (PtF, "%d %d %d", &Vert1, &Vert2, &Cost); fscanf (PtF, "%*c");
      if (Status != 3)
      { Destroy (&Digraph); fclose (PtF); return NULL; }

      if (InEdge (Digraph, Vert1, Vert2, Cost))
      { Destroy (&Digraph); fclose (PtF); return NULL; }
    }

  fclose (PtF);  /* fecho do ficheiro */

  return Digraph;  /* devolve o digrafo criado */
}

int StoreFile (PtDigraph pdig, char *pfilename)
{
  FILE *PtF; PtBiNode Vert, PEdge; PtEdge Edge;

  if (pdig == NULL) return NO_DIGRAPH;

  /* abertura com valida��o do ficheiro para escrita */
  if ((PtF = fopen (pfilename, "w")) == NULL) return NO_FILE;

  /* escrita do tipo do digrafo no ficheiro */
  fprintf (PtF, "%u\n", pdig->Type);

  /* escrita do n�mero de v�rtices e de arestas do digrafo no ficheiro */
  fprintf (PtF, "%u\t%u\n", pdig->NVertexes, pdig->NEdges);

  /* escrita dos v�rtices do digrafo no ficheiro */
  for (Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext)
    fprintf (PtF, "%u\n", Vert->Number);

  /* escrita das arestas do digrafo no ficheiro */
  for (Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext)
    for (PEdge = Vert->PtAdj; PEdge != NULL; PEdge = PEdge->PtNext)
    {
      Edge = (PtEdge) PEdge->PtElem;
      fprintf (PtF, "%u\t%u\t%d\n", Vert->Number, PEdge->Number, Edge->Cost);
    }

  fclose (PtF);  /* fecho do ficheiro */

  return OK;
}

/********************** Aula 13 e Trabalho Final  **********************/

int VertexType (PtDigraph pdig, unsigned int pv)
{
    PtBiNode pbinode;
    PtVertex pvertex;

    if (pdig == NULL)
        return NO_DIGRAPH;
    if (!pdig->NVertexes)
        return DIGRAPH_EMPTY;
    if ((pbinode = OutPosition(pdig->Head, pv)) == NULL)
		return NO_VERTEX;

    pvertex = (PtVertex)(pbinode->PtElem);
    if (pvertex->InDeg && !(pvertex->OutDeg))
        return SINK;
    if (!(pvertex->InDeg) && pvertex->OutDeg)
        return SOURCE;
    if (!(pvertex->InDeg) && !(pvertex->OutDeg))
        return DISC;

    return OK;
}

int Dijkstra (PtDigraph pdig, unsigned int pv, unsigned int pvpred[], int pvcost[])
{
    if(pdig == NULL)
        return NO_DIGRAPH;
    if(!pdig->NVertexes)
        return DIGRAPH_EMPTY;
    if(pvpred == NULL || pvcost == NULL)
        return NULL_PTR;
    if(OutPosition(pdig->Head, pv) == NULL)
		return NO_VERTEX;

    DijkstraPQueue(pdig, pv, pvpred, pvcost);

    return OK;
}

int Reach (PtDigraph pdig, unsigned int pv, unsigned int pvlist[])
{
    PtBiNode V;
    int *pvcost;
    unsigned int i;
    unsigned int *pvpred;

    if(pdig == NULL)
        return NO_DIGRAPH;
    if(!pdig->NVertexes)
        return DIGRAPH_EMPTY;
    if(pvlist == NULL)
        return NULL_PTR;
    if(OutPosition(pdig->Head, pv) == NULL)
        return NO_VERTEX;
    if((pvpred = (unsigned int*)malloc(pdig->NVertexes * sizeof(unsigned int))) == NULL)
        return NO_MEM;
    if((pvcost = (int*)malloc(pdig->NVertexes * sizeof(int))) == NULL) {
        free(pvpred);
        return NO_MEM;
    }

    DijkstraPQueue(pdig, pv, pvpred, pvcost);

    for(i = 1, V = pdig->Head; V != NULL; V = V->PtNext)
        if(pvpred[V->Number-1] != 0)
            pvlist[i++] = V->Number;
    pvlist[0] = i-1;

    free(pvpred);
    free(pvcost);
    return OK;
}

int DigraphComplete (PtDigraph pdig, unsigned int *pcomp)
{
    if(pdig == NULL)
        return NO_DIGRAPH;
    if(!pdig->NVertexes)
        return DIGRAPH_EMPTY;
    if(pcomp == NULL)
        return NULL_PTR;

    *pcomp = pdig->NEdges == (pdig->NVertexes) * (pdig->NVertexes - 1);
    return OK;
}

PtDigraph DigraphTranspose (PtDigraph pdig)
{
    PtDigraph Transpose;
    PtBiNode Vert, PEdge;
    PtEdge Edge;

    if(pdig == NULL)
        return NULL;
    if((Transpose = Create(pdig->Type)) == NULL)
        return NULL;

    for(Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext)
        if(InVertex(Transpose, Vert->Number)) {
            Destroy(&Transpose);
            return NULL;
        }

    for(Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext)
        for(PEdge = Vert->PtAdj; PEdge != NULL; PEdge = PEdge->PtNext) {
            Edge = (PtEdge)PEdge->PtElem;
            if(InEdge(Transpose, PEdge->Number, Vert->Number, Edge->Cost)) {
                Destroy(&Transpose);
                return NULL;
            }
        }

    return Transpose;
}

int DigraphStronglyConnected (PtDigraph pdig, unsigned int *pstrong)
{
    PtBiNode Vert;
    unsigned int *pvlist;

    if(pdig == NULL)
        return NO_DIGRAPH;
    if(pdig->NVertexes == 0)
        return DIGRAPH_EMPTY;
    if(pstrong == NULL)
        return NULL_PTR;
    if((pvlist = (unsigned int*)malloc(pdig->NVertexes * sizeof(unsigned int))) == NULL)
        return NO_MEM;

    for(Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext) {
        if(Reach(pdig, Vert->Number, pvlist) != OK) {
            free(pvlist);
            return NO_MEM;
        }
        if(pvlist[0] != pdig->NVertexes - 1) {
            *pstrong = 0;
            free(pvlist);
            return OK;
        }
    }

    *pstrong = 1;
    free(pvlist);
    return OK;
}

int DigraphTransitiveClosure (PtDigraph pdig)
{
    PtBiNode Vert;
    unsigned int i, *pvlist;

    if(pdig == NULL)
        return NO_DIGRAPH;
    if(pdig->NVertexes == 0)
        return DIGRAPH_EMPTY;
    if((pvlist = (unsigned int*)malloc(pdig->NVertexes * sizeof(unsigned int))) == NULL)
        return NO_MEM;

    for(Vert = pdig->Head; Vert != NULL; Vert = Vert->PtNext) {
        if(Reach(pdig, Vert->Number, pvlist) != OK) {
            free(pvlist);
            return NO_MEM;
        }
        for(i = 1; i <= pvlist[0]; i++)
            /*   A fun��o InEdge j� verifica se a aresta a inserir j� existe.
             *   Em caso afirmativo, retorna REP_EDGE, se bem que neste caso �
             * irrelevante.
             *   Assim sendo, � ignorada essa verifica��o por uma quest�o de
             * efici�ncia. */
            InEdge(pdig, Vert->Number, pvlist[i], 1);
    }

    free(pvlist);
    return OK;
}

/***************** Defini��o dos Subprogramas Internos *****************/

/* Fun��o que insere, de facto, uma aresta no d�grafo/grafo. Em caso de sucesso devolve OK, sen�o devolve NO_MEM para assinalar falta de mem�ria. */

static int InsertEdge (PtBiNode pv1, PtBiNode pv2, int pcost)
{
	PtBiNode Insert, Node;	/* posi��o de inser��o e nova aresta */

					/* criar o bin� e a aresta */
	if ((Node = CreateBiNode (pv2->Number)) == NULL) return NO_MEM;
	if ((Node->PtElem = CreateEdge (pcost)) == NULL)
	{ free (Node); return NO_MEM; }

					/* determinar posi��o de coloca��o e inserir a aresta */
	if (pv1->PtAdj == NULL || pv1->PtAdj->Number > pv2->Number)
	{				/* inser��o � cabe�a da lista das arestas */
		Node->PtNext = pv1->PtAdj; pv1->PtAdj = Node;
		if (Node->PtNext != NULL) Node->PtNext->PtPrev = Node;
	}
	else
	{				/* inser��o � frente do n� de inser��o */
		Insert = InPosition (pv1->PtAdj, pv2->Number);
		Node->PtNext = Insert->PtNext;
		if (Node->PtNext != NULL) Node->PtNext->PtPrev = Node;
		Node->PtPrev = Insert; Insert->PtNext = Node;
	}

	Node->PtAdj = pv2;	/* ligar o v�rtice 1 ao v�rtice 2 */
	/* incrementar semigraus dos v�rtices emergente do 1 e incidente do 2 */
	((PtVertex) pv1->PtElem)->OutDeg++;
	((PtVertex) pv2->PtElem)->InDeg++;

	return OK;
}

/* Fun��o que remove, de facto, uma aresta do d�grafo/grafo. */

static void DeleteEdge (PtBiNode pv1, PtBiNode pv2)
{
	PtBiNode Delete;	/* posi��o de remo��o da aresta */

					/* determinar posi��o de remo��o da aresta */
	Delete = OutPosition (pv1->PtAdj, pv2->Number);

	if (Delete == pv1->PtAdj)	/* remo��o da aresta */
	{				/* remo��o da aresta da cabe�a da lista das arestas */
		if (Delete->PtNext != NULL) Delete->PtNext->PtPrev = NULL;
		pv1->PtAdj = Delete->PtNext;
	}
	else
	{				/* remo��o de outra aresta do v�rtice */
		Delete->PtPrev->PtNext = Delete->PtNext;
		if (Delete->PtNext != NULL) Delete->PtNext->PtPrev = Delete->PtPrev;
	}

	DestroyBiNode (&Delete);	/* destruir bin� com aresta */

	/* decrementar semigraus dos v�rtices emergente do 1 e incidente do 2 */
	((PtVertex) pv1->PtElem)->OutDeg--;
	((PtVertex) pv2->PtElem)->InDeg--;
}

/* Fun��o que cria o v�rtice do d�grafo/grafo. Devolve a refer�ncia do v�rtice criado ou NULL, caso n�o consiga cri�-lo por falta de mem�ria. */
 
static PtVertex CreateVertex (void)
{
	PtVertex Vertex;

	if ((Vertex = (PtVertex) malloc (sizeof (struct vertex))) == NULL)
		return NULL;

	Vertex->InDeg = 0;	/* inicializa o semigrau incidente */
	Vertex->OutDeg = 0;	/* inicializa o semigrau emergente */
	return Vertex;	/* devolve o v�rtice criado */
}

/* Fun��o que cria a aresta do d�grafo/grafo. Devolve a refer�ncia da aresta criada ou NULL, caso n�o consiga cri�-la por falta de mem�ria. */

static PtEdge CreateEdge (int pcost)
{
	PtEdge Edge;

	if ((Edge = (PtEdge) malloc (sizeof (struct edge))) == NULL)
		return NULL;

	Edge->Cost = pcost;	/* armazena o custo da aresta */
	return Edge;	/* devolve a aresta criada */
}

/* Fun��o que cria o bin� da lista de v�rtices ou da lista de arestas. Devolve a refer�ncia do bin� criado ou NULL, caso n�o consiga cri�-lo por falta de mem�ria. */

static PtBiNode CreateBiNode (unsigned int pnumber)
{
	PtBiNode Node;

	if ((Node = (PtBiNode) malloc (sizeof (struct binode))) == NULL)
		return NULL;

	Node->PtNext = NULL;	/* bin� aponta para a frente para NULL */
	Node->PtPrev = NULL;	/* bin� aponta para a tr�s para NULL */
	Node->PtAdj = NULL;	/* lista de adjac�ncias nula */
	Node->Number = pnumber;	/* armazena o identificador do bin� */
	return Node;	/* devolve o bin� criado */
}

/* Fun��o que liberta a mem�ria ocupada pelo bin� e pelo seu elemento. */

static void DestroyBiNode (PtBiNode *pbinode)
{
	if (*pbinode == NULL) return;

	free ((*pbinode)->PtElem);
	free (*pbinode);
	*pbinode = NULL;
}

/* Fun��o de pesquisa para inser��o. Devolve um ponteiro para o bin� � frente do qual deve ser feita a inser��o do novo v�rtice (nova aresta) ou NULL, caso o v�rtice (a aresta) j� exista. */
 
static PtBiNode InPosition (PtBiNode phead, unsigned int pnumber)
{
	PtBiNode Node, Prev;

	for (Node = phead; Node != NULL; Node = Node->PtNext)
	{
		if (Node->Number >= pnumber) break;
		Prev = Node;
	}

	if (Node == NULL || Node->Number > pnumber) return Prev;
	else return NULL;	/* o elemento j� existe */
}

/* Fun��o de pesquisa para remo��o. Devolve um ponteiro para o bin� onde se encontra o v�rtice (a aresta) ou NULL, caso o v�rtice (a aresta) n�o exista. */

static PtBiNode OutPosition (PtBiNode phead, unsigned int pnumber)
{
	PtBiNode Node;

	for (Node = phead; Node != NULL; Node = Node->PtNext)
		if (Node->Number == pnumber) break;
	return Node;
}

/* Algoritmo de Dijkstra que determina os caminhos mais curtos de todos os v�rtices alcanc�veis pelo v�rtice pvertice. Esta vers�o assume que o digrafo, o v�rtice de partida e as sequ�ncias foram previamente validados. */

static int DijkstraPQueue (PtDigraph pdig, unsigned int pv, unsigned int pvpred[], int pcost[])
{
	PtPQueue PQueue; PtBiNode V, E; int Cost; VERTEX Elem;

	if (pdig == NULL) return NO_DIGRAPH;
	if (pdig->NVertexes == 0) return DIGRAPH_EMPTY;
				/* verificar se o v�rtice de partida existe */
	if ((V = OutPosition (pdig->Head, pv)) == NULL) return NO_VERTEX;

	PQueue = PQueueCreate (pdig->NVertexes);	/* criar a fila com prioridade */
	for (V = pdig->Head; V != NULL; V = V->PtNext)
	{
		V->Visit = 0;	/* desmarcar o v�rtice */
		pvpred[V->Number-1] = 0;	/* inicializar o predecessor */
		pcost[V->Number-1] = INT_MAX;	/* inicializar o custo */

				/* construir o elemento e p�-lo na fila com prioridade */
		Elem.Vertex = V->Number; Elem.Cost = pcost[V->Number-1];
		PQueueInsert (PQueue, &Elem);
	}
				/* inicializar o v�rtice de partida e atualiz�-lo na fila */
	Elem.Vertex = pv; Elem.Cost = pcost[pv-1] = 0;
	PQueueDecrease (PQueue, &Elem);

	while (!PQueueIsEmpty (PQueue))
	{		/* retirar o elemento da fila e pesquisar o v�rtice no grafo */
		PQueueDeleteMin (PQueue, &Elem);
		V = OutPosition (pdig->Head, Elem.Vertex);
		V->Visit = 1;	/* marcar o v�rtice como conhecido */
		if (pcost[V->Number-1] == INT_MAX) continue;

		for (E = V->PtAdj; E != NULL; E = E->PtNext)
			if (!E->PtAdj->Visit)	/* n�o processar v�rtices conhecidos */
			{	/* calcular o custo desta travessia */
				Cost = pcost[V->Number-1] + ((PtEdge) E->PtElem)->Cost;
				if (Cost < pcost[E->Number-1])
				{	/* atualizar o percurso e o custo desta travessia */	
					pvpred[E->Number-1] = V->Number;
					pcost[E->Number-1] = Cost;
				/* atualizar o elemento na fila com prioridade */
					Elem.Vertex = E->Number; Elem.Cost = Cost;
					PQueueDecrease (PQueue, &Elem);
				}
			}
	}

	PQueueDestroy (&PQueue);	/* destruir a fila com prioridade */
	return OK;
}
