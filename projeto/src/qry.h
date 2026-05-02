#ifndef _QRY_H
#define _QRY_H

typedef struct qry Qry;
typedef struct parametro Param;

#include "hashPM.h"
#include "hashTable.h"

/**
 * O qry é um arquivo de texto que contém uma série de comandos de consulta e modificação dos dados armazenados a partir do arquivo .geo,
 * conforme as instruções do arquivo .qry, e produzir os arquivos de saída .svg e .txt com os resultados das operações realizadas.
 * 
 * Este módulo é responsável por processar o arquivo .qry, 
 * realizando as operações de consulta e modificação nos dados armazenados a partir do arquivo .geo,
 * conforme as instruções do arquivo .qry, e produzir os arquivos de saída .svg e .txt com os resultados das operações realizadas.
 */

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função é responsável por montar o caminho completo do arquivo .qry 
 * a partir do diretório de entrada e do nome do arquivo .qry fornecidos na linha de comando, 
 * e armazenar o resultado no buffer caminhoQry.
 * 
 * @param param         Ponteiro para a estrutura de parâmetros
 * @param caminhoQry    Buffer para armazenar o caminho completo do arquivo .qry
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int montarCaminhoQry(Param* param, char* caminhoQry);
/**
 * Esta função é responsável por ler o arquivo .qry linha por linha, processar as consultas e modificações descritas no arquivo .qry,
 * e produzir os arquivos de saída .svg e .txt com os resultados das operações realizadas.
 * 
 * @param arquivoQry    Ponteiro para o arquivo .qry aberto
 * @param htp           Ponteiro para a tabela hash de pessoas (hashPM)
 * @param h             Ponteiro para a tabela hash de quadras (TabelaHash)
 * @param param         Ponteiro para a estrutura de parâmetros
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int readFileQry(FILE* arquivoQry, hashPM* htp, TabelaHash* h, Param* param);
/**
 * Esta função é responsável por clonar o arquivo .svg base para um novo arquivo .svg do .qry, 
 * que será utilizado para desenhar os resultados das operações do arquivo .qry.
 * 
 * O arquivo .svg do .qry é criado a partir do clone do arquivo .svg base, 
 * e o conteúdo do arquivo .svg base é copiado para o arquivo .svg do .qry até a tag de fechamento </svg>, 
 * que é ignorada para permitir que o arquivo .svg do .qry receba novos desenhos.
 * 
 * @param caminhoSvgBase O caminho completo do arquivo .svg base a ser clonado
 * @param caminhoSvgQry  O caminho completo do arquivo .svg do .qry a ser criado
 * @return Ponteiro para o arquivo .svg do .qry criado, ou NULL em caso de erro
 */
FILE* clonarSvgBase(char* caminhoSvgBase, char* caminhoSvgQry);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta é uma função wrapper para o processamento do arquivo .qry. 
 * 
 * Ela é responsável por orquestrar as etapas de leitura e processamento do arquivo .qry,
 * incluindo a montagem do caminho completo do arquivo .qry, a abertura do arquivo .qry para leitura, 
 * a chamada da função de leitura e processamento do arquivo .qry, e o fechamento do arquivo .qry após o processamento.
 * 
 * @param param Ponteiro para a estrutura de parâmetros
 * @param htp   Ponteiro para a tabela hash de pessoas (hashPM)
 * @param h     Ponteiro para a tabela hash de quadras (TabelaHash)
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int processarQry(Param* param, hashPM* htp, TabelaHash* h);
/**
 * Esta função é responsável por criar uma instância de Qry, alocando a memória necessária e inicializando seus campos.
 * @return Ponteiro para a instância de Qry criada. NULL em caso de erro
 */
