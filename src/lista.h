#ifndef _Lista_H
#define _Lista_H

typedef struct Lista Lista;
typedef struct cel   Cel;

/**
 * Cria uma lista encadeada vazia
 * @return Ponteiro para a lista criada. NULL, se não conseguiu criar a lista
 */
Lista* criaLista();

/**
 * Insere um item v no final da lista
 * @param lista A lista onde o item será inserido
 * @param v O item a ser inserido
 * @return Void
 */
void inserirFim(Lista* lista, void* v);

/**
 * Remove o item do início da lista
 * @param lista A lista onde o item será removido
 * @return Void
 */
void removerInicio(Lista* lista);

/**
 * Libera todos os recursos usados pela lista
 * @param lista A lista a ser liberada
 * @return Void
 */
void limparLista(Lista* lista);

#endif