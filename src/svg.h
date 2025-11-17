#ifndef _SVG_H
#define _SVG_H

typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .svg
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoSvg Buffer para armazenar o caminho completo do arquivo .svg
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int montarCaminhoSvg(Param* param, char* caminhoSvg);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .svg
 * @param param Ponteiro para a estrutura de parâmetros
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int processarSvg(Param* param);
/*###############################################################################################*/


#endif