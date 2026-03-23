#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 1009

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct HashNode {
    char* key;
    char* value;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode* buckets[HASH_SIZE];
} HashTable;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para verificar se uma chave existe na tabela hash
 * @param table Ponteiro para a tabela hash
 * @param key   Chave a ser verificada
 * @return      TRUE se a chave existir na tabela hash. FALSE caso contrário
 */
bool existeKey(HashTable* table, const char* key);
/**
 * Função para obter o registro associado a uma chave na tabela hash
 * @param table Ponteiro para a tabela hash
 * @param key   Chave a ser verificada
 * @return      Ponteiro para o nó associado à chave. NULL se a chave não existir
 * 
 * @note        Esta função utiliza a função existeKey para verificar 
 * se a chave existe antes de tentar obter o registro.
 * 
 * @note        Se a chave existir, a função deve retornar um ponteiro para o nó associado à chave. 
 * Caso contrário, deve retornar NULL
 */
HashNode* getRegistro(HashTable* table, const char* key);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para criar uma tabela hash
 * @param entradas Número de entradas (buckets) na tabela hash
 * @return         Ponteiro para a tabela hash criada. NULL em caso de erro
 * 
 * @note           A tabela hash deve ser alocada dinamicamente e inicializada com os buckets vazios
 * 
 * @note           O número de entradas deve ser maior que zero. 
 * Caso contrário, a função deve retornar NULL
 */
HashTable* criarTabela(int entradas);
/**
 * Função para liberar a memória alocada para a tabela hash e seus nós
 * @param table Ponteiro para a tabela hash a ser liberada
 * @return      0 em caso de sucesso, -1 em caso de erro
 */
int freeHash(HashTable* table);
/**
 * Função para inserir um registro (chave-valor) na tabela hash
 * @param table Ponteiro para a tabela hash
 * @param key   Chave do registro a ser inserido
 * @param value Valor do registro a ser inserido
 * @return      0 em caso de sucesso. -1 em caso de erro

 * @note        Se a chave não existir, um novo nó deve ser criado e inserido na tabela hash
 * 
 * @note        Se a chave já existir na tabela hash, 
 * o valor associado deve ser atualizado com o novo valor fornecido
 */
int inserirReg(HashTable* table, const char* key, const char* value);
/**
 * Função para remover um registro (chave-valor) da tabela hash
 * @param table Ponteiro para a tabela hash
 * @param key   Chave do registro a ser removido
 * @return      0 em caso de sucesso. -1 em caso de erro
 * 
 * @note        Deve existir um registro associado à chave fornecida para que a remoção seja bem-sucedida.
 * Caso contrário, a função deve retornar -1, o nó associado deve ser removido da tabela hash e a memória 
 * alocada para o nó deve ser liberada
 */
int removerKey(HashTable* table, const char* key);
/*###############################################################################################*/

#endif