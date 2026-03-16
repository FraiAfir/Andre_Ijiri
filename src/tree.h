#ifndef _TREE_H_
#define _TREE_H_

// #include "program.h"

typedef struct tree Tree;
typedef struct info Info;
typedef struct node Node;

/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Cria uma árvore vazia, onde os nós armazenarão informações do tipo Info
 * @return Árvore vazia. NULL caso não tenha conseguido criar a árvore
 */
Tree* criarTree();
/**
 * Função para inserir um nó ná árvore com a informacao info
 * @param t    A árvore onde o nó será inserido
 * @param info Informação a ser armazenada no nó da árvore
 * @return     0 se a informação foi inserida com sucesso. -1 caso já exista um nó com info
 */
int inserirTree(Tree* t, Info* info);
/**
 * Função para buscar o nó da árvore cuja informação é info
 * @param root O nó raiz da árvore onde a busca será realizada
 * @param info Informação a ser buscada na árvore
 * @return     Nó da árvore cuja informação é info. NULL, caso não exista um nó essa informação
 */
Node* getNode(Node* root, void* info);
/**
 * Função para remover o nó n da árvore t
 * @param t A árvore onde o nó será removido
 * @param n O nó a ser removido
 * @return  0 se o nó foi removido com sucesso. -1 caso o nó n seja inválido ou não exista na árvore t
 */
int deleteNode(Tree* t, Node* n);
/**
 * Função para imprimir a árvore t no formato DOT, que pode ser visualizado usando o Graphviz
 * Veja: https://graphviz.org/
 * @param root    O nó raiz da árvore que será impressa
 * @param espaco  Distância entre os níveis da árvore
 * @return        Void
 */
void printTree(Node* root, int espaco);
/**
 * Percorre a árvore em simétrico, invocando a função fVisita (veja descrição acima) em cada nó visitado
 * @param t A árvore a ser percorrida
 * @param fVisita Função a ser invocada em cada nó visitado
 * @param aux Parâmetro adicional a ser passado para a função fVisita
 * @return Void
 */
void percursoSimetrico(Node* root, void *aux);
/**
 * Função para desalocar a árvore t e todos os seus nós
 * @param t A árvore a ser desalocada
 * @return  0 se a árvore foi desalocada com sucesso. -1 caso contrário
 */
int freeTree(Tree* t);
/*###############################################################################################*/



/*                                       FUNÇÕES SECUNDÁRIAS                                     */
/**
 * Função para rotacionar o nó raiz da árvore t para a direita
 * @param t     A árvore que ocorrerá a rotação
 * @param pRoot Ponteiro para o nó raiz da árvore t que será rotacionado
 * @return      Void
 * @note        pRoot é um ponteiro duplo para que a função possa alterar o ponteiro raiz da árvore,
 * caso a rotação mude a raiz da árvore. 
 */
void rotateRight(Tree* t, Node** pRoot);
/**
 * Função para rotacionar o nó raiz da árvore t para a esquerda
 * @param t     A árvore que ocorrerá a rotação
 * @param pRoot Ponteiro para o nó raiz da árvore t que será rotacionado
 * @return      Void
 * @note        pRoot é um ponteiro duplo para que a função possa alterar o ponteiro raiz da árvore, 
 * caso a rotação mude a raiz da árvore.
 */
void rotateLeft(Tree* t, Node** pRoot);
/**
 * Função para desalocar o nó n e todos os seus filhos (percorre a árvore recursivamente)
 * @param n O nó a ser desalocado (percorre a árvore recursivamente)
 * @return  0 se o nó foi desalocado com sucesso. -1 caso contrário
 * @note    Esta função é chamada por 'freeTree' para desalocar todos os nós da árvore
 */
int freeNode(Node* n);
/**
 * Função para buscar o nó raiz da árvore t
 * @param t A árvore onde a busca será realizada
 * @return  Nó raiz da árvore t. NULL, caso a árvore t seja nula
 */
Node* getRoot(Tree* t);
/**
 * Função para inserir um nó na árvore de forma recursiva (Exemplo de inserção apenas na esquerda)
 * @param root O nó raiz da árvore onde a inserção será realizada
 * @param info Informação a ser armazenada no nó da árvore
 * @return     Nó da árvore com a informação inserida. NULL, caso não tenha conseguido inserir a informação na árvore
 * @note       Esta função é chamada por 'inserirTree' para inserir um nó na árvore t. 
 * Ela é recursiva e retorna o nó raiz da árvore após a inserção, para garantir que a conexão entre os nós seja mantida corretamente.
 */
Node* inserirNode(Node* root, Info* info);
/*###############################################################################################*/

#endif