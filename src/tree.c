#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tree.h"

typedef bool (*cmpFunc)(void*, void*);

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
// Estruturas para a árvore
typedef struct tree{
    Node*  raiz;    // Raiz da árvore
}Tree;

// Nó da árvore
typedef struct node{
    void*  info; // Informação associada ao nó

    Node*  esq;  // Filho esquerdo
    Node*  dir;  // Filho direito
}Node;
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
Tree* criarTree(){
    Tree* t = (Tree*) malloc(sizeof(Tree));
    if(t == NULL){
        printf("Erro ao alocar memoria para a arvore Tree.\n");
        return NULL;
    } t->raiz = NULL;

    return t;
}
int inserirTree(Tree* t, Info* info) {
    if (t == NULL || info == NULL) {
        printf("Arvore ou informacao invalida para insercao.\n");
        return -1;
    }

    // Pegamos a raiz atual, passamos para a recursão, e ATUALIZAMOS a raiz da árvore
    // (Isso resolve o problema de inserir o primeiríssimo elemento na árvore vazia)
    t->raiz= inserirNode(t->raiz, info); 
    
    return 0;
}
Node* getNode(Node* root, void* info){
    if(root == NULL || root->info == info) return root;

    if(info < root->info) return getNode(root->esq, info);
    else                  return getNode(root->dir, info);

    // Nó não encontrado
    return NULL;
}
int freeTree(Tree* t){
    if(t == NULL) return -1;
    
    freeNode(t->raiz);
    free(t);

    return 0;
}
int deleteNode(Tree* t, Node* n){
    if(t == NULL || n == NULL) return -1;

    // Implementação da remoção do nó
    // ...

    return 0;
}
void printTree(Node* root, int espaco){
    printf("================ ARVORE ===============\n");
    
    // 1: Caso base (se o nó for nulo), retornar
    if(root == NULL) return;

    // Aumenta a distância entre os níveis (pode alterar o 10 para mais ou menos espaço)
    espaco += 10;
    
    // 3: Processa o filho da DIREITA primeiro (ele ficará na parte de cima da tela)
    printTree(root->dir, espaco);
    
    // Imprime o NÓ ATUAL
    printf("\n");
    for (int i = 10; i < espaco; i++) printf(" "); // Imprime os espaços em branco para criar a indentação
    
    printf("%d\n", 1); // Teste
    
    // 4: Processa o filho da ESQUERDA por último (ele ficará na parte de baixo da tela)
    printTree(root->esq, espaco);
    
    printf("=======================================\n");
}
void percursoSimetrico(Node* root, void* aux){
    // 1: Verificar se o nó atual é NULL. Se for, retornar
    if(root == NULL) return;

    // 2: Percorrer a subárvore esquerda, processar o nó atual e percorrer a subárvore direita
    percursoSimetrico(root->esq, aux);
        // Processar o nó atual
    percursoSimetrico(root->dir, aux);
}
/*###############################################################################################*/



/*                                       FUNÇÕES SECUNDÁRIAS                                     */
Node* inserirNode(Node* root, Info* info){
    // 1: Se a raiz for NULL, cria um novo nó com a informação e retorna esse nó para ser conectado pelo "pai"
    if(root == NULL){
        Node* newNode = (Node*)malloc(sizeof(Node));
        if(newNode == NULL){
            printf("Erro ao alocar memoria para o novo no.\n");
            return NULL;
        }
        newNode->info = info;
        newNode->esq  = NULL;
        newNode->dir  = NULL;
        
        // 1.1: Retorna o novo nó para que ele seja conectado pelo "pai"
        return newNode; 
    }

    // 2: Se não é vazio, desce na árvore (Exemplo inserindo apenas na esquerda)
    // Atualiza o filho esquerdo do nó atual com o resultado da recursão (Isso é importante para conectar o novo nó criado na etapa 1.1)
    root->esq = inserirNode(root->esq, info); 

    // Retorna a própria raiz inalterada para manter a conexão original
    return root; 
}
void rotateRight(Tree* t, Node** pRoot){
    Node* child = (*pRoot)->esq;
    Node* aux   = child->dir;

    child->dir    = *pRoot;
    (*pRoot)->esq = aux;

    *pRoot = child;
}
void rotateLeft(Tree* t, Node** pRoot){
    Node* child = (*pRoot)->dir;
    Node* aux   = child->esq;

    child->esq    = *pRoot;
    (*pRoot)->dir = aux;

    *pRoot = child;
}
int freeNode(Node* n){
    if(n == NULL) return -1;

    freeNode(n->esq);
    freeNode(n->dir);

    // Liberar a informação associada ao nó
    if(n->info != NULL) free(n->info);

    free(n);
    return 0;
}
Node* getRoot(Tree* t){
    if(t == NULL) return NULL;
    return t->raiz;
}
/*###############################################################################################*/