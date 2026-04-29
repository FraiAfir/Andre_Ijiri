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
 * @param h             Ponteiro para a tabela hash de quadras (TabelaHash)
 * @return 0 em caso de sucesso. -1 em caso de erro
 */
int readFileQry(FILE* arquivoQry, hashPM* htp, TabelaHash* h);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .qry
 * @param param Ponteiro para a estrutura de parâmetros
 * @param htp   Ponteiro para a tabela hash de pessoas (hashPM)
 * @param h     Ponteiro para a tabela hash de quadras (TabelaHash)
 * @return 0 em caso de sucesso, -1 em caso de erro
 */
int processarQry(Param* param, hashPM* htp, TabelaHash* h);
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
 * Esta função é responsável por remover uma quadra do sistema, de acordo com as instruções do arquivo .qry
 * @param htp O ponteiro para a tabela hash de pessoas
 * @param cep O CEP da quadra a ser removida
 * @param txt O ponteiro para o arquivo de saída .txt
 * @param h   O ponteiro para a tabela hash de quadras (TabelaHash)
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int removerQuadraQRY(hashPM* htp, char* cep, FILE* txt, TabelaHash* h);
/**
 * Esta função é responsável por calcular o número de moradores de uma quadra, de acordo com as instruções do arquivo .qry
 * @param htp O ponteiro para a tabela hash de pessoas
 * @param cep O CEP da quadra para a qual o número de moradores deve ser calculado
 * @param txt O ponteiro para o arquivo de saída .txt
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int calcMoradores(hashPM* htp, char* cep, FILE* txt);
/**
 * Esta função é responsável por produzir várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry
 * A função deve percorrer toda a tabela hash de pessoas (hashPM) para calcular as estatísticas do censo, 
 * e imprimir os resultados no arquivo de saída .txt
 * 
 * @param htp   Ponteiro para a tabela hash de pessoas (hashPM)
 * @param txt   Ponteiro para o arquivo de saída .txt
 */
int produzirCenso(hashPM* htp, FILE* txt);
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
 * Esta função é responsável por registrar o nascimento de um habitante, de acordo com as instruções do arquivo .qry
 * A função deve registrar o nascimento do habitante utilizando a tabela hash de pessoas (hashPM)
 * Se o registro for bem-sucedido: Uma mensagem de sucesso deve ser impressa no arquivo de saída .txt
 * Se o registro falhar: Uma mensagem de erro deve ser impressa no arquivo de saída .txt
 * 
 * @param htp       Ponteiro para a tabela hash de pessoas (hashPM)
 * @param cpf       O CPF do habitante a ser registrado
 * @param nome      O nome do habitante a ser registrado
 * @param sobrenome O sobrenome do habitante a ser registrado
 * @param sexo      O sexo do habitante a ser registrado
 * @param nasc      A data de nascimento do habitante a ser registrado
 * @param txt       Ponteiro para o arquivo de saída .txt
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int registrarNascimento(hashPM* htp, char* cpf, char* nome, char* sobrenome, char sexo, char* nasc, FILE* txt);
/**
 * Função para registrar o falecimento de um habitante, de acordo com as instruções do arquivo .qry
 * @param htp O ponteiro para a tabela hash de pessoas
 * @param cpf O CPF do habitante a ser registrado como falecido
 * @param txt O ponteiro para o arquivo de saída .txt
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int registrarObito(hashPM* htp, char* cpf, FILE* txt);
/**
 * Esta função é responsável por registrar a mudança de endereço de um habitante, de acordo com as instruções do arquivo .qry
 * A função deve localizar o registro da pessoa na tabela hash utilizando o CPF como chave, 
 * atualizar os campos de endereço com as informações fornecidas, e salvar as alterações no disco.
 * Se o registro for encontrado e atualizado com sucesso: Uma mensagem de sucesso deve ser impressa no arquivo de saída .txt
 * Se o registro não for encontrado ou a atualização falhar: Uma mensagem de erro deve ser impressa no arquivo de saída .txt
 * 
 * @param htp       Ponteiro para a tabela hash de pessoas (hashPM)
 * @param cpf       O CPF do habitante a ser registrado para mudança de endereço
 * @param cep       O CEP do novo endereço
 * @param face      A face do novo endereço
 * @param num       O número do novo endereço
 * @param cmpl      O complemento do novo endereço
 * @param txt       Ponteiro para o arquivo de saída .txt
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int registrarMudanca(hashPM* htp, char* cpf, char* cep, char face, int num, char* cmpl, FILE* txt);
/**
 * Esta função é responsável por registrar o despejo de um habitante, de acordo com as instruções do arquivo .qry
 * A função deve localizar o registro da pessoa na tabela hash utilizando o CPF como chave, 
 * remover as informações de endereço (CEP, face, número, complemento) da estrutura de Pessoas, e salvar as alterações no disco.
 * Se o registro for encontrado e atualizado com sucesso: Uma mensagem de sucesso deve ser impressa no arquivo de saída .txt
 * Se o registro não for encontrado ou a atualização falhar: Uma mensagem de erro deve ser impressa no arquivo de saída .txt
 * 
 * @param htp       Ponteiro para a tabela hash de pessoas (hashPM)
 * @param cpf       O CPF do habitante a ser registrado para despejo
 * @param txt       Ponteiro para o arquivo de saída .txt
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int registrarDespejo(hashPM* htp, char* cpf, FILE* txt);
/*###############################################################################################*/

#endif