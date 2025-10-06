#ifndef _UTILS_H
#define _UTILS_H

/**
 * Criar um novo objeto de Parametro
 * @return  Ponteiro para o novo objeto criado. NULL em caso de erro
 */
void* criarParametro();

/**
 * Processa os parâmetros/argumentos passados na linha de comando
 * @param param Ponteiro para o objeto criado de Parametro
 * @param argc  Número total de argumentos
 * @param argv  Array dos argumentos
 * @return      0 se sucesso, -1 se erro
 */
int processarParametros(void* param, int argc, char* argv[]);

/**
 * Libera a memória alocada para o objeto de Parametro
 * @param param Ponteiro para o objeto criado de Parametro
 */
void freeParametros(void* param);


#endif