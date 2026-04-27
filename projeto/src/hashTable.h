#ifndef _HASH_H
#define _HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct quadras Quadras;
typedef struct bucket Bucket;
typedef struct tabelaHash TabelaHash;

/*
    A tabela hash é uma estrutura de dados que permite o armazenamento e a recuperação eficiente de pares chave:valor.
    Ela utiliza uma função de hash para mapear as chaves a índices em um array, onde os valores correspondentes são armazenados.
    As principais operações suportadas pela tabela hash incluem a criação da tabela, inserção de registros, remoção de chaves, 
    busca por valores associados a chaves, entre outras.
    Esta implementação específica da tabela hash é projetada para armazenar registros do tipo "Quadras", 
    que contêm informações como CEP, coordenadas, dimensões e cores.
*/

/*                                       FUNÇÕES AUXILIARES                                      */
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
 * @param indice_dir    Índice do diretório onde a quadra deve ser inserida
 * @param balde_antigo  Referência ao bucket que causou a colisão
 * @return              0 em caso de sucesso. -1 em caso de erro
 */
int duplicarDiretorio(TabelaHash* dir, int indice_dir, Bucket balde_antigo);
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
 * @param indice_dir        Índice do diretório onde a quadra deve ser inserida
 * @param balde_antigo      Referência ao bucket que causou a colisão
 * @param balde_novo        Referência ao novo bucket criado durante o processo de slipBucket
 * @param quadraCausadora   Referência para a quadra que causou a colisão e precisa ser redistribuída
 * @return                  0 em caso de sucesso. -1 em caso de erro
 */
int redistribuirRegistros(TabelaHash* dir, int indice_dir, Bucket* balde_antigo, Bucket* balde_novo, Quadras quadraCausadora, int bit_divisor);
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
int atualizarDiretorio(TabelaHash* dir, long offset_bucket_antigo, long offset_bucket_novo, Bucket* bucket_antigo, Bucket* bucket_novo, int bit_divisor);
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
unsigned int hashFunc(char* key);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Esta função é responsável por criar um novo arquivo binário e inicializa o Diretório na RAM com os endereços dos buckets no arquivo.
 * 
 * @param nomeArquivo Nome do arquivo binário a ser criado
 * @return            Ponteiro para a tabela hash criada. NULL em caso de erro
 */
TabelaHash* criarHash(const char* nomeArquivo);
/**
 * Esta função é responsável por criar um novo registro do tipo Quadras, inicializando os campos como nulo.
 * @return Ponteiro para o novo registro criado. NULL em caso de erro
 */
Quadras* criarQuadra();
/**
 * Esta função é responsável por liberar a memória alocada para a tabela hash e fechar o arquivo binário associado.
 * Fecha o arquivo e limpa o Diretório da RAM, liberando a memória alocada para os endereços dos buckets.
 * 
 * @param dir Ponteiro para o diretório da tabela hash a ser fechado
 * @return    0 em caso de sucesso. -1 em caso de erro
 */
void freeHash(TabelaHash* dir);
/**
 * Esta função é responsável por liberar a memória alocada para o objeto de Quadras.
 * 
 * @param q Ponteiro para o objeto de Quadras a ser liberado
 * @return  0 em caso de sucesso. -1 em caso de erro
 */
void freeQuadra(Quadras* q);
/**
 * Esta função é responsável por dividir um bucket que estourou a capacidade de TAM_BUCKET, 
 * redistribuindo os registros entre os buckets e atualizando o diretório da tabela hash.
 * A função deve lidar com colisões utilizando o método de hashing estendido, onde cada bucket pode armazenar múltiplos registros e, 
 * em caso de colisão, o bucket é dividido e os registros são redistribuídos entre os buckets.
 * 
 * @param dir               Ponteiro para o diretório da tabela hash
 * @param indice_dir        Índice do diretório onde a quadra deve ser inserida
 * @param quadraCausadora   Ponteiro para a quadra que causou a colisão
 * @return                  0 em caso de sucesso. -1 em caso de erro
 */
int splitBucket(TabelaHash* dir, int indice_dir, Quadras quadraCausadora);
/**
 * Esta função é responsável por inserir um novo registro do tipo Quadras na tabela hash,
 * utilizando a chave (CEP) para determinar o bucket onde o registro deve ser armazenado.
 * A função deve lidar com colisões utilizando o método de hashing estendido, onde cada bucket pode armazenar múltiplos registros e, 
 * em caso de colisão, o bucket é dividido e os registros são redistribuídos entre os buckets.
 * 
 * @param dir        Ponteiro para o diretório da tabela hash
 * @param cep        Chave do registro a ser inserido (CEP)
 * @param x          Coordenada x da quadra
 * @param y          Coordenada y da quadra
 * @param w          Largura da quadra
 * @param h          Altura da quadra
 * @param sw         Largura da borda
 * @param cfill      Cor de preenchimento
 * @param cstrk      Cor da borda
 * @return           0 em caso de sucesso. -1 em caso de erro
 */
int inserirReg(TabelaHash* dir, char* cep, double x, double y, double w, double h, double sw, char* cfill, char* cstrk);
/*###############################################################################################*/

#endif