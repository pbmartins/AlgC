/*******************************************************************************

 Ficheiro de interface do Tipo de Dados Abstracto SET (set.h).
 O conjunto é composto pela seu cardinal e pelos seus elementos de tipo carácter.
 A implementação providencia um construtor para criar um conjunto vazio. É da
 responsabilidade da aplicação, invocar o destrutor, para libertar a memória
 atribuída ao objecto. O módulo providencia um controlo centralizado de erro,
 disponibilizando uma função para obter o último erro ocorrido, uma função para
 obter uma mensagem de erro elucidativa e uma função para limpar o erro.
 Providencia também operações para armazenar e recuperar conjuntos para ficheiros.

 Autor : António Manuel Adrego da Rocha    Data : Março de 2015

 Interface file of the abstract data type SET (set.h). The set is defined
 by its cardinal and its components (capital characters) stored in a double-link
 list. The implementation provides a constructor for creating an empty set. The
 application has the responsibility of calling the destructor to release the 
 dynamic memory allocated to the set and the list of elements. The data-type has
 a central control error mechanism, providing operations for obtaining the last 
 error occurred, for obtaining the correspondent message and for cleaning the error.
 The data-type has also operations to store and retrieve sets from text files.

*******************************************************************************/

#ifndef _SET
#define _SET

/**************** Definição do Tipo Ponteiro para um Conjunto *****************/

typedef struct set *PtSet;

/************************ Definição de Códigos de Erro ************************/

#define  OK        0  /* operação realizada com sucesso - operation with success */
#define  NO_SET    1  /* o(s) conjunto(s) não existe(m) - set(s) do not exist */
#define  NO_MEM    2  /* memória esgotada - out of memory */
#define  NO_FILE   3  /* o ficheiro não existe - file does not exist */
#define  NO_ELEM   4  /* o elemento não existe no conjunto - element does not exist in the set */
#define  REP_ELEM  5  /* o elemento já existe no conjunto - element already exists in the set */

/*************************** Protótipos das Funções ***************************/

void SetClearError (void);
/*******************************************************************************
 Inicialização do erro. Error initialization.
*******************************************************************************/

int SetError (void);
/*******************************************************************************
 Devolve o código do último erro ocorrido. Returns the error code.
*******************************************************************************/

char *SetErrorMessage (void);
/*******************************************************************************
 Devolve uma mensagem esclarecedora da causa do último erro ocorrido.
 Returns an error message.
*******************************************************************************/

PtSet SetCreate (void);
/*******************************************************************************
 Cria um conjunto nulo. Devolve a referência do conjunto criado ou NULL, caso
 não consiga criar o conjunto, por falta de memória. Valores de erro: OK ou NO_MEM.
 
 Creates the empty set. Returns a reference to the new set or NULL if there 
 isn't enough memory. Error codes: OK or NO_MEM.
*******************************************************************************/

void SetDestroy (PtSet *pset);
/*******************************************************************************
 Destrói o conjunto pset, caso ele exista, e coloca a sua referência a NULL.
 Valores de erro: OK ou NO_SET.
 
 Destroys the set and releases the memory. Error codes: OK or NO_SET.
*******************************************************************************/

PtSet SetCopy (PtSet pset);
/*******************************************************************************
 Copia o conjunto pset, caso ele exista. Devolve a referência do conjunto cópia 
 ou NULL, caso não consiga fazer a cópia. Valores de erro: OK, NO_SET ou NO_MEM.

 Copies the set pset returning a new set or NULL if there isn't enough memory.
 Error codes: OK, NO_SET or NO_MEM.
*******************************************************************************/

int SetCardinal (PtSet pset);
/*******************************************************************************
 Determina o cardinal do conjunto pset. Valores de erro: OK ou NO_SET.

 Returns the cardinal of the set pset. Error codes: OK or NO_SET. 
*******************************************************************************/

int SetInsertElement (PtSet pset, char pelem);
/*******************************************************************************
 Insere pelem no conjunto pset, caso ele ainda não exista no conjunto. Devolve 1
 no caso da inserção ser realizada com sucesso e 0 no caso contrário.
 Valores de erro: OK, NO_SET, REP_ELEM ou NO_MEM.
 
 Inserts pelem in set pset, if it does not exist yet in the set. Returns 1 if the 
 insertion has success and 0 otherwise. Error codes: OK, NO_SET, REP_ELEM or NO_MEM.
*******************************************************************************/

