#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "params.h"
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
    int     prof_local;         // Profundidade Local do Bucket (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 1
    int     qntd_regs;          // Quantidade de Registros atualmente armazenados no bucket
    Quadras regs[TAM_BUCKET];   // Array de Registros do tipo Quadras. Cada bucket pode armazenar até 4 registros (TAM_BUCKET)
}Bucket;
typedef struct tabelaHash{
    int   prof_global;          // Profundidade Global da Tabela Hash (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 10
    int   tam_dir;              // Tamanho do Diretório (Número de Buckets) = 2^Profundidade Global
    long* endr_disco;           // Array de endereços dos buckets no arquivo físico da tabela hash
    FILE* arq_hf;               // Ponteiro para o arquivo físico da tabela hash
}TabelaHash;
/*###############################################################################################*/



/************************************** FUNÇÕES AUXILIARES ***************************************/
unsigned int hashFunc(char* key){

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

int duplicarDiretorio(TabelaHash* dir, int indice_dir, Bucket bucket_antigo){
    // 1: Verifica se a profundidade local do bucket causador da colisão é igual à profundidade global da tabela hash
    if(bucket_antigo.prof_local == dir->prof_global){
        int tam_antigo = dir->tam_dir;      // Tamanho do diretório antes da duplicação (Número de buckets antes da duplicação)
        int tam_novo   = tam_antigo * 2;    // Tamanho do diretório após a duplicação (Número de buckets após a duplicação)

        // 1.1: Duplica o tamanho do vetor de endereços do diretório na RAM
        dir->endr_disco = (long*)realloc(dir->endr_disco, tam_novo * sizeof(long));
        if(dir->endr_disco == NULL){
            fprintf(stderr, "ERRO: Falha na realocacao do diretorio da tabela hash durante o slipBucket.\n");
            return -1;
        }

        // 1.2: Espelha os endereços dos buckets antigos para os novos índices do diretório
        // Exemplo: Se o diretório antigo tinha 2 buckets (Índices 0 e 1), após a duplicação, 
        // o novo diretório terá 4 buckets (Índices 0, 1, 2 e 3).
        // O bucket do índice 0 será espelhado para o índice 2, e o bucket do índice 1 será espelhado para o índice 3.
        for(int i = 0; i < tam_antigo; i++){
            dir->endr_disco[i + tam_antigo] = dir->endr_disco[i];
            // Exemplo: dir->endr_disco[2] = dir->endr_disco[0]; // O bucket do índice 0 é espelhado para o índice 2
            // Exemplo: dir->endr_disco[3] = dir->endr_disco[1]; // O bucket do índice 1 é espelhado para o índice 3
        }

        // 1.3: Atualiza a Profundidade Global e o Tamanho do Diretório
        dir->tam_dir = tam_novo; // Atualiza o tamanho do diretório para o novo valor (Número de buckets após a duplicação)
        dir->prof_global++;      // Aumenta a profundidade global em 1, pois agora estamos usando mais um bit para calcular o índice do bucket

        printf("DIRETORIO DUPLICADO --> Novo tamanho: %d | Profundidade global: %d\n", dir->tam_dir, dir->prof_global);
    }

    return 0;
}

int redistribuirRegistros(TabelaHash* dir, int indice_dir, Bucket* bucket_antigo, Bucket* bucket_novo, Quadras quadraCausadora, int bit_divisor){
    // 1: Colocamos os 4 registros antigos + 1 novo num buffer temporário
    Quadras buffer[TAM_BUCKET + 1];
    for (int i = 0; i < TAM_BUCKET; i++) buffer[i] = bucket_antigo->regs[i];
    buffer[TAM_BUCKET] = quadraCausadora;
    
    // 2: Esvazia o bucket antigo para preencher de novo com os registros corretos
    bucket_antigo->qntd_regs = 0;
    
    // 4: Redistribuir os registros
    for(int i = 0; i < TAM_BUCKET + 1; i++){
        // 4.1: Calcula o índice do bucket para o registro atual usando a função de hash e o bit divisor
        unsigned int h = hashFunc(buffer[i].cep);
        
        // 4.2: Se o bit da profundidade for 0, fica no balde antigo. Se for 1, vai pro novo balde
        if ((h & bit_divisor) == 0){
            if(bucket_antigo->qntd_regs < TAM_BUCKET) bucket_antigo->regs[bucket_antigo->qntd_regs++] = buffer[i];
            else printf("ERRO FATAL: Colisao severa: O CEP %s foi perdido no Split.\n", buffer[i].cep);
        }else{
            if(bucket_novo->qntd_regs < TAM_BUCKET) bucket_novo->regs[bucket_novo->qntd_regs++] = buffer[i];
            else printf("ERRO FATAL: Colisao severa: O CEP %s foi perdido no Split.\n", buffer[i].cep);
        }
    }

    return 0;
}

int atualizarDiretorio(TabelaHash* dir, long offset_bucket_antigo, long offset_bucket_novo, Bucket* bucket_antigo, Bucket* bucket_novo, int bit_divisor){
    // 1: Procura no diretório todos os ponteiros que apontavam para o bucket_antigo 
    // e que possuem o 'bit_divisor' igual a 1, e muda eles para o bucket_novo
    for (int i = 0; i < dir->tam_dir; i++){
        if (dir->endr_disco[i] == offset_bucket_antigo)
            if ((i & bit_divisor) != 0) dir->endr_disco[i] = offset_bucket_novo;
    }

    // 2: Salva os dois buckets atualizados fisicamente no HD
    fseek(dir->arq_hf, offset_bucket_antigo, SEEK_SET);
    fwrite(bucket_antigo, sizeof(Bucket), 1, dir->arq_hf);
    
    fseek(dir->arq_hf, offset_bucket_novo, SEEK_SET);
    fwrite(bucket_novo, sizeof(Bucket), 1, dir->arq_hf);

    printf("SPLIT CONCLUIDO: Bucket Velho ficou com %d regs, Bucket Novo com %d regs.\n", 
           bucket_antigo->qntd_regs, bucket_novo->qntd_regs);

    return 0;
}

int buscarQuadra(TabelaHash* dir, char* cep, Quadras* resultado){
    // 1: Calcular o Hash e o Índice no diretório
    int valor_hash = hashFunc(cep);

    // 2: Aplicamos a máscara para pegar apenas os bits da prof_global atual
    int indice = valor_hash & ((1 << dir->prof_global) - 1); 
    /**
     * Calcula o índice do diretório usando os últimos 'p' bits do hash do CEP, onde 'p' é a profundidade global da tabela hash. 
     * A expressão (1 << dir->prof_global) - 1 cria uma máscara que tem os últimos 'p' bits definidos como 1 e os demais como 0, 
     * permitindo que apenas os últimos 'p' bits do valor_hash sejam usados para calcular o índice do diretório. 
     * Isso é essencial para garantir que o índice seja calculado corretamente com base na profundidade global da tabela hash, 
     * especialmente após operações de split que podem aumentar a profundidade global.
     */
    
    // 3: Pegar o offset no disco
    long offset = dir->endr_disco[indice];

    // 4: Ler o Bucket do disco
    Bucket b;
    fseek(dir->arq_hf, offset, SEEK_SET);
    fread(&b, sizeof(Bucket), 1, dir->arq_hf);

    // 5: Procurar o CEP dentro do balde (máximo 4 iterações)
    for(int i = 0; i < b.qntd_regs; i++){
        if(strcmp(b.regs[i].cep, cep) == 0){
            *resultado = b.regs[i]; // Copia os dados para o retorno
            return 1;
        }
    }

    return 0;
}

int removerQuadra(TabelaHash* dir, char* cep){
    // 1: Calcular o Hash e o Índice no diretório
    int valor_hash = hashFunc(cep);                             // Calcula o hash do CEP usando a função de hash definida anteriormente
    int indice = valor_hash & ((1 << dir->prof_global) - 1);    // Calcula o índice do diretório usando os últimos 'p' bits do hash do CEP, onde 'p' é a profundidade global da tabela hash

    // 2: Acessa o bucket correspondente ao índice do diretório e lê os registros armazenados no bucket a partir do arquivo físico da tabela hash
    long offset = dir->endr_disco[indice];

    // 3: Declara estrutura para armazenar os dados do bucket lido do disco
    Bucket b; 

    // 4: Posiciona o ponteiro do arquivo no início do bucket correspondente ao índice do diretório
    fseek(dir->arq_hf, offset, SEEK_SET);

    // 5: Lê os dados do bucket do arquivo físico da tabela hash para a estrutura de dados do bucket na memória RAM
    fread(&b, sizeof(Bucket), 1, dir->arq_hf);

    // 6: Procura pelo CEP no bucket atual. Se encontrar, remove a quadra do bucket e salva o bucket atualizado no disco.
    for(int i = 0; i < b.qntd_regs; i++){
        // 6.1: Encontrou a quadra. Agora remove ela do bucket.
        if(strcmp(b.regs[i].cep, cep) == 0){
            // 6.1.1: Substitui o registro da quadra a ser removida pelo último registro do bucket.
            b.regs[i] = b.regs[b.qntd_regs - 1];    // Isso é feito para evitar "buracos" no array de registros do bucket, 
            // mantendo os registros contíguos e facilitando a gestão do espaço no bucket
            
            // 6.1.2: Diminui a quantidade de registros do bucket em 1, pois um registro foi removido 
            b.qntd_regs--;  // Isso é importante para manter o controle correto do número de registros atualmente armazenados no bucket, 
            // garantindo que as operações de inserção e remoção funcionem corretamente 
            // e que o bucket não seja considerado cheio quando na verdade tem espaço disponível após a remoção de um registro

            // 6.1.3: Volta o ponteiro e sobrescreve o balde atualizado no disco
            fseek(dir->arq_hf, offset, SEEK_SET);
            fwrite(&b, sizeof(Bucket), 1, dir->arq_hf);
            return 1;
        }
    }

    // 7: Se não encontrar o CEP no bucket, retorna 0 para indicar que a quadra não foi encontrada para remoção
    return 0;
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

int splitBucket(TabelaHash* dir, int indice_dir, Quadras quadraCausadora){
    // 1: Ler o bucket do disco para a memória
    long offset_bucket_antigo = dir->endr_disco[indice_dir];

    Bucket bucket_antigo;
    fseek(dir->arq_hf, offset_bucket_antigo, SEEK_SET);
    fread(&bucket_antigo, sizeof(Bucket), 1, dir->arq_hf);

    // 2: Duplicar o diretório (Se necessário) | Aumentar a Profundidade Global e o Tamanho do Diretório
    printf("ESTOROU O BUCKET --> Indice do diretorio: %d | Profundidade local do bucket: %d | Profundidade global da tabela hash: %d\n", 
        indice_dir, bucket_antigo.prof_local, dir->prof_global);
    if(duplicarDiretorio(dir, indice_dir, bucket_antigo) != 0){
        fprintf(stderr, "ERRO: Falha ao duplicar o diretorio durante o slipBucket.\n");
        return -1;
    }

    // 3: Criar um novo bucket vazio no final do arquivo para armazenar os registros que serão redistribuídos
    Bucket bucket_novo;
    memset(&bucket_novo, 0, sizeof(Bucket));                // Zera a memória para evitar lixo do C
    bucket_novo.prof_local = bucket_antigo.prof_local + 1;  // Aumenta a profundidade local para o novo bucket

    // 3.1: Incrementa a profundidade de ambomos os buckets (antigo e novo)
    bucket_antigo.prof_local++;
    bucket_novo.prof_local = bucket_antigo.prof_local;

    // 3.2: Busca a posição física do novo bucket
    fseek(dir->arq_hf, 0, SEEK_END);                // Move o ponteiro para o final do arquivo
    long offset_bucket_novo = ftell(dir->arq_hf);   // Obtém o offset do novo bucket (posição atual do ponteiro no final do arquivo)

    // O bit que define quem vai pra onde (é sempre 1 << (profundidade_local_nova - 1))
    int bit_divisor = 1 << (bucket_antigo.prof_local - 1);

    // 4: Redistribuir os registros do bucket antigo entre o bucket antigo e o novo bucket, de acordo com a nova profundidade local
    printf("REDISTRIBUINDO OS REGISTROS...\n");
    if(redistribuirRegistros(dir, indice_dir, &bucket_antigo, &bucket_novo, quadraCausadora, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao redistribuir os registros durante o slipBucket.\n");
        return -1;
    }
    printf("REGISTROS REDISTRIBUIDOS COM SUCESSO!\n");

    // 5: Atualizar o diretório para apontar para os buckets correto (antigo e novo) de acordo com a nova profundidade local
    printf("ATUALIZANDO O DIRETORIO...\n");
    if(atualizarDiretorio(dir, offset_bucket_antigo, offset_bucket_novo, &bucket_antigo, &bucket_novo, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao atualizar o diretorio durante o slipBucket.\n");
        return -1;
    }
    printf("DIRETORIO ATUALIZADO COM SUCESSO!\n\n");

    return 0;
}

int inserirReg(TabelaHash* dir, char* cep, double x, double y, double w, double h, double sw, char* cfill, char* cstrk){
    // 1: Cria um novo registro do tipo Quadras com os dados fornecidos
    Quadras novaQuadra;                         // Cria uma variável do tipo Quadras para armazenar os dados da nova quadra a ser inserida
    memset(&novaQuadra, 0, sizeof(Quadras));    // Zera a memória para evitar lixo do C

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
    Bucket balde_atual;                                     // Cria uma variável do tipo Bucket para armazenar os dados do bucket lido do disco
    fseek(dir->arq_hf, offset, SEEK_SET);                   // Move o ponteiro do arquivo para o offset do bucket onde a quadra deve ser inserida
    fread(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);    // Lê os dados do bucket do disco e armazena na variável balde_atual

    // 7: Verifica se há espaço neste balde
    if(balde_atual.qntd_regs < TAM_BUCKET){
        // SUCESSO: O balde não está cheio
        // Copia a nova quadra para o primeiro espaço livre
        int pos = balde_atual.qntd_regs;    // Posição do próximo espaço livre no array de registros do bucket (inicialmente 0, depois 1, 2, etc.)
        balde_atual.regs[pos] = novaQuadra; // Copia a nova quadra para o primeiro espaço livre do array de registros do bucket
        balde_atual.qntd_regs++;            // Aumenta o contador de moradores do balde

        // IMPORTANTE: Volta o ponteiro do disco para o início deste balde e sobrescreve
        fseek(dir->arq_hf, offset, SEEK_SET);
        fwrite(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);        
        printf("Quadra %s salva no disco (Balde indice %d)\n", novaQuadra.cep, indice_dir);
        
        return 0;        
    }else{
        // OVERFLOW: O balde estourou a capacidade de TAM_BUCKET
        printf("\n");
        printf("ALERTA: O Balde %d esta cheio! Iniciando SPLIT...\n", indice_dir);        
        splitBucket(dir, indice_dir, novaQuadra);

        return 0;
    }
}

int salvarDiretorioHFC(TabelaHash* dir, char* nomeArquivoHFC){
    // 1: Abre o arquivo de diretório para escrita em modo binário
    FILE* f = fopen("quadras.hfc", "wb");
    if(f == NULL){
        printf("ERRO: Nao foi possivel criar o arquivo quadras.hfc\n");
        free(f);
        return -1;
    }

    // 2: Grava os dados (Profundidade e Tamanho)
    fwrite(&(dir->prof_global), sizeof(int), 1, f);
    fwrite(&(dir->tam_dir), sizeof(int), 1, f);

    // 3: Grava o vetor inteiro de uma só vez
    fwrite(dir->endr_disco, sizeof(long), dir->tam_dir, f);

    // 4: Fecha o arquivo e retorna sucesso
    fclose(f);
    printf("Diretorio salvo com sucesso no arquivo quadras.hfc!\n");

    return 0;
}

TabelaHash* carregarDiretorioHFC(char* nomeArquivoHFC, char* nomeArquivoHF){
    // 1: Abre o arquivo de diretório para leitura em modo binário
    FILE* f = fopen(nomeArquivoHFC, "rb");
    if(f == NULL){
        printf("ERRO: Nao foi possivel abrir o arquivo quadras.hfc\n");
        free(f);
        return NULL;
    }

    // 2: Aloca a estrutura do Diretório na memória RAM
    TabelaHash* dir = malloc(sizeof(TabelaHash));
    if(dir == NULL){
        printf("ERRO: Nao foi possivel alocar memoria para o diretorio\n");
        free(dir);
        return NULL;
    }
    
    // 3: Lê a profundidade global, o tamanho do diretório e os endereços dos buckets do arquivo para a estrutura na RAM
    // fread(&variavel_destino, tamanho_de_cada_elemento, quantidade_de_elementos, arquivo)
    fread(&(dir->prof_global), sizeof(int), 1, f);
    fread(&(dir->tam_dir), sizeof(int), 1, f);

    // 4: Aloca o vetor de endereços dos buckets na RAM
    dir->endr_disco = malloc(sizeof(long) * dir->tam_dir);
    if(dir->endr_disco == NULL){
        printf("ERRO: Nao foi possivel alocar memoria para o vetor de enderecos\n");
        free(dir->endr_disco);
        free(dir);
        return NULL;
    }

    // 5: Lê os endereços dos buckets do arquivo para o vetor de endereços na RAM
    fread(dir->endr_disco, sizeof(long), dir->tam_dir, f);
    
    // 6: Abre o arquivo de dados (.hf) que já existe
    dir->arq_hf = fopen(nomeArquivoHF, "rb+"); 

    // 7: Fecha o arquivo de diretório e retorna o ponteiro para a estrutura do diretório carregada na RAM
    fclose(f);
    return dir;
}
/*###############################################################################################*/