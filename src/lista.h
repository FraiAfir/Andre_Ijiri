#ifndef _Lista_h_
#define _Lista_h_

/*
                                        DEFINIÇÃO
    Uma lista é uma estrutura de dados que organiza itens em uma sequência, 
mantendo uma ordem específica e permitindo a adição e remoção de elementos. 
    Ela pode ser implementada de forma contígua 
(como em vetores/arrays, onde os elementos estão em memória fisicamente próximos) ou de forma encadeada 
(onde cada elemento, chamado de nó, contém dados e um ponteiro para o próximo).

                                CARACTERÍSTICAS PRINCIPAIS
    Ordem: Os elementos de uma lista têm uma relação de ordem entre si, seja física ou lógica. 
    Nós (ou Nodos): Na lista encadeada, cada item é um nó que armazena o dado e um ponteiro para o próximo nó.
    Flexibilidade: Listas permitem o crescimento e o encolhimento de acordo com a necessidade da aplicação. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct lista Lista;
typedef struct cel Celula;

/**
 * Cria uma nova Lista vazia
 * @return Ponteiro para uma nova Lista
 */
Lista* criaLista();

/**
 * Busca um elemento dentro da Lista
 * @param lista Ponteiro para a Lista que deseja encontrar o elemento
 * @param p     Posição do elemento que está procurando
 * @return      Ponteiro para o elemento (se existir), NULL se ele não estiver na Lista
 * @note        A Lista deve ter sido criada antes da chamada desta função
 */
Celula* buscarElemento(Lista* lista, int p);

/**
 * Insere um elemento no final da Lista
 * @param lista Ponteiro para a Lista que deseja inserir o elemento
 * @param v     Valor do elemento que deseja inserir
 * @return      0 se sucesso, -1 se erro
 * @note        A Lista deve ter sido criada antes da chamada desta função
 */
int inserirFim(Lista* lista, int v);

/**
 * Remove elemento do meio da Lista
 * @param lista Ponteiro para a Lista que deseja remover o elemento
 * @param p     Posição do elemento que deseja remover
 * @return      0 se sucesso, -1 se erro
 * @note        A Lista deve ter sido criada antes da chamada desta função
 * @note        Esta função pode ser utilizada para remover elementos do início/fim da Lista também
 */
int removerMeio(Lista* lista, int p);

/**
 * Imprime a Lista
 * @param lista Ponteiro para a Lista que deseja imprimir
 * @return      Não há retorno para esta função
 * @note        A Lista deve ter sido criada antes da chamada desta função
 */
void imprimirLista(Lista* lista);

/**
 * Limpa a Lista
 * @param lista Ponteiro para a Lista que você deseja limpar
 * @return      0 se sucesso, -1 se erro
 * @note        A Lista deve ter sido criada antes da chamada desta função
 */
int limparLista(Lista* lista);

#endif