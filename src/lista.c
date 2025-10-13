#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"

/*                          ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                          */
// Estrutura para a lista encadeada
typedef struct lista{
    Celula* inicio;
    Celula* fim;
    int tamanho;
}Lista;
// Estrutura para a célula da lista
typedef struct cel{
    void* item;
    struct cel* prox;
    struct cel* ant;
}Celula;
/*#############################################################################################*/



/*                                      FUNÇÕES PRINCIPAIS                                     */
// Função para criar uma lista encadeada vazia
Lista* criarLista(){
    Lista* l = (Lista*)malloc(sizeof(Lista));

    l->inicio  = NULL;
    l->fim     = NULL;
    l->tamanho = 0;

    return l;
}
// Função para inserir um item no final da lista
void inserirFim(Lista* lista, void* v){
    Celula* nova = malloc(sizeof(Celula));
    nova->item = v;

    if (lista->inicio == NULL){
        lista->inicio = nova;
        lista->fim    = nova;

        nova->prox = NULL;
        nova->ant  = NULL;
    }else{
        nova->prox = NULL;
        lista->fim->prox = nova;
        nova->ant        = lista->fim;
        lista->fim       = nova;
    }

    lista->tamanho += 1;
}
// Função para remover o item do início da lista
void removerInicio(Lista* lista){
    Celula* cel = lista->inicio;

    lista->inicio = lista->inicio->prox;
    if(lista->inicio == NULL) lista->fim = NULL; // Lista ficará vazia
    else                      lista->inicio->ant = NULL;
    
    free(cel);
    lista->tamanho -= 1;
}
// Função para limpar a lista e desalocar todos os recursos
void limparLista(Lista* lista) {
    if (!lista) return;

    Celula* atual = lista->inicio;
    while (atual != NULL) {
        Celula* temp = atual;
        atual = atual->prox;

        free(temp->item); // Libera o item armazenado
        free(temp);       // Libera a célula
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;

    free(lista);
}
/*#############################################################################################*/
