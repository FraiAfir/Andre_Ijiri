#ifndef _VIA_H
#define _VIA_H

typedef struct via Via;
typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .via
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoVia Buffer para armazenar o caminho completo do arquivo .via
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int montarCaminhoVia(Param* param, char* caminhoVia);
/**
 * Função para ler e processar os dados do arquivo .via
 * @param arquivoVia Ponteiro para o arquivo .via aberto
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int readFileVia(FILE* arquivoVia);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .via
 * @param param Ponteiro para a estrutura de parâmetros
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int processarVia(Param* param);
/*###############################################################################################*/


#endif