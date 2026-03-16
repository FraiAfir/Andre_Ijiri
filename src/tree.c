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
    }

    t->raiz    = NULL;

    return t;
}
int insertTree(Tree* t, Node* root, Info* info){
    // Verificar se a árvore ou a informação a ser inserida são inválidas
    if(t == NULL || info == NULL){
        printf("Arvore ou informacao invalida para insercao.\n");
        return -1;
    }

    // Se a raiz da árvore for NULL, insere a informação na raiz  
    if(root == NULL){
        // 
        Node* newNode = (Node*) malloc(sizeof(Node));
        if(newNode == NULL){
            printf("Erro ao alocar memória para o novo nó.\n");
            return -1;
        }

        // Inicializar o novo nó
        newNode->info = info;
        newNode->esq = NULL;
        newNode->dir = NULL;
    }

    // Comparar a informação a ser inserida com a informação do nó atual da árvore
    // else{
    //     if(comparar(info, root->info)) return insertTree(t, root->esq, info, comparar);
    //     else                           return insertTree(t, root->dir, info, comparar);
    // }

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
void printTree(Tree* t, char* arqPrint){
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
/*###############################################################################################*/