Qry* criarQry();
/**
 * Esta função é responsável por liberar a memória alocada para uma instância de Qry, incluindo os campos internos da estrutura.
 * @param qry Ponteiro para a instância de Qry
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
int freeQry(Qry* qry);
/**
 * Esta função é responsável por remover uma quadra do sistema, de acordo com as instruções do arquivo .qry.
 * A função deve realizar as seguintes operações:
 * - Buscar a quadra a ser removida utilizando o CEP fornecido, e obter as coordenadas da âncora da quadra para desenhar a remoção no SVG;
 * - Despejar os moradores da quadra a ser removida, ou seja, 
 * remover os dados de moradia (CEP, face, num, compl) dos registros das pessoas que moram na quadra e salvar os buckets atualizados no disco;
 * - Remover a quadra do sistema, de acordo com as instruções do arquivo .qry, e imprimir uma mensagem de sucesso no arquivo de saída .txt;
 * - Se a quadra não for encontrada, reportar o erro no arquivo de saída .txt;
 * 
 * @param htp       O ponteiro para a tabela hash de pessoas
 * @param cep       O CEP da quadra a ser removida
 * @param txt       O ponteiro para o arquivo de saída .txt
 * @param h         O ponteiro para a tabela hash de quadras (TabelaHash)
 * @param qrySVG    O ponteiro para o arquivo .svg do .qry, para desenhar a remoção da quadra no SVG
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int removerQuadraQRY(hashPM* htp, char* cep, FILE* txt, TabelaHash* h, FILE* qrySVG);
/**
 * Esta função é responsável por calcular o número de moradores de uma quadra, de acordo com as instruções do arquivo .qry.
 * A função deve realizar as seguintes operações:
 * - Calcular o número total de moradores da quadra, bem como a distribuição dos moradores por face (N, S, L, O);
 * - Imprimir os resultados no arquivo de saída .txt;
 * 
 * @param htp       O ponteiro para a tabela hash de pessoas
 * @param h         O ponteiro para a tabela hash de quadras (TabelaHash)
 * @param cep       O CEP da quadra para a qual o número de moradores deve ser calculado
 * @param txt       O ponteiro para o arquivo de saída .txt
 * @param qrySVG    O ponteiro para o arquivo .svg do .qry, para desenhar a quadra e seus moradores no SVG
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int calcMoradores(hashPM* htp, TabelaHash* h, char* cep, FILE* txt, FILE* qrySVG);
/**
 * Esta função é responsável por produzir várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry
 * A função deve percorrer toda a tabela hash de pessoas (hashPM) para calcular as estatísticas do censo, 
 * e imprimir os resultados no arquivo de saída .txt
 * 
 * @param htp   Ponteiro para a tabela hash de pessoas (hashPM)
 * @param txt   Ponteiro para o arquivo de saída .txt
 * @return      0 em caso de sucesso. -1 em caso de erro
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
 * Esta função é responsável por registrar o óbito de um habitante, de acordo com as instruções do arquivo .qry
 * A função deve realizar as seguintes operações:
 * - Buscar o registro do habitante a ser registrado como falecido utilizando o CPF fornecido, 
 * e obter as coordenadas da âncora do endereço do habitante para desenhar a cruz vermelha no SVG;
 * 
 * - Imprimir os dados do habitante falecido no arquivo de saída .txt, 
 * incluindo nome, sobrenome, sexo, data de nascimento, e endereço (se for morador) ou situação de sem-teto (se não for morador);
 * 
 * - Remover o registro do habitante do sistema, de acordo com as instruções do arquivo .qry, 
 * e desenhar uma cruz vermelha no local do endereço do habitante falecido no arquivo .svg do .qry;
 * 
 * - Se o registro do habitante não for encontrado, reportar o erro no arquivo de saída .txt;
 * - Se ocorrer qualquer erro durante o processo de registro de óbito, reportar o erro no arquivo de saída .txt;
 * 
 * @param htp       O ponteiro para a tabela hash de pessoas
 * @param htq       O ponteiro para a tabela hash de quadras, para obter as coordenadas do endereço do habitante falecido e desenhar a cruz vermelha no SVG
 * @param cpf       O CPF do habitante a ser registrado como falecido
 * @param txt       O ponteiro para o arquivo de saída .txt
 * @param qrySVG    O ponteiro para o arquivo .svg do .qry, para desenhar a cruz vermelha no local do endereço do habitante falecido
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int registrarObito(hashPM* htp, TabelaHash* htq, char* cpf, FILE* txt, FILE* qrySVG);
/**
 * Esta função é responsável por registrar a mudança de endereço de um habitante, de acordo com as instruções do arquivo .qry
 * A função deve localizar o registro da pessoa na tabela hash utilizando o CPF como chave, 
 * atualizar os campos de endereço com as informações fornecidas, e salvar as alterações no disco.
 * Se o registro for encontrado e atualizado com sucesso: Uma mensagem de sucesso deve ser impressa no arquivo de saída .txt
 * Se o registro não for encontrado ou a atualização falhar: Uma mensagem de erro deve ser impressa no arquivo de saída .txt
 * 
 * @param htp       Ponteiro para a tabela hash de pessoas (hashPM)
 * @param htq       Ponteiro para a tabela hash de quadras (TabelaHash), para obter as coordenadas do endereço de destino e desenhar o quadrado vermelho no SVG
 * @param cpf       O CPF do habitante a ser registrado para mudança de endereço
 * @param novo_cep  O CEP do novo endereço
 * @param face      A face do novo endereço
 * @param num       O número do novo endereço
 * @param compl     O complemento do novo endereço
 * @param txt       Ponteiro para o arquivo de saída .txt
 * @param qrySVG    Ponteiro para o arquivo de saída .svg
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int registrarMudanca(hashPM* htp, TabelaHash* htq, char* cpf, char* novo_cep, char face, int num, char* compl, FILE* txt, FILE* qrySVG);
/**
 * Esta função é responsável por registrar o despejo de um habitante, de acordo com as instruções do arquivo .qry
 * A função deve localizar o registro da pessoa na tabela hash utilizando o CPF como chave, 
 * remover as informações de endereço (CEP, face, número, complemento) da estrutura de Pessoas, e salvar as alterações no disco.
 * Se o registro for encontrado e atualizado com sucesso: Uma mensagem de sucesso deve ser impressa no arquivo de saída .txt
 * Se o registro não for encontrado ou a atualização falhar: Uma mensagem de erro deve ser impressa no arquivo de saída .txt
 * 
 * @param htp       Ponteiro para a tabela hash de pessoas (hashPM)
 * @param htq       Ponteiro para a tabela hash de quadras (TabelaHash)
 * @param cpf       O CPF do habitante a ser registrado para despejo
 * @param txt       Ponteiro para o arquivo de saída .txt
 * @param qrySVG    Ponteiro para o arquivo de saída .svg
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int registrarDespejo(hashPM* htp, TabelaHash* htq, char* cpf, FILE* txt, FILE* qrySVG);
/*###############################################################################################*/

#endif