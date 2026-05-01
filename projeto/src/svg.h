#ifndef _SVG_H
#define _SVG_H

typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função monta o caminho completo do arquivo .svg a partir do diretório de saída e do nome do arquivo .geo, 
 * e armazena o resultado no buffer caminhoSvg.
 * 
 * @param param      Ponteiro para a estrutura de parâmetros
 * @param caminhoSvg Buffer para armazenar o caminho completo do arquivo .svg
 * @return           Ponteiro para o caminho completo do arquivo .svg. NULL em caso de erro
 */
char* montarCaminhoSvg(Param* param, char* caminhoSvg);
/**
 * Esta função monta o caminho completo do arquivo .svg a partir do diretório de saída e do nome do arquivo .qry, 
 * e armazena o resultado no buffer caminhoSvgQry.
 * 
 * @param param         Ponteiro para a estrutura de parâmetros
 * @param caminhoSvgQry Buffer para armazenar o caminho completo do arquivo .svg (Com o qry)
 * @return              Ponteiro para o caminho completo do arquivo .svg (Com o qry). NULL em caso de erro
 */
char* montarCaminhoSvgQry(Param* param, char* caminhoSvgQry);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função processa o arquivo .svg, gerando o conteúdo do arquivo .txt e do arquivo .svg (Com o qry) 
 * a partir dos dados do arquivo .geo e do arquivo .qry.
 * 
 * @param param Ponteiro para a estrutura de parâmetros
 * @return      0 em caso de sucesso, -1 em caso de erro
 */
int processarSvg(Param* param);

/**
 * Esta função cria um arquivo .svg com a declaração do elemento <svg> e as dimensões especificadas.
 * @param caminhoSvg Caminho completo do arquivo .svg a ser criado
 * @param largura    Largura do arquivo .svg
 * @param altura     Altura do arquivo .svg
 * @return           Ponteiro para o arquivo .svg criado, ou NULL em caso de erro
 */
FILE* criarSvg(char* caminhoSvg, double largura, double altura);

/**
 * Esta função desenha uma forma no arquivo .svg com as propriedades especificadas.
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
 * Esta função fecha o arquivo .svg, garantindo que todas as alterações sejam salvas corretamente.
 * @param arqSvg Ponteiro para o arquivo .svg a ser fechado
 * @return       0 em caso de sucesso, -1 em caso de erro
 */
int fecharSvg(FILE* arqSvg);
/*###############################################################################################*/


#endif