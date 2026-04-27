
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashPM.h"

#define TAM_STRING 32
#define TAM_BUCKET 4

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct pessoas{
    char cpf[20];

    char nome[50];
    char sobrenome[50];
    char sexo[5]; // String para evitar problemas de leitura com sscanf
    char nasc[20];

    char cep[20];
    char face[5]; // String para evitar problemas de leitura com sscanf
    char num[10];
    char compl[50];
}Pessoas;
typedef struct bucket{
    int     prof_local;         // Profundidade Local do Bucket (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 1
    int     qntd_regs;          // Quantidade de Registros atualmente armazenados no bucket
    Pessoas regs[TAM_BUCKET];   // Array de Registros do tipo Pessoas. Cada bucket pode armazenar até 4 registros (TAM_BUCKET)
}Bucket;
typedef struct hashPM{
    int   prof_global;          // Profundidade Global da Tabela Hash (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 10
    int   tam_dir;              // Tamanho do Diretório (Número de Buckets) = 2^Profundidade Global
    long* endr_disco;           // Array de endereços dos buckets no arquivo físico da tabela hash
    FILE* arq_hf;               // Ponteiro para o arquivo físico da tabela hash
}hashPM;
/*###############################################################################################*/



/************************************** FUNÇÕES AUXILIARES ***************************************/
unsigned int hashFuncPM(char* key){

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

int duplicarDiretorioPM(hashPM* dir, int indice_dir, Bucket bucket_antigo){
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

int redistribuirRegistrosPM(hashPM* dir, int indice_dir, Bucket* bucket_antigo, Bucket* bucket_novo, Pessoas pessoaCausadora, int bit_divisor){
    // 1: Colocamos os 4 registros antigos + 1 novo num buffer temporário
    Pessoas buffer[TAM_BUCKET + 1];
    for (int i = 0; i < TAM_BUCKET; i++) buffer[i] = bucket_antigo->regs[i];
    buffer[TAM_BUCKET] = pessoaCausadora;
    
    // 2: Esvazia o bucket antigo para preencher de novo com os registros corretos
    bucket_antigo->qntd_regs = 0;
    
    // 4: Redistribuir os registros
    for(int i = 0; i < TAM_BUCKET + 1; i++){
        // 4.1: Calcula o índice do bucket para o registro atual usando a função de hash e o bit divisor
        unsigned int h = hashFuncPM(buffer[i].cpf);
        
        // 4.2: Se o bit da profundidade for 0, fica no balde antigo. Se for 1, vai pro novo balde
        if ((h & bit_divisor) == 0){
            if(bucket_antigo->qntd_regs < TAM_BUCKET) bucket_antigo->regs[bucket_antigo->qntd_regs++] = buffer[i];
            else printf("ERRO FATAL: Colisao severa: O CPF %s foi perdido no Split.\n", buffer[i].cpf);
        }else{
            if(bucket_novo->qntd_regs < TAM_BUCKET) bucket_novo->regs[bucket_novo->qntd_regs++] = buffer[i];
            else printf("ERRO FATAL: Colisao severa: O CPF %s foi perdido no Split.\n", buffer[i].cpf);
        }
    }

    return 0;
}

int atualizarDiretorioPM(hashPM* dir, long offset_bucket_antigo, long offset_bucket_novo, Bucket* bucket_antigo, Bucket* bucket_novo, int bit_divisor){
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

int adicionarMoradia(hashPM* dir, char* cpf, char* cep, char* face, char* num, char* compl){
    // 1: Calcula o índice do diretório usando a função de hash e a profundidade global da tabela hash
    unsigned int hash_val = hashFuncPM(cpf);          // Calcula o hash do CPF usando a função de hash definida anteriormente
    int p = dir->prof_global;                       // Obtém a profundidade global da tabela hash para determinar quantos bits usar para calcular o índice do bucket
    unsigned int ult_bits = (1 << p) - 1;           // Calcula uma máscara para obter os últimos 'p' bits do hash do CPF, onde 'p' é a profundidade global da tabela hash
    unsigned int indice_dir = hash_val & ult_bits;  // Calcula o índice do diretório usando os últimos 'p' bits do hash do CPF, onde 'p' é a profundidade global da tabela hash

    // 2: Acessa o bucket correspondente ao índice do diretório e lê os registros armazenados no bucket a partir do arquivo físico da tabela hash
    long offset = dir->endr_disco[indice_dir];
    Bucket balde_atual;
    fseek(dir->arq_hf, offset, SEEK_SET);
    fread(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);

    // 3: Procura pelo CPF no bucket atual. Se encontrar, atualiza os dados de moradia (CEP, face, num, compl) e salva o bucket atualizado no disco.
    for(int i = 0; i < balde_atual.qntd_regs; i++){
        
        if(strcmp(balde_atual.regs[i].cpf, cpf) == 0){ 
            // SUCESSO: Encontrou o habitante. Agora ele vira morador.
            strcpy(balde_atual.regs[i].cep, cep);
            strcpy(balde_atual.regs[i].face, face);
            strcpy(balde_atual.regs[i].num, num);
            strcpy(balde_atual.regs[i].compl, compl);

            // IMPORTANTE: Volta o ponteiro e sobrescreve o balde atualizado no disco
            fseek(dir->arq_hf, offset, SEEK_SET);
            fwrite(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);
            
            printf("Habitante %s agora e morador no CEP %s\n", cpf, cep);
            return 0;
        }
    }

    // 4: Não encontrou o habitante com o CPF fornecido. Impossível adicionar moradia.
    printf("ALERTA: CPF %s nao encontrado. Impossivel adicionar moradia.\n", cpf);
    return -1;
}
/*###############################################################################################*/



/*                                        FUNÇÕES PRINCIPAIS                                     */
hashPM* criarHashPM(const char* nomeArquivo){
    // 1: Aloca a estrutura do Diretório na memória RAM
    hashPM* dir = (hashPM*)malloc(sizeof(hashPM));
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
    memset(bucket_vazio.regs, 0, TAM_BUCKET * sizeof(Pessoas));

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

Pessoas* criarPessoa(){
    Pessoas* p = (Pessoas*)malloc(sizeof(Pessoas));
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Pessoas\n");
        return NULL;
    }

    // Inicializa os campos da estrutura Pessoas com valores padrão
    strcpy(p->cpf,  "");        // Inicializa o CPF como string vazia
    strcpy(p->nome, "");        // Inicializa o nome como string vazia
    strcpy(p->sobrenome, "");   // Inicializa a sobrenome como string vazia
    strcpy(p->sexo, "");        // Inicializa a sexo como string vazia
    strcpy(p->nasc, "");        // Inicializa a data de nascimento como string vazia

    return p;
}

void freeHashPM(hashPM* dir){
    if(dir == NULL) return;

    // 1: Fecha o arquivo físico associado à tabela hash
    if(dir->arq_hf != NULL) fclose(dir->arq_hf);

    // 2: Libera a memória alocada para o Diretório da RAM
    if(dir->endr_disco != NULL) free(dir->endr_disco);

    // 3: Libera a memória alocada para a estrutura da tabela hash
    free(dir);
}

void freePessoa(Pessoas* p){
    if(p == NULL) return;
    free(p);
}

int splitBucketPM(hashPM* dir, int indice_dir, Pessoas pessoaCausadora){
    // 1: Ler o bucket do disco para a memória
    long offset_bucket_antigo = dir->endr_disco[indice_dir];

    Bucket bucket_antigo;
    fseek(dir->arq_hf, offset_bucket_antigo, SEEK_SET);
    fread(&bucket_antigo, sizeof(Bucket), 1, dir->arq_hf);

    // 2: Duplicar o diretório (Se necessário) | Aumentar a Profundidade Global e o Tamanho do Diretório
    printf("ESTOROU O BUCKET --> Indice do diretorio: %d | Profundidade local do bucket: %d | Profundidade global da tabela hash: %d\n", 
        indice_dir, bucket_antigo.prof_local, dir->prof_global);
    if(duplicarDiretorioPM(dir, indice_dir, bucket_antigo) != 0){
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
    if(redistribuirRegistrosPM(dir, indice_dir, &bucket_antigo, &bucket_novo, pessoaCausadora, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao redistribuir os registros durante o slipBucket.\n");
        return -1;
    }
    printf("REGISTROS REDISTRIBUIDOS COM SUCESSO!\n");

    // 5: Atualizar o diretório para apontar para os buckets correto (antigo e novo) de acordo com a nova profundidade local
    printf("ATUALIZANDO O DIRETORIO...\n");
    if(atualizarDiretorioPM(dir, offset_bucket_antigo, offset_bucket_novo, &bucket_antigo, &bucket_novo, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao atualizar o diretorio durante o slipBucket.\n");
        return -1;
    }
    printf("DIRETORIO ATUALIZADO COM SUCESSO!\n\n");

    return 0;
}

int inserirRegPM(hashPM* dir, char* cpf, char* nome, char* sobrenome, char* sexo, char* nasc){
    // 1: Cria um novo registro do tipo Pessoas com os dados fornecidos
    Pessoas novaPessoa;                         // Cria uma variável do tipo Pessoas para armazenar os dados da nova pessoa a ser inserida
    memset(&novaPessoa, 0, sizeof(Pessoas));    // Zera a memória para evitar lixo do C

    // 2: Atribui os valores fornecidos para a nova pessoa
    strcpy(novaPessoa.cpf, cpf);                // Copia o CPF para a nova pessoa
    strcpy(novaPessoa.nome, nome);              // Copia o nome para a nova pessoa
    strcpy(novaPessoa.sobrenome, sobrenome);    // Copia o sobrenome para a nova pessoa
    strcpy(novaPessoa.sexo, sexo);              // Copia o sexo para a nova pessoa
    strcpy(novaPessoa.nasc, nasc);              // Copia a data de nascimento para a nova pessoa

    // 3: Calcula o hash matemático do CPF
    unsigned int hash_val = hashFuncPM(novaPessoa.cpf);

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
        balde_atual.regs[pos] = novaPessoa; // Copia a nova quadra para o primeiro espaço livre do array de registros do bucket
        balde_atual.qntd_regs++;            // Aumenta o contador de moradores do balde

        // IMPORTANTE: Volta o ponteiro do disco para o início deste balde e sobrescreve
        fseek(dir->arq_hf, offset, SEEK_SET);
        fwrite(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);        
        
        return 0;        
    }else{
        // OVERFLOW: O balde estourou a capacidade de TAM_BUCKET
        printf("\n");
        printf("ALERTA: O Balde %d esta cheio! Iniciando SPLIT...\n", indice_dir);        
        splitBucketPM(dir, indice_dir, novaPessoa);

        return 0;
    }
}
/*###############################################################################################*/