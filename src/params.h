#ifndef _UTILS_H
#define _UTILS_H

typedef struct parametro* Param;

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
 * @return      0 se sucesso, -1 se erro
 */
int processarParametros(Param* param, int argc, char* argv[]);

/**
 * Libera a memória alocada para o objeto de Parametro
 * @param param Ponteiro para o objeto criado de Parametro
 */
void freeParametros(Param* param);

/**
 * Função auxiliar para processar os argumentos da linha de comando
 * @param param Ponteiro para o objeto criado de Parametro
 * @param argc  Número total de argumentos
 * @param argv  Array dos argumentos
 * @return      0 se sucesso, -1 se erro
 */
int processarArgumentosInternos(Param* param, int argc, char* argv[]);

/**
 * Trata os caminhos completos dos arquivos e diretórios de entrada e saída
 * @param param Ponteiro para o objeto criado de Parametro
 * @return      0 se sucesso, -1 se erro
 */
int tratarCaminhosCompletos(Param* param);

#endif