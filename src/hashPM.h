
#ifndef _HASHPM_H
#define _HASHPM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct pessoas Pessoas;
typedef struct bucket Bucket;
typedef struct hashPM hashPM;

/**
 * A Tabela Hash é uma estrutura de dados que permite o armazenamento e a recuperação eficiente de pares chave:valor.
 * Ela utiliza uma função de hash para mapear as chaves a índices em um array, onde os valores correspondentes são armazenados.
 * As principais operações suportadas pela Tabela Hash incluem a criação da tabela, inserção de registros, remoção de chaves, 
 * busca por valores associados a chaves, entre outras.
 * Esta implementação específica da Tabela Hash é projetada para armazenar registros do tipo "Pessoas", 
 * que contêm informações como CPF, nome, endereço, etc.

 * A Tabela Hash é implementada utilizando o método de hashing extensível, 
 * onde cada bucket pode armazenar um número limitado de registros (TAM_BUCKET),
 * e quando um bucket atinge sua capacidade máxima, ele é dividido (split) 
 * e os registros são redistribuídos entre o bucket antigo e um novo bucket criado durante o processo de divisão.
 * O diretório da Tabela Hash é duplicado quando necessário para acomodar mais buckets, 
 * e a profundidade global e local são atualizadas de acordo com as operações realizadas.
 * 
 * A função de hash utilizada é o algoritmo FNV-1a, que é conhecido por ser simples, 
 * eficiente, e é amplamente utilizado em implementações de tabelas hash.
 */

/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Esta função é responsável por calcular o índice do bucket correspondente a uma chave fornecida.
 * Ela utiliza a função de hash DJB2 para mapear a chave a um índice dentro do tamanho do diretório da tabela hash.

 * A função de hash DJB2 é conhecida por ser simples e eficiente, e é amplamente utilizada em implementações de tabelas hash.
 * A escolha de 5381 como valor inicial é baseada em experimentos e análises que mostraram que ele proporciona uma boa distribuição de chaves para muitas entradas comuns.
 * A função de hash DJB2 é definida como: hash(i) = hash(i - 1) * 33 + c, onde hash(0) é o valor inicial (5381) e c é o valor ASCII do caractere atual.
 * O número 33 é escolhido porque é um número primo que ajuda a distribuir as chaves de forma mais uniforme na tabela hash, reduzindo a probabilidade de colisões.
 * 
 * @param key Chave a ser mapeada para um índice de bucket na tabela hash (string: CEP)
 * @return    Índice do bucket correspondente à chave fornecida
 */
unsigned int hashFuncPM(char* key);
/**
 * Esta é uma função auxiliar de slipBucket(); 
 *  - Responsável por duplicar o diretório da tabela hash quando um bucket estoura a capacidade de TAM_BUCKET,
 * aumentando a profundidade global e o tamanho do diretório, e redistribuindo os registros entre os buckets.
 *  - A função verifica se a profundidade local do bucket causador da colisão é igual à profundidade global da tabela hash.
 *  - Se for igual, significa que o bucket causador da colisão está utilizando todos os bits disponíveis para calcular o índice do bucket, 
 * e portanto, é necessário duplicar o diretório para acomodar mais buckets.
 *  - A função duplica o tamanho do vetor de endereços do diretório na RAM, 
 * espelha os endereços dos buckets antigos para os novos índices do diretório, e atualiza a profundidade global e o tamanho do diretório.

 * @param dir           Ponteiro para o diretório da tabela hash
 * @param indice_dir    Índice do diretório onde a pessoa deve ser inserida
 * @param balde_antigo  Referência ao bucket que causou a colisão
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int duplicarDiretorioPM(hashPM* dir, int indice_dir, Bucket balde_antigo);
/**
 * Esta é uma função auxiliar de slipBucket();
 *  - Responsável por redistribuir os registros de um bucket que estourou a capacidade de TAM_BUCKET, 
 * entre o bucket antigo e um novo bucket criado durante o processo de slipBucket.
 *  - A função coloca os registros antigos e o novo registro que causou a colisão em um buffer temporário, 
 * esvazia o bucket antigo para preenchê-lo novamente, 
 * e utiliza um bit divisor para determinar quais registros devem permanecer no bucket antigo e quais devem ser movidos para o novo bucket.
 *  - O bit divisor é calculado como 1 << (profundidade_local_nova - 1), 
 * onde profundidade_local_nova é a profundidade local do bucket após a divisão.
 *  - A função itera sobre o buffer de registros, calcula o índice do bucket para cada registro usando a função de hash e o bit divisor, 
 * e redistribui os registros entre o bucket antigo e o novo bucket de acordo com o índice calculado.
 * 
 * @param dir               Ponteiro para o diretório da tabela hash
 * @param indice_dir        Índice do diretório onde a pessoa deve ser inserida
 * @param balde_antigo      Referência ao bucket que causou a colisão
 * @param balde_novo        Referência ao novo bucket criado durante o processo de slipBucket
 * @param bit_divisor       Bit divisor utilizado para determinar a redistribuição dos registros entre o bucket antigo e o novo bucket
 * @return                  0 em caso de sucesso. -1 em caso de erro
 */
