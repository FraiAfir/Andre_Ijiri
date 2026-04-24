#ifndef _GEO_H
#define _GEO_H

#include "hashTable.h"

typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .geo
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoGeo Buffer para armazenar o caminho completo do arquivo .geo
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoGeo(Param* param, char* caminhoGeo);
/**
 * Função para ler e processar os dados do arquivo .geo
 * @param arquivoGeo Ponteiro para o arquivo .geo aberto
 * @param dir        Ponteiro para a tabela hash onde os dados do arquivo .geo serão armazenados
 * @param q          Ponteiro para o objeto que armazenará os dados lidos do arquivo .geo
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int readFileGeo(FILE* arquivoGeo, TabelaHash* dir, Quadras* q);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .geo
 * @param param Ponteiro para a estrutura de parâmetros
 * @param h     Ponteiro para a tabela hash que armazenará os dados do arquivo .geo
 * @param q     Ponteiro para o objeto que armazenará os dados lidos do arquivo .geo
 * @return 0 em caso de sucesso. -1 em caso de erro
 */
int processarGeo(Param* param, TabelaHash* h, Quadras* q);
/*###############################################################################################*/

#endif