#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

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
Tree* createTree(){
    Tree* t = (Tree*) malloc(sizeof(Tree));
    if(t == NULL){
        printf("Erro ao alocar memoria para a arvore Tree.\n");
        return NULL;
    }

    t->raiz    = NULL;

    return t;
}
int insertTree(Tree* t, Node* root, void* info){
    if(t == NULL || info == NULL){
        printf("Arvore ou informacao invalida para insercao.\n");
        return -1;
    }

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

    else{
        if(){

        }
    }

    return 0;
}
Node* getNode(Node* root, void* info){
    if(root == NULL || root->info == info) return root;

    if(info < root->info) return getNode(root->esq, info);
    else                  return getNode(root->dir, info);

    // Nó não encontrado
    return NULL;
}
int deleteNode(Tree* t, Node* n){
    if(t == NULL || n == NULL) return -1;

    // Implementação da remoção do nó
    // ...

    return 0;
}
int freeTree(Tree* t){
    if(t == NULL) return -1;
    
    freeNode(t->raiz);
    free(t);

    return 0;
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
int deleteNode(Tree* t, Node* n){
    if(t == NULL || n == NULL) return -1;

    // Implementação da remoção do nó
    // ...

    return 0;
}
void printTree(Tree* t, char* arqPrint){

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
/*###############################################################################################*/