int redistribuirRegistrosPM(hashPM* dir, int indice_dir, Bucket* balde_antigo, Bucket* balde_novo, int bit_divisor);
/**
 * Esta é uma função auxiliar de slipBucket();
 *  - Responsável por atualizar o diretório da tabela hash para apontar para os buckets corretos
 * (antigo e novo) de acordo com a nova profundidade local após a divisão de um bucket.
 *  - A função procura no diretório todos os ponteiros que apontavam para o bucket antigo e que possuem o bit divisor igual a 1, 
 * e muda esses ponteiros para apontar para o novo bucket.
 *  - O bit divisor é utilizado para identificar quais índices do diretório devem ser atualizados para apontar para o novo bucket, 
 * garantindo que os registros sejam corretamente acessados após a divisão do bucket.
 * - Após atualizar os ponteiros no diretório, a função salva os dois buckets atualizados fisicamente no HD, 
 * garantindo que as mudanças sejam persistidas.
 * 
 * @param dir                   Ponteiro para o diretório da tabela hash
 * @param offset_bucket_antigo  Offset do bucket antigo no arquivo
 * @param offset_bucket_novo    Offset do bucket novo no arquivo
 * @param bucket_antigo         Referência ao bucket antigo
 * @param bucket_novo           Referência ao novo bucket
 * @param bit_divisor           Bit divisor utilizado para identificar quais índices do diretório devem ser atualizados
 * @return                      0 em caso de sucesso. -1 em caso de erro
 */
int atualizarDiretorioPM(hashPM* dir, long offset_bucket_antigo, long offset_bucket_novo, Bucket* bucket_antigo, Bucket* bucket_novo, int bit_divisor);
/**
 * Esta função é responsável por atualziar os dados de um habitante (Pessoas) na tabela hash, 
 * adicionando as informações de moradia (CEP, face, número, complemento)
 * A função deve localizar o registro da pessoa na tabela hash utilizando o CPF como chave, 
 * e atualizar os campos de endereço com as informações fornecidas.
 * 
 * @param dir       Ponteiro para o diretório da tabela hash
 * @param cpf       Chave do registro a ser atualizado (CPF)
 * @param cep       CEP do endereço a ser adicionado
 * @param face      Face do endereço a ser adicionado
 * @param num       Número do endereço a ser adicionado
 * @param compl     Complemento do endereço a ser adicionado
 * @return          0 em caso de sucesso. -1 em caso de erro
 */
int adicionarMoradia(hashPM* dir, char* cpf, char* cep, char* face, char* num, char* compl);
/**
 * Esta função é responsável por buscar um registro do tipo Pessoas na tabela hash utilizando o CPF como chave.
 * A função deve calcular o índice do bucket correspondente ao CPF fornecido, 
 * acessar o bucket no arquivo físico da tabela hash, e procurar pelo registro dentro do bucket.
 * 
 * Se o registro for encontrado: 
 * A função deve copiar os dados para a estrutura de resultado fornecida e retornar 1 para indicar que o registro foi encontrado.
 * Se o registro não for encontrado: 
 * A função deve retornar 0 para indicar que o registro não foi encontrado.
 * 
 * @param dir       Ponteiro para o diretório da tabela hash
 * @param cpf       Chave do registro a ser buscado (CPF)
 * @param resultado Ponteiro para a estrutura onde os dados do registro encontrado serão copiados
 * @return          1 se o registro for encontrado, 0 caso contrário
 */
