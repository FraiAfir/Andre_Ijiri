#ifndef _PM_H
#define _PM_H

typedef struct pm PM;
typedef struct parametro Param;

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função para montar o caminho completo do arquivo .pm
 * @param param     Ponteiro para a estrutura de parâmetros
 * @param caminhoPM Buffer para armazenar o caminho completo do arquivo .pm
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoPM(Param* param, char* caminhoPM);
/**
 * Função para ler e processar os dados do arquivo .pm
 * @param arquivoPM Ponteiro para o arquivo .pm aberto
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int readFilePM(FILE* arquivoPM);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para processar o arquivo .pm
 * @param param Ponteiro para a estrutura de parâmetros
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int processarPM(Param* param);
/**
 * Função para criar uma instância de PM com os dados lidos do arquivo .pm
 * @return Ponteiro para a instância de PM criada
 */
PM* criarPM();
/**
 * Função para liberar a memória alocada para uma instância de PM
 * @param pm Ponteiro para a instância de PM
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int freePM(PM* pm);
/**
 * Função para inserir um habitante, de acordo com as instruções do arquivo .pm
 * @param cpf       O CPF do habitante a ser inserido
 * @param nome      O nome do habitante a ser inserido
 * @param sobrenome O sobrenome do habitante a ser inserido
 * @param sexo      O sexo do habitante a ser inserido
 * @param nasc      A data de nascimento do habitante a ser inserido
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int inserirHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc);
/**
 * Função para informar que um dado habitante mora num dado endereço, de acordo com as instruções do arquivo .pm
 * @param cpf   O CPF do habitante
 * @param cep   O CEP do endereço onde o habitante mora
 * @param face  A face do endereço onde o habitante mora
 * @param num   O número do endereço onde o habitante mora
 * @param cmpl  O complemento do endereço onde o habitante mora
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int informarHabitanteMoraEndereco(char* cpf, char* cep, char face, int num, char* cmpl);
/*###############################################################################################*/


#endif