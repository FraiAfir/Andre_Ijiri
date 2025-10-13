#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "streap.h"

/*                          ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                          */
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
/*#############################################################################################*/

/*                                      FUNÇÕES AUXILIARES                                     */
/*#############################################################################################*/

/*                                      FUNÇÕES PRINCIPAIS                                     */
/*#############################################################################################*/
