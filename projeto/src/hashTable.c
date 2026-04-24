#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashTable.h"

#define TAM_STRING 32
#define TAM_BUCKET 4

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct quadras{
    char   cep[TAM_STRING];
    double x, y, w, h;
    double sw;
    char   cfill[TAM_STRING];
    char   cstrk[TAM_STRING];
}Quadras;
typedef struct bucket{
    int     prof_local;       // Profundidade Local do Bucket (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 1
    int     qntd_regs;        // Quantidade de Registros atualmente armazenados no bucket
    Quadras regs[TAM_BUCKET]; // Array de Registros do tipo Quadras. Cada bucket pode armazenar até 4 registros (TAM_BUCKET)
}Bucket;
typedef struct tabelaHash{
    int   prof_global; // Profundidade Global da Tabela Hash (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 10
    int   tam_dir;     // Tamanho do Diretório (Número de Buckets) = 2^Profundidade Global
    long* endr_disco;  // Array de endereços dos buckets no arquivo físico da tabela hash
    FILE* arq_hf;      // Ponteiro para o arquivo físico da tabela hash
}TabelaHash;
/*###############################################################################################*/



/************************************** FUNÇÕES AUXILIARES ***************************************/
unsigned int hashFunc(const char* key){

    // Valor inicial recomendado para a função de hash DJB2, criado por Daniel J. Bernstein. 
    // É um número primo que ajuda a distribuir as chaves de forma mais uniforme na tabela hash.
    unsigned int hash = 5381; 

    // Variável para armazenar o valor ASCII do caractere atual da string durante a iteração
    int c;

    while ((c = *key++)){
        // Multiplica o hash atual por 33 e soma o valor ASCII da letra
        // (hash << 5) + hash é equivalente a hash * 33, mas é mais eficiente em termos de operações de bitwise.
        // O número 33 é escolhido porque é um número primo que ajuda a distribuir as chaves
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

bool existeKey(TabelaHash* table, const char* key){
    return 0;
}

Quadras* getRegistro(TabelaHash* table, const char* key){
    return NULL;
}

char* getValue(TabelaHash* table, const char* key){
    return NULL;
}

/*###############################################################################################*/



/*                                        FUNÇÕES PRINCIPAIS                                     */
TabelaHash* criarHash(const char* nomeArquivo){
    // 1: Aloca a estrutura do Diretório na memória RAM
    TabelaHash* dir = (TabelaHash*)malloc(sizeof(TabelaHash));
    if(dir == NULL){
        printf("ERRO: Falha na alocacao da tabela hash.\n");
        return NULL;
    }printf("\t\tTabela hash alocada com sucesso na memoria RAM!\n");

    // 2: Cria/Abre o arquivo físico no disco no modo Binário de Leitura e Escrita
    // "wb+" Cria um arquivo novo limpo
    // "rb+" Abre um arquivo existente para leitura e escrita (mantendo o conteúdo existente)
    dir->arq_hf = fopen(nomeArquivo, "wb+");
    if(dir->arq_hf == NULL){
        printf("ERRO: Falha na abertura do arquivo físico da tabela hash.\n");
        free(dir);
        return NULL;
    }printf("\t\tArquivo fisico da tabela hash criado/aberto com sucesso!\n");

    // 3: Inicializa as regras do Hashing Estendido (Profundidade 1 = 2 espaços)
    // 3.1: Inicializa a Profundidade Global e o Tamanho do Diretório
    dir->prof_global = 1; // Profundidade Global Inicial = 1
    dir->tam_dir     = 2; // Tamanho do Diretório Inicial = 2^Profundidade Global = 2^1 = 2

    // 3.2: Aloca o Diretório na RAM para armazenar os endereços dos buckets no arquivo
    dir->endr_disco  = (long*)malloc(dir->tam_dir * sizeof(long));
    if(dir->endr_disco == NULL){
        printf("ERRO: Falha na alocacao do diretorio da tabela hash.\n");
        fclose(dir->arq_hf);
        free(dir);
        return NULL;
    }printf("\t\tDiretorio da tabela hash alocado com sucesso na memoria RAM!\n");

    // 4: Cria um Balde Vazio padrão na RAM para copiarmos pro disco
    Bucket bucket_vazio;
    bucket_vazio.prof_local = 1; // Profundidade Local Inicial = 1
    bucket_vazio.qntd_regs  = 0; // Quantidade de Registros Inicial = 0

    // 4.1: Zera toda a memória do array de registros para não gravar lixo do C no disco
    // memset é uma função da biblioteca string.h que preenche um bloco de memória com um valor específico.
    // memset(destino, valor, tamanho) -> Preenche o bloco de memória apontado por destino com o valor especificado,
    // por um número de bytes definido por tamanho.
    memset(bucket_vazio.regs, 0, TAM_BUCKET * sizeof(Quadras));

    // 5: Gravando o Primeiro e o Segundo Balde (Índice 0 e 1) no disco
    // ftell é uma função da biblioteca stdio.h que retorna a posição atual do ponteiro de arquivo em bytes a partir do início do arquivo.
    // ftell(arquivo) -> Retorna a posição atual do ponteiro de arquivo em bytes a partir do início do arquivo.
    // fwrite é uma função da biblioteca stdio.h que escreve dados de um buffer para um arquivo.
    // fwrite(buffer, tamanho, quantidade, arquivo) -> Escreve dados do buffer para o arquivo,
    // onde tamanho é o tamanho em bytes de cada elemento a ser escrito, quantidade é o número de elementos a serem escritos,
    // e arquivo é o ponteiro para o arquivo onde os dados serão escritos.

    // 5.1: Armazena o endereço do início do arquivo para o primeiro bucket e escreve o bucket vazio no arquivo
    dir->endr_disco[0] = ftell(dir->arq_hf);
    fwrite(&bucket_vazio, sizeof(Bucket), 1, dir->arq_hf);
    // 5.2: Armazena o endereço do início do arquivo para o segundo bucket e escreve o bucket vazio no arquivo
    dir->endr_disco[1] = ftell(dir->arq_hf);
    fwrite(&bucket_vazio, sizeof(Bucket), 1, dir->arq_hf);

    printf("\t\tArquivo Hash %s inicializado com sucesso!\n", nomeArquivo);

    // 6: Retorna o ponteiro para a tabela hash criada
    return dir;
}

Quadras* criarQuadra(){
    Quadras* q = (Quadras*)malloc(sizeof(Quadras));
    if(q == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Quadras\n");
        return NULL;
    }

    // Inicializa os campos da estrutura Quadras com valores padrão
    strcpy(q->cep, ""); // Inicializa o CEP como string vazia
    q->x = 0.0;         // Inicializa a coordenada x como 0.0
    q->y = 0.0;         // Inicializa a coordenada y como 0.0
    q->w = 0.0;         // Inicializa a largura como 0.0
    q->h = 0.0;         // Inicializa a altura como 0.0
    q->sw = 1.0;        // Inicializa a espessura da borda como 1.0 (valor padrão)
    strcpy(q->cfill, ""); // Inicializa a cor de preenchimento como string vazia
    strcpy(q->cstrk, ""); // Inicializa a cor da borda como string vazia

    return q;
}

void freeHash(TabelaHash* dir){
    if(dir == NULL) return;

    // 1: Fecha o arquivo físico associado à tabela hash
    if(dir->arq_hf != NULL) fclose(dir->arq_hf);

    // 2: Libera a memória alocada para o Diretório da RAM
    if(dir->endr_disco != NULL) free(dir->endr_disco);

    // 3: Libera a memória alocada para a estrutura da tabela hash
    free(dir);
}

void freeQuadra(Quadras* q){
    if(q == NULL) return;
    free(q);
}

int inserirReg(TabelaHash* dir, char* cep, double x, double y, double w, double h, double sw, char* cfill, char* cstrk){
    // 1: Cria um novo registro do tipo Quadras com os dados fornecidos
    Quadras novaQuadra;
    memset(&novaQuadra, 0, sizeof(Quadras)); // Zera a memória para evitar lixo do C

    // 2: Atribui os valores fornecidos para a nova quadra
    strcpy(novaQuadra.cep, cep);        // Copia o CEP para a nova quadra
    novaQuadra.x = x;                   // Atribui a coordenada x
    novaQuadra.y = y;                   // Atribui a coordenada y
    novaQuadra.w = w;                   // Atribui a largura
    novaQuadra.h = h;                   // Atribui a altura
    novaQuadra.sw = sw;                 // Atribui a espessura da borda
    strcpy(novaQuadra.cfill, cfill);    // Copia a cor de preenchimento para a nova quadra
    strcpy(novaQuadra.cstrk, cstrk);    // Copia a cor da borda para a nova quadra

    // 3: Calcula o hash matemático do CEP
    unsigned int hash_val = hashFunc(novaQuadra.cep);

    // 4: Aplica a máscara para descobrir o índice do Diretório (olhando 'p' bits)
    // 4.1: Atribuímos à p, a profundidade global atual da tabela hash
    int p = dir->prof_global;
    // 4.2: Fórmula para obter os últimos 'p' bits do hash (equivalente a hash_val % (2^p))
    unsigned int ult_bits = (1 << p) - 1;
    // 4.3: Índice do Diretório onde a quadra deve ser inserida
    unsigned int indice_dir = hash_val & ult_bits;
    // Exemplo:
    // hash_val = 12345678 (em binário: 101111000110000101001110)
    // p = 3 (Profundidade Global = 3 bits)
    // ult_bits = (1 << 3) - 1 = 8 - 1 = 7 (em binário: 000000000000000000000111)
    // indice_dir = hash_val & ult_bits = 101111000110000101001110 & 000000000000000000000111 = 000000000000000000000110 (em decimal: 6)
    // & => É uma operação bitwise AND que compara cada bit de hash_val com o correspondente bit de ult_bits.
    // O resultado é o valor dos últimos 'p' bits de hash_val, que determina o índice do diretório onde a quadra deve ser inserida.

    // 5: Consulta o Diretório na RAM para saber o endereço real do disco
    long offset = dir->endr_disco[indice_dir];

    // 6: Vai até o bloco no disco e carrega o Balde para a memória
    Bucket balde_atual;
    fseek(dir->arq_hf, offset, SEEK_SET);
    fread(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);

    // 7: Verifica se há espaço neste balde
    if (balde_atual.qntd_regs < TAM_BUCKET) {
        
        // SUCESSO: O balde não está cheio!
        // Copia a nova quadra para o primeiro espaço livre
        int pos = balde_atual.qntd_regs;
        balde_atual.regs[pos] = novaQuadra;
        balde_atual.qntd_regs++; // Aumenta o contador de moradores do balde

        // IMPORTANTE: Volta o ponteiro do disco para o início deste balde e sobrescreve
        fseek(dir->arq_hf, offset, SEEK_SET);
        fwrite(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);
        
        // (Opcional) Print para debug
        // printf("Quadra %s salva no disco (Balde índice %d)\n", novaQuadra.cep, indice_dir);
        
        return 0; // Inserção concluída com sucesso!
        
    } else {
        
        // OVERFLOW: O balde estourou a capacidade de TAM_BUCKET!
        // Neste momento, entra a lógica avançada de divisão (Split) do Hashing Estendido.
        printf("ALERTA: O Balde %d esta cheio! Iniciando SPLIT para a quadra %s...\n", indice_dir, novaQuadra.cep);
        
        // TODO: Chamar função realizarSplit(dir, offset, indice_dir);
        // TODO: Tentar inserirQuadra novamente após o split.
        
        return -1; // Retornando -1 por enquanto, até criarmos o Split.
    }
}

int removerKey(TabelaHash* table, const char* key){
    return 0;
}
/*###############################################################################################*/