int buscarPessoa(hashPM* dir, char* cpf, Pessoas* resultado);
/**
 * Esta função é responsável por atualizar os dados de um habitante (Pessoas) na tabela hash, 
 * permitindo a atualização de informações como nome, sobrenome, sexo e data de nascimento.
 * Será utilizada para comandos como "mud" e "dspj"
 * 
 * @param dir               Ponteiro para o diretório da tabela hash
 * @param pessoaAtualizada  Ponteiro para a estrutura de Pessoas contendo os dados atualizados do habitante
 * @return                  0 em caso de sucesso. -1 em caso de erro
 */
int atualizarPessoa(hashPM* dir, Pessoas* pessoaAtualizada);
/**
 * Esta função é responsável por remover um registro do tipo Pessoas da tabela hash utilizando o CPF como chave.
 * A função deve calcular o índice do bucket correspondente ao CPF fornecido,
 * acessar o bucket no arquivo físico da tabela hash, e procurar pelo registro dentro do bucket.

 * @param dir   Ponteiro para o diretório da tabela hash
 * @param cpf   CPF da pessoa a ser removida
 * @return      1 em caso de sucesso, 0 caso contrário
 */
int removerPessoa(hashPM* dir, char* cpf);
/**
 * Esta função é responsável por calcular várias estatísticas sobre os habitantes de Bitnópolis,
 * percorrendo toda a tabela hash de pessoas (hashPM) para contar:
 * - total de habitantes; 
 * - moradores; 
 * - pessoas sem teto; 
 * - distribuição por sexo e moradia;
 * 
 * A função deve iterar sobre todos os buckets da tabela hash, ler os registros de cada bucket, e atualizar as contagens de acordo com as informações de cada habitante.
 * As estatísticas calculadas incluem:
 * - Total de habitantes
 * - Total de moradores
 * - Total de pessoas sem teto
 * - Distribuição por sexo (homens e mulheres)
 * - Distribuição por moradia (moradores e pessoas sem teto)
 * 
 * @param dir         Ponteiro para o diretório da tabela hash
 * @param totHab      Ponteiro para a variável onde será armazenado o total de habitantes
 * @param totMor      Ponteiro para a variável onde será armazenado o total de moradores
 * @param totSemTeto  Ponteiro para a variável onde será armazenado o total de pessoas sem teto
 * @param masc        Ponteiro para a variável onde será armazenado o total de homens
 * @param fem         Ponteiro para a variável onde será armazenado o total de mulheres
 * @param morMasc     Ponteiro para a variável onde será armazenado o total de homens moradores
 * @param morFem      Ponteiro para a variável onde será armazenado o total de mulheres moradoras
 * @param semTetoMasc Ponteiro para a variável onde será armazenado o total de homens sem teto
 * @param semTetoFem  Ponteiro para a variável onde será armazenado o total de mulheres sem teto
 * @note              Esta é uma função auxiliar à função produzirCenso() de qry.c 
 * que é responsável por ler o comando do arquivo .qry e chamar esta função para calcular as estatísticas dos habitantes de Bitnópolis.
 */
void calcularEstatisticas(hashPM* dir, int* totHab, int* totMor, int* totSemTeto, 
                            int* masc, int* fem, int* morMasc, int* morFem, 
                            int* semTetoMasc, int* semTetoFem);
