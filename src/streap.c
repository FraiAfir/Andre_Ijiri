// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #include "streap.h"

// /*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
// // Estruturas para a árvore treap espacial
// typedef struct streap{
//     Node*  raiz;     // Raiz da árvore
//     double epsilon;  // Precisão para comparar números reais
// }STreap;
// // Estruturas para a informação, nó e lista de nós
// typedef struct info{
//     char*  descricao; // Descrição da informação
//     double valor;     // Valor associado à informação
// }Info;
// // Nó da árvore
// typedef struct node{
//     double x, y;       // Coordenadas da âncora
//     int    prioridade; // Prioridade do nó (para manter a propriedade de heap da treap)
//     Info*  info;       // Informação associada ao nó

//     double mbbX1, mbbY1, mbbX2, mbbY2; // Bounding box do nó (retângulo envolvente)

//     Node*  esq; // Filho esquerdo
//     Node*  dir; // Filho direito
// }Node;
// // Lista de nós
// typedef struct lista{
//     Node** nos;        // Array de nós
//     int    tamanho;    // Tamanho atual da lista
//     int    capacidade; // Capacidade máxima da lista
// }Lista;
// /*###############################################################################################*/



// /*                                       FUNÇÕES PRINCIPAIS                                      */
// STreap* createSTrp(double epsilon){
//     STreap* t = (STreap*) malloc(sizeof(STreap));
//     if(t == NULL){
//         printf("Erro ao alocar memória para a árvore STreap.\n");
//         return NULL;
//     }

//     t->raiz    = NULL;
//     t->epsilon = epsilon;

//     return t;
// }
// Node* insertSTrp(STreap* t, double x, double y,  Info* info){
//     if(t == NULL || info == NULL){
//         printf("Árvore ou informação inválida para inserção.\n");
//         return NULL;
//     }

//     // Criar o novo nó
//     Node* novoNode = (Node*) malloc(sizeof(Node));
//     if(novoNode == NULL){
//         printf("Erro ao alocar memória para o novo nó.\n");
//         return NULL;
//     }

//     // Inicializar o novo nó
//     novoNode->x = x;
//     novoNode->y = y;
//     novoNode->info = info;
//     novoNode->prioridade = rand(); // Atribuir uma prioridade aleatória
//     novoNode->mbbX1 = x; // Inicialmente, o bounding box é o próprio ponto
//     novoNode->mbbY1 = y; 
//     novoNode->mbbX2 = x;
//     novoNode->mbbY2 = y;
//     novoNode->esq = NULL;
//     novoNode->dir = NULL;

//     // Inserir o nó na árvore
//     // Falta fazer
// }
// void getNodeRegiaoSTrp(STreap* t, double x, double y, double w, double h, Lista* resultado){
//     if(t == NULL || resultado == NULL){
//         printf("Árvore ou lista de resultado inválida para consulta de região.\n");
//         return;
//     }

//     // Definir a região de busca
//     double x1 = x;
//     double y1 = y;
//     double x2 = x + w;
//     double y2 = y + h;

//     // Percorrer a árvore e buscar os nós na região especificada no retângulo (x,y) - (x+w,y+h)
//     // Falta fazer
// }
// Info* getInfoSTrp(STreap* t, Node* n){
//     if(t == NULL || n == NULL){
//         printf("Árvore ou nó inválido para obtenção de informação.\n");
//         return NULL;
//     }
    
//     return n->info;
// }
// Node* getNodeSTrp(STreap* t, double xa, double ya){
//     if(t == NULL){
//         printf("Árvore inválida para busca de nó.\n");
//         return NULL;
//     }

//     // Buscar o nó com âncora (xa, ya)
//     Node* aux = t->raiz;    
//     while(aux != NULL){

//         double x = fabs(aux->x - xa);
//         double y = fabs(aux->y - ya);
//         double erro = t->epsilon;

//         if     (x < erro && y < erro)                     return aux;     // Nó encontrado
//         else if(xa < aux->x || (x < erro && ya < aux->y)) aux = aux->esq; // Ir para o filho esquerdo
//         else                                              aux = aux->dir; // Ir para o filho direito
//     }

//     // Nó não encontrado
//     return NULL;
// }


// void killSTrp(STreap* t){
//     if(t == NULL) return;
    
//     killNode(t->raiz);
//     free(t);
// }
// void killNode(Node* n){
//     if(n == NULL) return;

//     killNode(n->esq);
//     killNode(n->dir);

//     // Liberar a informação associada ao nó
//     if(n->info != NULL){
//         free(n->info->descricao);
//         free(n->info);
//     }free(n);
// }
// /*###############################################################################################*/



// /*                                       FUNÇÕES SECUNDÁRIAS                                     */
// void rotateRight(STreap* t, Node** pRoot){
//     Node* child = (*pRoot)->esq;
//     Node* aux   = child->dir;

//     child->dir    = *pRoot;
//     (*pRoot)->esq = aux;

//     *pRoot = child;
// }
// void rotateLeft(STreap* t, Node** pRoot){
//     Node* child = (*pRoot)->dir;
//     Node* aux   = child->esq;

//     child->esq    = *pRoot;
//     (*pRoot)->dir = aux;

//     *pRoot = child;
// }
// /*###############################################################################################*/