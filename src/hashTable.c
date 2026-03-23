#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashTable.h"

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct HashNode{
    char* key;
    char* value;
    struct HashNode *next;
} HashNode;
typedef struct HashTable{
    HashNode** buckets;
    int tamanho;
} HashTable;
/*###############################################################################################*/


/*************************************** FUNÇÕES AUXILIARES ***************************************/
unsigned int hashFunction(const char* key){
    // 1: Inicializa o hash com um valor inicial
    unsigned int hash = 0; 

    // 2: Percorre cada caractere da chave e atualiza o hash usando uma fórmula simples
    while (*key){
        // 2.1: Multiplica o hash por um número primo e adiciona o valor do caractere
        hash = (hash * 31) + (*key++); 
    }

    // 3: Retorna o índice do bucket correspondente
    return hash % HASH_SIZE; 
}

bool existeKey(HashTable* table, const char* key){
    // 1: Verifica se a tabela hash ou a chave são nulas
    if (table == NULL || key == NULL) return false;

    // 2: Calcula o índice do bucket usando a função de hash
    unsigned int index = hashFunction(key); 

    // 3: Obtém o ponteiro para o primeiro nó do bucket correspondente
    HashNode* current = table->buckets[index]; 

    // 4: Percorre a lista encadeada do bucket para verificar se a chave existe
    while (current != NULL){
        // 4.1: Compara a chave do nó atual com a chave fornecida
        if (strcmp(current->key, key) == 0){
            // Chave encontrada
            return true; 
        }

        // 4.2: Move para o próximo nó na lista encadeada
        current = current->next; 
    }
    // Chave não encontrada
    return false; 
}

HashNode* getRegistro(HashTable* table, const char* key){
    // 1: Verifica se a tabela hash ou a chave são nulas
    if (table == NULL || key == NULL) return NULL;

    // 2: Verifica se a chave existe na tabela hash antes de tentar obter o registro
    if (!existeKey(table, key)) return NULL;

    // 3: Calcula o índice do bucket usando a função de hash
    unsigned int index = hashFunction(key);

    // 4: Obtém o ponteiro para o primeiro nó do bucket correspondente
    HashNode* current = table->buckets[index];

    // 5: Percorre a lista encadeada do bucket para encontrar o nó associado à chave
    while (current != NULL){
        // 5.1: Compara a chave do nó atual com a chave fornecida
        if (strcmp(current->key, key) == 0){
            // Retorna o nó associado à chave
            return current; 
        }
        // 5.2: Move para o próximo nó na lista encadeada
        current = current->next; 
    }

    // Chave não encontrada (Isso não deve acontecer devido à verificação anterior)
    return NULL;
}

char* getValue(HashTable* table, const char* key){
    // 1: Verifica se a tabela hash ou a chave são nulas
    if (table == NULL || key == NULL) return NULL;

    // 2: Obtém o nó associado à chave usando a função getRegistro
    HashNode* node = getRegistro(table, key);

    // 3: Se o nó existir, retorna o valor associado. Caso contrário, retorna NULL
    return (node != NULL) ? node->value : NULL; 
}

int getTamanho(HashTable* table){
    // 1: Verifica se a tabela hash é nula
    if (table == NULL) return -1;

    // 2: Retorna o número de buckets na tabela hash
    return table->tamanho; 
}
/*###############################################################################################*/

/*                                       FUNÇÕES PRINCIPAIS                                      */
HashTable* criarTabela(int entradas){
    // 1: Verifica se o número de entradas é válido (maior que zero)
    if (entradas <= 0) return NULL;

    // 2: Aloca a estrutura principal
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) return NULL;

    // 3: Aloca o array de buckets dinamicamente com base nas "entradas"
    table->buckets = (HashNode**)malloc(entradas * sizeof(HashNode*));
    if (table->buckets == NULL){
        printf("ERRO: Alocar memoria para os buckets da tabela hash.\n");
        free(table);
        return NULL;
    }

    // 4: Inicializa todos os buckets com NULL e guarda o tamanho
    table->tamanho = entradas;
    for (int i = 0; i < entradas; i++) table->buckets[i] = NULL;

    // 5: Retorna o ponteiro para a tabela hash criada
    return table;
}

