#ifndef _GEO_H
#define _GEO_H

#include "lista.h"

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
int readFileGeo(FILE* arquivoGeo, Lista* l);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .geo
 * @param param Ponteiro para a estrutura de parâmetros
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int processarGeo(Param* param, Lista* l);
/**
 * Função para criar uma instância de Geo com os dados lidos do arquivo .geo
 * @param comando Comando lido do arquivo .geo
 * @param cep     CEP do local
 * @param x       Coordenada x
 * @param y       Coordenada y
 * @param w       Largura
 * @param h       Altura
 * @param sw      Espessura da borda
 * @param cfill   Cor de preenchimento
 * @param cstrk   Cor da borda
 * @return        Ponteiro para a instância de Geo criada
 */
Geo* criarGeo(char* comando, char* cep, double x, double y, double w, double h, double sw, char* cfill, char* cstrk);
/**
 * Função para liberar a memória alocada para uma instância de Geo
 * @param geo Ponteiro para a instância de Geo
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int freeGeo(Geo* geo);
/*###############################################################################################*/

#endif