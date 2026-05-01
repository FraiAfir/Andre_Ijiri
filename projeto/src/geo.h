#ifndef _GEO_H
#define _GEO_H

#include "hashTable.h"

typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função monta o caminho completo do arquivo .geo a partir do diretório de entrada 
 * e do nome do arquivo .geo fornecidos nos parâmetros da linha de comando, e armazena o caminho completo no buffer fornecido.
 * 
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoGeo Buffer para armazenar o caminho completo do arquivo .geo
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoGeo(Param* param, char caminhoGeo[512]);
/**
 * Esta função lê o arquivo .geo linha por linha, 
 * processa os comandos encontrados e armazena os dados em uma estrutura de dados adequada (Tabela Hash e Quadras).
 * 
 * @param arquivoGeo Ponteiro para o arquivo .geo aberto
 * @param dir        Ponteiro para a tabela hash onde os dados do arquivo .geo serão armazenados
 * @param q          Ponteiro para o objeto que armazenará os dados lidos do arquivo .geo
 * @param param      Ponteiro para a estrutura de parâmetros
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int readFileGeo(FILE* arquivoGeo, TabelaHash* dir, Quadras* q, Param* param);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função processa o arquivo .geo, lendo os dados e armazenando-os em uma estrutura de dados adequada (Tabela Hash e Quadras), 
 * e também gera um arquivo .svg com a representação gráfica dos dados do arquivo .geo.
 * 
 * @param param Ponteiro para a estrutura de parâmetros
 * @param h     Ponteiro para a tabela hash que armazenará os dados do arquivo .geo
 * @param q     Ponteiro para o objeto que armazenará os dados lidos do arquivo .geo
 * @return 0 em caso de sucesso. -1 em caso de erro
 */
int processarGeo(Param* param, TabelaHash* h, Quadras* q);
/*###############################################################################################*/

#endif