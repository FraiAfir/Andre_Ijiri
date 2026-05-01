#ifndef _PM_H
#define _PM_H

typedef struct parametro Param;

#include "hashPM.h"

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .pm
 * @param param     Ponteiro para a estrutura de parâmetros
 * @param caminhoPM Buffer para armazenar o caminho completo do arquivo .pm
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoPM(Param* param, char* caminhoPM);
/**
 * Função para ler e processar os dados do arquivo .pm
 * @param arquivoPM Ponteiro para o arquivo .pm aberto
 * @param dir       Ponteiro para o diretório da tabela hash onde os dados do arquivo .pm serão armazenados
 * @param p         Ponteiro para a estrutura de Pessoas onde os dados do arquivo .pm serão armazenados
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int readFilePM(FILE* arquivoPM, hashPM* dir, Pessoas* p);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .pm
 * @param param Ponteiro para a estrutura de parâmetros
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int processarPM(Param* param, hashPM* h, Pessoas* p);
/*###############################################################################################*/


#endif