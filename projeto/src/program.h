#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <stdbool.h>

#include "pm.h"
#include "geo.h"
#include "qry.h"
#include "params.h"
#include "hashTable.h"
#include "hashPM.h"

/**
 * Este módulo é responsável por implementar as funções de inicialização e encerramento do programa.
 * 
 * As funções de inicialização e encerramento são responsáveis por alocar e liberar os recursos necessários para a execução do programa, 
 * como os objetos de Parametro, as estruturas de dados para armazenar os dados dos arquivos .geo e .pm, 
 * e os objetos para armazenar os dados lidos desses arquivos. 
 */

/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função é responsável por inicializar os objetos de Parametro e da TabelaHash para armazenar os dados do arquivo .geo,
 * bem como os objetos para armazenar os dados lidos do arquivo .geo e do arquivo .pm, caso este seja fornecido como argumento.
 * 
 * @param param Ponteiro para o objeto criado de Parametro
 * @param ht    Ponteiro para o objeto criado da TabelaHash
 * @param q     Ponteiro para o objeto criado para armazenar os dados lidos do arquivo .geo
 * @param hp    Ponteiro para o objeto criado da TabelaHash para armazenar os dados do arquivo .pm
 * @param p     Ponteiro para o objeto criado para armazenar os dados lidos do arquivo .pm
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int bootProgram(Param** param, TabelaHash** ht, Quadras** q, hashPM** hp, Pessoas** p);
/**
 * Esta função é responsável por liberar a memória alocada para os objetos de Parametro e da TabelaHash, 
 * bem como para os objetos utilizados para armazenar os dados lidos do arquivo .geo e do arquivo .pm, caso este seja fornecido como argumento.
 * 
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