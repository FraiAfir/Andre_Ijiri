#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "streap.h"

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
// Estruturas para a árvore treap espacial
typedef struct streap{
    Node*  raiz;     // Raiz da árvore
    double epsilon; // Precisão para comparar números reais
}STreap;
// Estruturas para a informação, nó e lista de nós
typedef struct info{
    char*  descricao; // Descrição da informação
    double valor;    // Valor associado à informação
}Info;
// Nó da árvore
typedef struct node{
    double x, y;       // Coordenadas da âncora
    int    prioridade; // Prioridade do nó (para manter a propriedade de heap da treap)
    Info*  info;       // Informação associada ao nó

    double mbbX1, mbbY1, mbbX2, mbbY2; // Bounding box do nó (retângulo envolvente)

    Node*  esq; // Filho esquerdo
    Node*  dir; // Filho direito
}Node;
// Lista de nós
typedef struct lista{
    Node** nos;        // Array de nós
    int    tamanho;    // Tamanho atual da lista
    int    capacidade; // Capacidade máxima da lista
}Lista;
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
STreap* createSTrp(double epsilon){
    STreap* t = (STreap*) malloc(sizeof(STreap));
    if(t == NULL){
        printf("Erro ao alocar memória para a árvore STreap.\n");
        return NULL;
    }

    t->raiz    = NULL;
    t->epsilon = epsilon;

    return t;
}
// Falta implementar a função de inserção
Node* insertSTrp(STreap* t, double x, double y,  Info* info){
    if(t == NULL || info == NULL){
        printf("Árvore ou informação inválida para inserção.\n");
        return NULL;
    }

    // Criar o novo nó
    Node* novoNode = (Node*) malloc(sizeof(Node));
    if(novoNode == NULL){
        printf("Erro ao alocar memória para o novo nó.\n");
        return NULL;
    }

    // Inicializar o novo nó
    novoNode->x = x;
    novoNode->y = y;
    novoNode->info = info;
    novoNode->prioridade = rand(); // Atribuir uma prioridade aleatória
    novoNode->mbbX1 = x; // Inicialmente, o bounding box é o próprio ponto
    novoNode->mbbY1 = y; 
    novoNode->mbbX2 = x;
    novoNode->mbbY2 = y;
    novoNode->esq = NULL;
    novoNode->dir = NULL;

    // Inserir o nó na árvore
}




/*###############################################################################################*/