/**
 * Esta função é responsável por calcular o número de moradores de uma quadra específica,
 * identificada pelo CEP, percorrendo toda a tabela hash de pessoas (hashPM) para contar o número de moradores da quadra,
 * e a distribuição por face (N, S, L, O).
 * 
 * A função deve iterar sobre todos os buckets da tabela hash, ler os registros de cada bucket, 
 * e verificar se o CEP do registro corresponde ao CEP da quadra especificada.
 * Se o CEP corresponder, a função deve verificar a face do endereço do registro e atualizar as contagens de moradores para cada face 
 * (N, S, L, O) de acordo com as informações do registro.
 * 
 * As estatísticas calculadas incluem:
 * - Total de moradores da quadra
 * - Distribuição por face (N, S, L, O)
 * 
 * @param dir   Ponteiro para o diretório da tabela hash
 * @param cep   CEP da quadra para a qual o número de moradores deve ser calculado
 * @param total Ponteiro para a variável onde será armazenado o total de moradores da quadra
 * @param morN  Ponteiro para a variável onde será armazenado o total de moradores da face N
 * @param morS  Ponteiro para a variável onde será armazenado o total de moradores da face S
 * @param morL  Ponteiro para a variável onde será armazenado o total de moradores da face L
 * @param morO  Ponteiro para a variável onde será armazenado o total de moradores da face O
 * @note        Esta é uma função auxiliar à função calMoradores() de qry.c 
 * que é responsável por ler o comando do arquivo .qry e chamar esta função para calcular o número de moradores da quadra especificada pelo CEP.
 */
void calcularMoradoresQuadra(hashPM* dir, char* cep, int* total, int* morN, int* morS, int* morL, int* morO);
/**
 * Esta função é responsável por "despejar" os moradores de uma quadra específica, ou seja, 
 * remover os dados de moradia (CEP, face, num, compl) dos registros das pessoas que moram na quadra especificada pelo CEP.
 * A função deve iterar sobre todos os buckets da tabela hash, ler os registros de cada bucket 
 * e verificar se o CEP do registro corresponde ao CEP da quadra especificada.
 * Se o CEP corresponder, a função deve remover os dados de moradia (CEP, face, num, compl) do registro da pessoa
 * e salvar o bucket atualizado no disco.
 * 
 * @param dir   Ponteiro para o diretório da tabela hash
 * @param cep   CEP da quadra cujos moradores devem ser despejados
 * @param txt   Ponteiro para o arquivo de saída onde os resultados serão escritos
 */
void despejarMoradoresQuadra(hashPM* dir, char* cep, FILE* txt);

/**
 * Estas são as funções getters e setters para acessar e modificar os campos da estrutura Quadras,
 * permitindo a manipulação dos dados de uma quadra de forma encapsulada.
 * Cada função é responsável por acessar ou modificar um campo específico da estrutura Quadras,
 * como as coordenadas, dimensões, cores, etc., garantindo a integridade dos dados e facilitando a manutenção do código.
 * 
 * @param p Ponteiro para a estrutura de Pessoas cujos campos devem ser acessados ou modificados
 * @return  Valor do campo específico da estrutura de Pessoas. Void para os setters, indicando que o campo foi modificado com sucesso.
 */
char* getPessoaNome     (Pessoas* p);
char* getPessoaSobrenome(Pessoas* p);
char* getPessoaSexo     (Pessoas* p);
char* getPessoaNasc     (Pessoas* p);
char* getPessoaCep      (Pessoas* p);
char* getPessoaCpf      (Pessoas* p);
char* getPessoaFace     (Pessoas* p);
char* getPessoaNum      (Pessoas* p);
char* getPessoaCompl    (Pessoas* p);
void setPessoaEndereco  (Pessoas* p, char* cep, char* face, char* num, char* compl);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função é responsável por criar um novo arquivo binário 
 * e inicializa o Diretório na RAM com os endereços dos buckets no arquivo.
 * 
 * @param nomeArquivo Nome do arquivo binário a ser criado
 * @return            Ponteiro para a tabela hash criada. NULL em caso de erro
 */
hashPM* criarHashPM(const char* nomeArquivo);
/**
 * Esta função é responsável por criar um novo registro do tipo Pessoas, inicializando os campos como nulo.
 * @return Ponteiro para o novo registro criado. NULL em caso de erro
 */
Pessoas* criarPessoa();
/**
 * Esta função é responsável por liberar a memória alocada para a tabela hash e fechar o arquivo binário associado.
 * Fecha o arquivo e limpa o Diretório da RAM, liberando a memória alocada para os endereços dos buckets.
 * 
 * @param dir Ponteiro para o diretório da tabela hash a ser fechado
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
void freeHashPM(hashPM* dir);
/**
 * Esta função é responsável por liberar a memória alocada para o objeto de Pessoas.
 * 
 * @param p Ponteiro para o objeto de Pessoas a ser liberado
 * @return  0 em caso de sucesso. -1 em caso de erro
 */
