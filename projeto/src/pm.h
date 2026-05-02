#ifndef _PM_H
#define _PM_H

typedef struct parametro Param;

#include "params.h"
#include "hashPM.h"
#include "hashTable.h"

/**
 * Este módulo é responsável por processar o arquivo .pm, 
 * que contém informações sobre os habitantes de uma cidade, incluindo seus dados pessoais e suas moradias. 
 * 
 * O módulo inclui funções para ler o arquivo .pm, processar os dados contidos nele, 
 * e armazenar as informações relevantes em uma estrutura de dados adequada (Tabela Hash e Pessoas).
 */

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta é uma função auxiliar de processarPM. Ela é responsável por montar o caminho completo do arquivo .pm 
 * a partir do diretório de entrada e do nome do arquivo .pm fornecidos como parâmetros.
 * 
 * O caminho completo do arquivo .pm é armazenado no buffer caminhoPM, 
 * que deve ser alocado pelo chamador da função processarPM.
 * 
 * @param param     Ponteiro para a estrutura de parâmetros
 * @param caminhoPM Buffer para armazenar o caminho completo do arquivo .pm
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoPM(Param* param, char* caminhoPM);
/**
 * Esta é uma função auxiliar de processarPM. Ela é responsável por ler o arquivo .pm linha por linha,
 * processar os comandos e os dados contidos em cada linha, 
 * e armazenar as informações relevantes em uma estrutura de dados adequada (Tabela Hash e Pessoas).
 * 
 * @param arquivoPM Ponteiro para o arquivo .pm aberto
 * @param dir       Ponteiro para o diretório da tabela hash onde os dados do arquivo .pm serão armazenados
 * @param p         Ponteiro para a estrutura de Pessoas onde os dados do arquivo .pm serão armazenados
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int readFilePM(FILE* arquivoPM, hashPM* dir, TabelaHash* htq, Pessoas* p);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta é uma função wrapper para o processamento do arquivo .pm 
 * Ela é responsável por coordenar as etapas de processamento do arquivo .pm,
 * incluindo a montagem do caminho completo do arquivo .pm, a abertura do arquivo para leitura, 
 * a verificação da estrutura de dados para armazenar os dados do arquivo .pm, 
 * a leitura e processamento dos dados do arquivo .pm, e o fechamento do arquivo após o processamento.
 * 
 * @param param Ponteiro para a estrutura de parâmetros
 * @param h     Ponteiro para o diretório da tabela hash onde os dados do arquivo .pm serão armazenados
 * @param p     Ponteiro para a estrutura de Pessoas onde os dados do arquivo .pm serão armazenados
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int processarPM(Param* param, hashPM* h, TabelaHash* htq, Pessoas* p);
/*###############################################################################################*/


#endif