int SetDeleteElement (PtSet pset, char pelem);
/*******************************************************************************
 Remove pelem do conjunto pset, caso ele exista no conjunto. Devolve 1 no caso
 da remoção ser realizada com sucesso e 0 no caso contrário.
 Valores de erro: OK, NO_SET ou NO_ELEM.
 
 Deletes pelem from the set pset, if it does exist in the set. Returns 1 if the 
 deletion has success and 0 otherwise. Error codes: OK, NO_SET or NO_ELEM.
*******************************************************************************/

char SetObserveElement (PtSet pset, unsigned int ppos);
/*******************************************************************************
 Devolve o elemento armazenado na posição ppos do conjunto pset.
 Valores de erro: OK, NO_SET ou NO_ELEM.
 
 Returnes the character stored in the position ppos of the set pset.
 Error codes: OK, NO_SET or NO_ELEM.
*******************************************************************************/

int SetSearchElement (PtSet pset, char pelem);
/*******************************************************************************
 Verifica se pelem existe no conjunto pset. Devolve 1 em caso afirmativo e 0 em 
 caso contrário. Valores de erro: OK ou NO_SET.

 Verifies if pelem exists in the set pvset. Returns 1 in affirmative case and 0 
 otherwise. Error codes: OK or NO_SET.
*******************************************************************************/

PtSet SetReunion (PtSet pset1, PtSet pset2);
/*******************************************************************************
 Faz a reunião dos dois conjuntos pset1 e pset2. Devolve a referência do conjunto
 criado ou NULL, caso não consiga realizar a reunião por falta de memória.
 Valores de erro: OK, NO_SET ou NO_MEM.
 
 Set reunion returning a new set. Error codes: OK, NO_SET or NO_MEM.
*******************************************************************************/

PtSet SetIntersection (PtSet pset1, PtSet pset2);
/*******************************************************************************
 Faz a intersecção dos dois conjuntos pset1 e pset2. Devolve a referência do 
 conjunto criado ou NULL, caso não consiga realizar a intersecção por falta de
 memória. Valores de erro: OK, NO_SET ou NO_MEM.

 Set intersection returning a new set. Error codes: OK, NO_SET or NO_MEM.
*******************************************************************************/

PtSet SetSymmetricDifference (PtSet pset1, PtSet pset2);
/*******************************************************************************
 Faz a diferença simétrica dos dois conjuntos pset1 e pset2 (a reunião menos a 
 intersecção). Devolve a referência do conjunto criado ou NULL, caso não consiga
 realizar a intersecção por falta de memória. Valores de erro: OK, NO_SET ou NO_MEM.
 
 Set symmetric difference returning a new set. Error codes: OK, NO_SET or NO_MEM.
*******************************************************************************/

int SetEquals (PtSet pset1, PtSet pset2);
/*******************************************************************************
 Compara se os dois conjuntos pset1 e pset2 são idênticos. Devolve 1 em caso 
 afirmativo e 0 em caso contrário. Valores de erro: OK ou NO_SET.

 Verifies if two sets are equal (same cardinal and all components are equal).
 Returns 1 in affirmative case and 0 otherwise. Error codes: OK or NO_SET.
*******************************************************************************/

void SetStoreFile (PtSet pset, char *pfname);
/*******************************************************************************
 Armazena o conjunto pset, caso ele exista, no ficheiro pfname. O ficheiro tem
 na primeira linha o cardinal do conjunto, seguido dos elementos, um por linha.
 Valores de erro: OK, NO_SET ou NO_FILE.
 
 Stores set pset in text file pfname. Error codes: OK, NO_SET or NO_FILE.
*******************************************************************************/

PtSet SetCreateFile (char *pfname);
/*******************************************************************************
 Cria um conjunto a partir de informacao lida do ficheiro pfname. Devolve a
 referência do conjunto criado ou NULL, caso não consiga criar o conjunto, por
 falta de memória. Valores de erro: OK, NO_FILE ou NO_MEM.

 Creating a new set from text file pfname. Returns a reference to the new set
 or NULL if there isn't enough memory. Error codes: OK, NO_FILE or NO_MEM.
*******************************************************************************/

#endif