void freePessoa(Pessoas* p);
/**
 * Esta função é responsável por dividir um bucket que estourou a capacidade de TAM_BUCKET, 
 * redistribuindo os registros entre os buckets e atualizando o diretório da tabela hash.
 * A função deve lidar com colisões utilizando o método de hashing estendido, onde cada bucket pode armazenar múltiplos registros e, 
 * em caso de colisão, o bucket é dividido e os registros são redistribuídos entre os buckets.
 * 
 * @param dir               Ponteiro para o diretório da tabela hash
 * @param indice_dir        Índice do diretório onde a pessoa deve ser inserida
 * @return                  0 em caso de sucesso. -1 em caso de erro
 */
int splitBucketPM(hashPM* dir, int indice_dir);
/**
 * Esta função é responsável por inserir um novo registro do tipo Pessoas na tabela hash,
 * utilizando a chave (CPF) para determinar o bucket onde o registro deve ser armazenado.
 * A função deve lidar com colisões utilizando o método de hashing estendido, onde cada bucket pode armazenar múltiplos registros e, 
 * em caso de colisão, o bucket é dividido e os registros são redistribuídos entre os buckets.
 * 
 * @param dir        Ponteiro para o diretório da tabela hash
 * @param cpf        Chave do registro a ser inserido (CPF)
 * @param nome       Nome da pessoa a ser inserida
 * @param sobrenome  Sobrenome da pessoa a ser inserida
 * @param sexo       Sexo da pessoa a ser inserida
 * @param nasc       Data de nascimento da pessoa a ser inserida
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int inserirRegPM(hashPM* dir, char* cpf, char* nome, char* sobrenome, char* sexo, char* nasc);
/**
 * Esta função é responsável por salvar o diretório da tabela hash em um arquivo binário,
 * permitindo que a estrutura da tabela hash seja persistida e possa ser recarregada posteriormente.
 * A função deve escrever os endereços dos buckets no arquivo, 
 * garantindo que a estrutura da tabela hash seja mantida e possa ser reconstruída quando o arquivo for lido novamente.
 * 
 * @param dir               Ponteiro para o diretório da tabela hash
 * @param nomeArquivoHFC    Nome do arquivo binário onde o diretório deve ser salvo
 * @return                  0 em caso de sucesso. -1 em caso de erro
 */
int salvarDiretorioHFC_PM(hashPM* dir, char* nomeArquivoHFC);
/**
 * Esta função é responsável por carregar o diretório da tabela hash a partir de um arquivo binário,
 * reconstruindo a estrutura da tabela hash na RAM e permitindo que as operações de inserção, 
 * busca e remoção sejam realizadas novamente.
 * A função deve ler os endereços dos buckets do arquivo e armazená-los na RAM, 
 * garantindo que a estrutura da tabela hash seja reconstruída corretamente para que as operações possam ser realizadas.
 * 
 * @param nomeArquivoHFC    Nome do arquivo binário de onde o diretório deve ser carregado
 * @param nomeArquivoHF     Nome do arquivo de dados (.hf) que já existe e deve ser aberto para leitura e escrita
 * @return                  Ponteiro para a tabela hash carregada. NULL em caso de erro
 */
hashPM* carregarDiretorioPM(char* nomeArquivoHFC, char* nomeArquivoHF);
/**
 * Esta função é responsável por gerar um relatório textual (.hfd) da tabela hash,
 * mostrando o conteúdo do diretório e dos buckets de forma legível para o usuário.
 * 
 * @param dir               Ponteiro para o diretório da tabela hash
 * @param nomeArquivoHFD    Nome do arquivo textual onde o relatório deve ser salvo
 * @return                  0 em caso de sucesso. -1 em caso de erro
 */
int gerarRelatorioHFD(hashPM* dir, char* nomeArquivoHFD);
/*###############################################################################################*/

#endif