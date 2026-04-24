#ifndef _HASH_H
#define _HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct quadras Quadras;
typedef struct bucket Bucket;
typedef struct tabelaHash TabelaHash;

/*
    A tabela hash é uma estrutura de dados que permite o armazenamento e a recuperação eficiente de pares chave:valor.
    Ela utiliza uma função de hash para mapear as chaves a índices em um array, onde os valores correspondentes são armazenados.
    As principais operações suportadas pela tabela hash incluem a criação da tabela, inserção de registros, remoção de chaves, 
    busca por valores associados a chaves, entre outras.
    Esta implementação específica da tabela hash é projetada para armazenar registros do tipo "Quadras", 
    que contêm informações como CEP, coordenadas, dimensões e cores.
*/

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função é responsável por calcular o índice do bucket correspondente a uma chave fornecida.
 * Ela utiliza a função de hash DJB2 para mapear a chave a um índice dentro do tamanho do diretório da tabela hash.

 * A função de hash DJB2 é conhecida por ser simples e eficiente, e é amplamente utilizada em implementações de tabelas hash.
 * A escolha de 5381 como valor inicial é baseada em experimentos e análises que mostraram que ele proporciona uma boa distribuição de chaves para muitas entradas comuns.
 * A função de hash DJB2 é definida como: hash(i) = hash(i - 1) * 33 + c, onde hash(0) é o valor inicial (5381) e c é o valor ASCII do caractere atual.
 * O número 33 é escolhido porque é um número primo que ajuda a distribuir as chaves de forma mais uniforme na tabela hash, reduzindo a probabilidade de colisões.
 * 
 * @param key Chave a ser mapeada para um índice de bucket na tabela hash (string: CEP)
 * @return    Índice do bucket correspondente à chave fornecida
 */
unsigned int hashFunc(const char* key);
/**
 * Função para verificar se uma chave existe na tabela hash
 * @param table Ponteiro para a tabela hash
 * @param key   Chave a ser verificada
 * @return      TRUE se a chave existir na tabela hash. FALSE caso contrário
 */
bool existeKey(TabelaHash* table, const char* key);
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
Quadras* getRegistro(TabelaHash* table, const char* key);
/**
 * Função para buscar o valor associado a uma chave na tabela hash
 * @param table Ponteiro para a tabela hash
 * @param key   Chave a ser buscada
 * @return      Ponteiro para o valor associado à chave. NULL se a chave não existir
 */
char* getValue(TabelaHash* table, const char* key);
/**
 * Função para obter o tamanho da tabela hash (número de buckets)
 * @param table Ponteiro para a tabela hash
 * @return      Número de buckets na tabela hash
 */
int getTamanho(TabelaHash* table);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função é responsável por criar um novo arquivo binário e inicializa o Diretório na RAM com os endereços dos buckets no arquivo.
 * 
 * @param nomeArquivo Nome do arquivo binário a ser criado
 * @return            Ponteiro para a tabela hash criada. NULL em caso de erro
 */
TabelaHash* criarHash(const char* nomeArquivo);
/**
 * Esta função é responsável por criar um novo registro do tipo Quadras, inicializando os campos como nulo.
 * @return Ponteiro para o novo registro criado. NULL em caso de erro
 */
Quadras* criarQuadra();
/**
 * Esta função é responsável por liberar a memória alocada para a tabela hash e fechar o arquivo binário associado.
 * Fecha o arquivo e limpa o Diretório da RAM, liberando a memória alocada para os endereços dos buckets.
 * 
 * @param dir Ponteiro para o diretório da tabela hash a ser fechado
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
void freeHash(TabelaHash* dir);
/**
 * Esta função é responsável por inserir um novo registro do tipo Quadras na tabela hash,
 * utilizando a chave (CEP) para determinar o bucket onde o registro deve ser armazenado.
 * A função deve lidar com colisões utilizando o método de hashing estendido, onde cada bucket pode armazenar múltiplos registros e, 
 * em caso de colisão, o bucket é dividido e os registros são redistribuídos entre os buckets.
 * 
 * @param dir        Ponteiro para o diretório da tabela hash
 * @param cep        Chave do registro a ser inserido (CEP)
 * @param x          Coordenada x da quadra
 * @param y          Coordenada y da quadra
 * @param w          Largura da quadra
 * @param h          Altura da quadra
 * @param sw         Largura da borda
 * @param cfill      Cor de preenchimento
 * @param cstrk      Cor da borda
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int inserirReg(TabelaHash* dir, char* cep, double x, double y, double w, double h, double sw, char* cfill, char* cstrk);
/**
 * Esta função é responsável por remover um registro da tabela hash, utilizando a chave (CEP) para determinar o bucket onde o registro deve ser removido.
 * 
 * @param table Ponteiro para a tabela hash
 * @param key   Chave do registro a ser removido
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int removerKey(TabelaHash* table, const char* key);
/*###############################################################################################*/

#endif