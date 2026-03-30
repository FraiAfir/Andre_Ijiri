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
 * @param cpf O CPF da quadra a ser removida
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int removerQuadra(char* cpf);
/**
 * Função para calcular o número de moradores de uma quadra, de acordo com as instruções do arquivo .qry
 * @param cpf O CPF da quadra para a qual o número de moradores deve ser calculado
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int calcMoradores(char* cpf);
/**
 * Função para produzir várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry
 * @return 0 em caso de sucesso. -1 em caso de erro
 */
int produzirCenso();
/**
 * Função para obter dados sobre o habitante identificado pelo CPF, de acordo com as instruções do arquivo .qry
 * @param cpf O CPF do habitante para o qual os dados devem ser obtidos
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int obterDadosHabitante(char* cpf);
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