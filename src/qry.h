#ifndef _QRY_H
#define _QRY_H

typedef struct qry Qry;
typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .qry
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoQry Buffer para armazenar o caminho completo do arquivo .qry
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int montarCaminhoQry(Param* param, char* caminhoQry);
/**
 * Função para ler e processar os dados do arquivo .qry
 * @param arquivoQry Ponteiro para o arquivo .qry aberto
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int readFileQry(FILE* arquivoQry);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .qry
 * @param param Ponteiro para a estrutura de parâmetros
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int processarQry(Param* param);
/*###############################################################################################*/


#endif