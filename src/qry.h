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
/**
 * Função para criar uma instância de Qry com os dados lidos do arquivo .qry
 * @param comando Comando lido do arquivo .qry
 * @param cep     CEP lido do arquivo .qry
 * @param x       Coordenada X lida do arquivo .qry
 * @param y       Coordenada Y lida do arquivo .qry
 * @param w       Largura lida do arquivo .qry
 * @param h       Altura lida do arquivo .qry
 * @param sw      Espessura da borda lida do arquivo .qry
 * @param cfill   Cor de preenchimento lida do arquivo .qry
 * @param cstrk   Cor da borda lida do arquivo .qry
 * @return        Ponteiro para a instância de Qry criada, ou NULL em caso de erro
 */
Qry* criarQry(char* comando, char* cep, char* face, int num, double x, double y, double w, double h, 
    char* cfill, char* cstrk, char* sentido, char* nome, double fator, char* cmc, char* cmr);
/**
 * Função para liberar a memória alocada para uma instância de Qry
 * @param qry Ponteiro para a instância de Qry
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int freeQry(Qry* qry);
/*###############################################################################################*/

#endif