int freeHash(HashTable* table){
    // 1: Verifica se a tabela hash é nula
    if (table == NULL) return -1;

    // 2: Percorre cada bucket da tabela hash e libera a memória alocada para os nós
    for (int i = 0; i < HASH_SIZE; i++){
        // 2.1: Obtém o ponteiro para o primeiro nó do bucket atual
        HashNode* current = table->buckets[i]; 

        // 2.2: Percorre a lista encadeada do bucket e libera a memória alocada para cada nó
        while (current != NULL){
            // 2.2.1: Armazena o ponteiro para o nó atual antes de mover para o próximo nó
            HashNode* temp = current; 

            // 2.2.2: Move para o próximo nó antes de liberar a memória
            current = current->next; 

            // 2.2.3: Libera a memória alocada para a chave, valor e o nó em si
            free(temp->key);   // Libera a memória alocada para a chave
            free(temp->value); // Libera a memória alocada para o valor
            free(temp);        // Libera o nó em si
        }
    }
    // 3: Libera a tabela hash
    free(table); 

    return 0;
}

int inserirReg(HashTable* table, const char* key, const char* value){
    // 1: Verifica se a tabela hash, a chave ou o valor são nulos
    if (table == NULL || key == NULL || value == NULL) return -1;

    // 2: Calcula o índice do bucket usando a função de hash
    unsigned int index = hashFunction(key);

    // 3: Obtém o ponteiro para o primeiro nó do bucket correspondente
    HashNode* current = table->buckets[index];

    // 4: Verifica se a chave já existe na tabela hash
    while (current != NULL){
        // 4.1: Compara a chave do nó atual com a chave fornecida
        if (strcmp(current->key, key) == 0){
            // 4.1.1: Chave já existe, atualiza o valor associado
            free(current->value);           // 4.1.2: Libera a memória do valor antigo
            current->value = strdup(value); // 4.1.3: Aloca nova memória para o novo valor
            return 0; // Sucesso
        }
        // 4.2: Move para o próximo nó na lista encadeada
        current = current->next; 
    }

    // 5: Chave não existe, cria um novo nó e insere na tabela hash
    // 5.1: Aloca memória para o novo nó
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode)); 
    // 5.2: Verifica se a alocação do novo nó foi bem-sucedida
    if (newNode == NULL){
        printf("ERRO: Alocar memoria para o novo no.\n");
        return -1;
    }
    newNode->key   = strdup(key);           // 5.3: Aloca memória para a chave e copia o valor
    newNode->value = strdup(value);         // 5.4: Aloca memória para o valor e copia o valor
    newNode->next  = table->buckets[index]; // 5.5: Insere o novo nó no início da lista encadeada do bucket
    table->buckets[index] = newNode;        // 5.6: Atualiza o ponteiro do bucket para apontar para o novo nó

    return 0; // Sucesso
}

int removerKey(HashTable* table, const char* key){
    // 1: Verifica se a tabela hash ou a chave são nulas
    if (table == NULL || key == NULL) return -1;

    // 2: Calcula o índice do bucket usando a função de hash
    unsigned int index = hashFunction(key);

    // 3: Obtém o ponteiro para o primeiro nó do bucket correspondente
    HashNode* current = table->buckets[index];

    // Ponteiro para o nó anterior, necessário para remover o nó da lista encadeada
    HashNode* prev = NULL; 

    // 4: Percorre a lista encadeada do bucket para encontrar o nó associado à chave
    while (current != NULL){
        // 4.1: Compara a chave do nó atual com a chave fornecida
        if (strcmp(current->key, key) == 0){ // Chave encontrada, remove o nó da lista encadeada            
            // 4.1.1: O nó a ser removido é o primeiro nó do bucket
            if (prev == NULL) table->buckets[index] = current->next;
            
            // 4.1.2: O nó a ser removido está no meio ou no final da lista
            else prev->next = current->next;
            
            // 4.1.3: Libera a memória alocada para a chave, valor e o nó em si
            free(current->key);   // Libera a memória alocada para a chave
            free(current->value); // Libera a memória alocada para o valor
            free(current);        // Libera o nó em si
            return 0; // Sucesso
        }
        prev    = current;       // 4.2: Atualiza o ponteiro do nó anterior
        current = current->next; // 4.3: Move para o próximo nó na lista encadeada
    }
    // Chave não encontrada
    return -1; 
}