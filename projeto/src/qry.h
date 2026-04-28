#ifndef _QRY_H
#define _QRY_H

typedef struct qry Qry;
typedef struct parametro Param;

#include "hashPM.h"
#include "hashTable.h"

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
 * @param arquivoQry    Ponteiro para o arquivo .qry aberto
 * @param htp           Ponteiro para a tabela hash de pessoas (hashPM)
 * @param p             Ponteiro para a estrutura de pessoas (Pessoas)
 * @return 0 em caso de sucesso. -1 em caso de erro
 */
int readFileQry(FILE* arquivoQry, hashPM* htp);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .qry
 * @param param Ponteiro para a estrutura de parâmetros
 * @param htp   Ponteiro para a tabela hash de pessoas (hashPM)
 * @param p     Ponteiro para a estrutura de pessoas (Pessoas)
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int processarQry(Param* param, hashPM* htp);
/**
 * Função para criar uma instância de Qry com os dados lidos do arquivo .qry
 * @return Ponteiro para a instância de Qry criada, ou NULL em caso de erro
 */
Qry* criarQry();
/**
 * Função para liberar a memória alocada para uma instância de Qry
 * @param qry Ponteiro para a instância de Qry
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int freeQry(Qry* qry);
/**
 * Função para remover uma quadra do sistema, de acordo com as instruções do arquivo .qry
 * @param cep O CEP da quadra a ser removida
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int removerQuadra(char* cep);
/**
 * Função para calcular o número de moradores de uma quadra, de acordo com as instruções do arquivo .qry
 * @param cep O CEP da quadra para a qual o número de moradores deve ser calculado
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int calcMoradores(char* cep);
/**
 * Função para produzir várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry
 * @return 0 em caso de sucesso. -1 em caso de erro
 */
int produzirCenso();
/**
 * Esta função é responsável por obter os dados de um habitante identificado pelo CPF, de acordo com as instruções do arquivo .qry
 * A função deve tentar buscar os dados do habitante no disco utilizando a tabela hash de pessoas (hashPM). 
 * Se o habitante for encontrado: Seus dados devem ser impressos no arquivo de saída .txt
 * Se o habitante não for encontrado: Uma mensagem de erro deve ser impressa no arquivo de saída .txt
 * 
 * @param htp   Ponteiro para a tabela hash de pessoas (hashPM)
 * @param cpf   O CPF do habitante a ser buscado
 * @param txt   Ponteiro para o arquivo de saída .txt
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int obterDadosHabitante(hashPM* htp, char* cpf, FILE* txt);
/**
 * Função para o nascimento de um habitante, de acordo com as instruções do arquivo .qry
 * @param cpf       O CPF do habitante a ser registrado
 * @param nome      O nome do habitante a ser registrado
 * @param sobrenome O sobrenome do habitante a ser registrado
 * @param sexo      O sexo do habitante a ser registrado
 * @param nasc      A data de nascimento do habitante a ser registrado
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int registrarNascimento(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc);
/**
 * Função para registrar o falecimento de um habitante, de acordo com as instruções do arquivo .qry
 * @param cpf O CPF do habitante a ser registrado como falecido
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int registrarObito(char* cpf);
/**
 * Função para registrar a mudança de endereço de um habitante, de acordo com as instruções do arquivo .qry
 * @param cpf   O CPF do habitante que está se mudando
 * @param cep   O CEP do novo endereço
 * @param face  A face do novo endereço
 * @param num   O número do novo endereço
 * @param cmpl  O complemento do novo endereço
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int registrarMudanca(char* cpf, char* cep, char face, int num, char* cmpl);
/**
 * Função para registrar o despejo de um habitante, de acordo com as instruções do arquivo .qry
 * @param cpf O CPF do habitante a ser registrado como despejado
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int registrarDespejo(char* cpf);
/*###############################################################################################*/

#endif