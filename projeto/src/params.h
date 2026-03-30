#ifndef _PARAMS_H
#define _PARAMS_H

#include "tree.h"

typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Trata os caminhos completos dos arquivos e diretórios de entrada e saída
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      0 se sucesso, -1 se erro
 */
int tratarCaminhosCompletos(Param* param);
/**
 * Função auxiliar para processar os argumentos da linha de comando
 * @param param Ponteiro para o objeto criado de Parametro
 * @param argc  Número total de argumentos
 * @param argv  Array dos argumentos
 * @return      0 se sucesso, -1 se erro
 */
int processarArgumentosInternos(Param* param, int argc, char* argv[]);
/**
 * Função para obter o caminho completo do diretório de entrada
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      Ponteiro para a string do caminho completo do diretório de entrada
 */
char* getDirEntradaCompleto(Param* param);
/**
 * Função para obter o caminho completo do diretório de saída
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      Ponteiro para a string do caminho completo do diretório de saída
 */
char* getDirSaidaCompleto(Param* param);
/**
 * Função para obter o nome do arquivo .geo
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      Ponteiro para a string do nome do arquivo .geo
 */
char* getNomeGeo(Param* param);
/**
 * Função para obter o nome do arquivo .qry
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      Ponteiro para a string do nome do arquivo .qry
 */
char* getNomeQry(Param* param);
/**
 * Função para obter o nome do arquivo .pm
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      Ponteiro para a string do nome do arquivo .pm
 */
char* getNomePM(Param* param);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Criar um novo objeto de Parametro
 * @return  Ponteiro para o novo objeto criado. NULL em caso de erro
 */
Param* criarParametro();
/**
 * Processa os parâmetros/argumentos passados na linha de comando
 * @param param Ponteiro para o objeto criado de Parametro
 * @param argc  Número total de argumentos
 * @param argv  Array dos argumentos
 * @return      0 se sucesso. -1 se erro
 */
int processarParametros(Param* param, int argc, char* argv[]);
/**
 * Libera a memória alocada para o objeto de Parametro
 * @param param Ponteiro para o objeto criado de Parametro
 */
void freeParametros(Param* param);
/*###############################################################################################*/

#endif