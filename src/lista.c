#include "lista.h"

typedef struct cel{
    int         valor;
    struct cel* prox;
}Celula;

struct lista{
    Celula* prim;
    Celula* ulti;
    int tamanho;
};

Lista* criaLista(){
    Lista* l = malloc(sizeof(Lista));

    l->prim  = NULL;
    l->ulti  = NULL;
    l->tamanho = 0;
    
    // printf("\nLista criada!");
    return l;
}

int inserirFim(Lista *lista, int v){
    Celula* aux  = lista->prim;
    Celula *nova = malloc(sizeof(Celula));
    nova->valor  = v;

    if(lista->prim == NULL){ // A Lista está vazia
        nova->prox  = lista->prim;
        lista->prim = nova;
    }else{
        while(aux->prox != NULL)
            aux = aux->prox;
        nova->prox  = NULL;
        aux->prox   = nova;
        lista->ulti = nova;
    }

    lista->tamanho++;
    printf("\nYou've collected the item");
    Sleep(1500);
    system("cls");
    
    
    return 0;
}

int removerMeio(Lista* lista, int p){
    Celula* ant = NULL;
    Celula* del = lista->prim;
    
    int i = 1;
    while(del != NULL && i != p){
        ant = del;
        del = del->prox;
        i++;
    }
    
    if(del == NULL){
        printf("\nCelula nao encontrada");
        return 0;
    }
    
    if(i == 1){
        lista->prim = del->prox;
        free(del);
    }else{
        ant->prox = del->prox;
        free(del);
    }
        
    lista->tamanho--;
    return 0;
}

Celula* buscarElemento(Lista* lista, int p){
    Celula* cel = lista->prim;

    int i = 1;
    while(i != p && cel != NULL){
        cel = cel->prox;
        i++;
    }

    return cel;
}

void imprimirLista(Lista* lista){
    Celula* aux = lista->prim;

    printf("\n");
    int i = 1;
    while(aux != NULL){
        switch(aux->valor){
            case 1:  printf("%d - Health Potion\n", i, aux->valor);      i++; break;
            case 2:  printf("%d - Monster's Repelent\n", i, aux->valor); i++; break;
            case 3:  printf("%d - Treasure Chest\n", i, aux->valor);     i++; break;
            default: printf("%d - ITEM ERROR\n", i, aux->valor);         i++; break;
        }
        aux = aux->prox;
    }
}

int getValor(Celula* cel){
    return cel->valor;
}

void limparLista(Lista* l){
    Celula*   del = l->prim;
    Celula* atual = NULL;
    
    while(del != NULL){
        atual = del;
        del   = del->prox;
        free(atual);
    }free(del);
}