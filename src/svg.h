#ifndef _SVG_H
#define _SVG_H

typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .svg
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoSvg Buffer para armazenar o caminho completo do arquivo .svg
 * @return           Ponteiro para o caminho completo do arquivo .svg. NULL em caso de erro
 */
char* montarCaminhoSvg(Param* param, char* caminhoSvg);
/**
 * Função para montar o caminho completo do arquivo .svg (Com o qry)
 * @param param         Ponteiro para a estrutura de parâmetros
 * @param caminhoSvgQry Buffer para armazenar o caminho completo do arquivo .svg (Com o qry)
 * @return              Ponteiro para o caminho completo do arquivo .svg (Com o qry). NULL em caso de erro
 */
char* montarCaminhoSvgQry(Param* param, char* caminhoSvgQry);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .svg
 * @param param Ponteiro para a estrutura de parâmetros
 * @return      0 em caso de sucesso, -1 em caso de erro
 */
int processarSvg(Param* param);

/**
 * Função para criar o arquivo .svg
 * @param caminhoSvg Caminho completo do arquivo .svg a ser criado
 * @return           Ponteiro para o arquivo .svg criado, ou NULL em caso de erro
 */
FILE* criarSvg(char* caminhoSvg);

/**
 * Função para desenhar uma forma no arquivo .svg
 * @param arqSvg    Ponteiro para o arquivo .svg
 * @param tipoForma Tipo da forma a ser desenhada
 * @param x         Coordenada x da posição da forma
 * @param y         Coordenada y da posição da forma
 * @param w         Largura da forma
 * @param h         Altura da forma
 * @param sw        Largura da borda da forma
 * @param cstrk     Cor da borda da forma
 * @param cfill     Cor de preenchimento da forma
 * @return          0 em caso de sucesso, -1 em caso de erro
 */
int desenharFormaSvg(FILE* arqSvg, char* tipoForma, 
    double x, double y, double w, double h, double sw,
    char* cstrk, char* cfill);

/**
 * Função para fechar o arquivo .svg
 * @param arqSvg Ponteiro para o arquivo .svg a ser fechado
 * @return       0 em caso de sucesso, -1 em caso de erro
 */
int fecharSvg(FILE* arqSvg);
/*###############################################################################################*/


#endif