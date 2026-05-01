#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <stdbool.h>

#include "pm.h"
#include "geo.h"
#include "qry.h"
#include "params.h"
#include "hashTable.h"
#include "hashPM.h"

/*
    O programa é responsável por inicializar as estruturas de dados necessárias para a execução do código,
    assim como liberar os recursos alocados ao final da execução do programa.

    As principais funções do programa incluem:
    - bootProgram: Inicializa os objetos de Parametro e da TabelaHash para armazenar os dados do arquivo .geo
    - shutProgram: Libera a memória alocada para os objetos de Parametro e da TabelaHash
*/

/*                                       FUNÇÕES AUXILIARES                                      */
// /**
//  * Função de comparação entre as informações dos nós da árvore. Deve ser implementada de acordo com o tipo de informação armazenada nos nós da árvore
//  * @param info1 Informação a ser inserida
//  * @param info2 Informação do nó atual da árvore
//  * @return      true se info1 for menor que info2, false caso contrário
//  */
// bool cmpFuncGeo(void* info1, void* info2);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função é responsável por inicializar os objetos de Parametro e da TabelaHash para armazenar os dados do arquivo .geo
 * @param param Ponteiro para o objeto criado de Parametro
 * @param ht    Ponteiro para o objeto criado da TabelaHash
 * @param q     Ponteiro para o objeto criado para armazenar os dados lidos do arquivo .geo
 * @param hp    Ponteiro para o objeto criado da TabelaHash para armazenar os dados do arquivo .pm
 * @param p     Ponteiro para o objeto criado para armazenar os dados lidos do arquivo .pm
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int bootProgram(Param** param, TabelaHash** ht, Quadras** q, hashPM** hp, Pessoas** p);
/**
 * Esta função é responsável por liberar a memória alocada para os objetos de Parametro e da Tabela Hash
 * @param param Ponteiro para o objeto de Parametro
 * @param ht    Ponteiro para o objeto da TabelaHash
 * @param q     Ponteiro para o objeto de Quadras
 * @param hp    Ponteiro para o objeto da TabelaHash para armazenar os dados do arquivo .pm
 * @param p     Ponteiro para o objeto de Pessoas
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int shutProgram(Param** param, TabelaHash** ht, Quadras** q, hashPM** hp, Pessoas** p);

/*###############################################################################################*/

#endif