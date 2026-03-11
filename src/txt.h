#ifndef _TXT_H
#define _TXT_H

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .txt (Com o qry)
 * @param param         Ponteiro para a estrutura de parâmetros
 * @param caminhoTxt    Buffer para armazenar o caminho completo do arquivo .txt (Com o qry)
 * @return              Ponteiro para o caminho completo do arquivo .txt (Com o qry). NULL em caso de erro
 */
char* montarCaminhoTxt(Param* param, char* caminhoTxt);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para abrir o arquivo .txt para escrita
 * @param caminhoTxt Caminho completo do arquivo .txt a ser criado
 * @return           Ponteiro para o arquivo .txt criado. NULL em caso de erro
 */
FILE* criarTxt(char* arqTxt);

/**
 * Função para escrever um comando ou resposta no arquivo .txt
 * @param txt   Ponteiro para o arquivo .txt
 * @param texto Texto a ser escrito no arquivo
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int excreverComandoTxt(FILE* txt, char* texto);

/**
 * Função para fechar o arquivo .txt após a geração do conteúdo
 * @param txt Ponteiro para o arquivo .txt a ser fechado
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int finalizarTxt(FILE* txt);
/*###############################################################################################*/

#endif