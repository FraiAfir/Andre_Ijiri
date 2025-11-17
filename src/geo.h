#ifndef _GEO_H
#define _GEO_H

typedef struct geo Geo;
typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .geo
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoGeo Buffer para armazenar o caminho completo do arquivo .geo
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int montarCaminhoGeo(Param* param, char* caminhoGeo);
/**
 * Função para ler e processar os dados do arquivo .geo
 * @param arquivoGeo Ponteiro para o arquivo .geo aberto
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int readFileGeo(FILE* arquivoGeo);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .geo
 * @param param Ponteiro para a estrutura de parâmetros
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int processarGeo(Param* param);
/*###############################################################################################*/


#endif