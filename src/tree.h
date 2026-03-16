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
 * @param t    A árvore onde a informação será inserida
 * @param root Nó raiz da árvore onde a informação será inserida. Se for NULL, a informação será inserida na raiz da árvore
 * @param info Informação a ser armazenada
 * @return     0 se a informação foi inserida com sucesso. -1 caso já exista um nó com info
 */
int insertTree(Tree* t, Node* root, Info* info);
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
 * @param t       A árvore a ser impressa
 * @param nomeArq Nome do arquivo onde a árvore será impressa
 * @return        Void
 */
void printTree(Tree* t, char *arqPrint);
/**
 * Percorre a árvore em simétrico, invocando a função fVisita (veja descrição acima) em cada nó visitado
 * @param t A árvore a ser percorrida
 * @param fVisita Função a ser invocada em cada nó visitado
 * @param aux Parâmetro adicional a ser passado para a função fVisita
 * @return Void
 */
void percursoSimetrico(Tree* t, void *aux);
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
/*###############################################################################################*/

#endif