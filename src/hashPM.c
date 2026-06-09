
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashPM.h"

#define TAM_STRING 32
#define TAM_BUCKET 5

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct pessoas{
    char cpf[25];

    char nome[50];
    char sobrenome[50];
    char sexo[5]; // String para evitar problemas de leitura com sscanf
    char nasc[20];

    char cep[25];
    char face[5]; // String para evitar problemas de leitura com sscanf
    char num[10];
    char compl[50];
}Pessoas;
typedef struct bucket{
    int     prof_local;         // Profundidade Local do Bucket (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 1
    int     qntd_regs;          // Quantidade de Registros atualmente armazenados no bucket
    Pessoas regs[TAM_BUCKET];   // Array de Registros do tipo Pessoas. Cada bucket pode armazenar até 5 registros (TAM_BUCKET)
}Bucket;
typedef struct hashPM{
    int   prof_global;          // Profundidade Global da Tabela Hash (Número de bits utilizados para calcular o índice do bucket) -> Inicialmente 10
    int   tam_dir;              // Tamanho do Diretório (Número de Buckets) = 2^Profundidade Global
    long* endr_disco;           // Array de endereços dos buckets no arquivo físico da tabela hash
    FILE* arq_hf;               // Ponteiro para o arquivo físico da tabela hash
}hashPM;
/*###############################################################################################*/



/************************************** FUNÇÕES AUXILIARES ***************************************/
/**
 *  - O algoritmo FNV-1a é uma variação do algoritmo FNV (Fowler-Noll-Vo) 
 * que é amplamente utilizado para hashing de strings devido à sua simplicidade e eficiência.
 *  - Ele é projetado para ser rápido e fornecer uma boa distribuição de hash, 
 * o que ajuda a reduzir a probabilidade de colisões em tabelas hash.
 *  - O FNV-1a é particularmente eficaz para strings que são semelhantes, como CPFs que diferem apenas em um dígito, 
 * pois pequenas mudanças na string resultam em grandes mudanças no hash, graças à operação XOR.
 */
unsigned int hashFuncPM(char* key){
    // 1. Algoritmo FNV-1a
    unsigned int hash = 2166136261u; // offset do FNV-1a (um número primo grande recomendado para um bom espalhamento inicial)
    int c;
    
    // 2: Itera sobre cada caractere da string até o final (quando c se torna 0, ou seja, o caractere nulo de terminação da string)
    while ((c = (unsigned char)*key++)){ // O (unsigned char) impede bugs com caracteres estranhos
        /**
         *  - XOR com o byte atual da string para misturar os bits do hash, 
         * garantindo que pequenas mudanças na string resultem em grandes mudanças no hash.
         *  - O operador XOR (^) é uma operação bitwise que compara cada bit do hash atual com o correspondente bit do byte atual da string (c).
         *  - Se os bits forem iguais, o resultado é 0; se forem diferentes, o resultado é 1. 
         *  - Isso ajuda a misturar os bits do hash de forma eficaz, 
         * garantindo que pequenas mudanças na string resultem em grandes mudanças no hash, 
         * o que é desejável para reduzir a probabilidade de colisões na tabela hash.
         * 
         * Exemplo: 
         * hash = 2166136261 (em binário: 10000001011111000010110101100101);
         * c    =    '1'     (em binário: 00110001);
         * hash ^= c => 
         * 10000001011111000010110101100101 XOR 00000000000000000000000000110001 => 
         * 10000001011111000010110101010100 (em decimal: 2166136228)
         */
        // 2.1: XOR com o byte atual da string para misturar os bits do hash
         hash ^= c;
        
        // 2.2: Multiplica o hash pelo número primo para espalhar os bits de forma mais uniforme, reduzindo a probabilidade de colisões
        hash *= 16777619; // Número primo de FNV recomendado para a multiplicação no algoritmo FNV-1a. Ele é escolhido porque ajuda a espalhar os bits do hash de forma mais uniforme, reduzindo a probabilidade de colisões
    }

    // 3: Finalizador MurmurHash3 (A "Avalanche")
    // Mistura os bits para garantir que os últimos sejam sempre aleatórios e únicos, mesmo para CPFs quase iguais
    hash ^= hash >> 16; // Realiza um XOR entre o hash atual e o hash deslocado 16 bits para a direita, misturando os bits do hash para garantir uma melhor distribuição e reduzir a probabilidade de colisões, especialmente para strings semelhantes como CPFs que diferem apenas em um dígito
    hash *= 0x85ebca6b; // Multiplica o hash por um número primo para espalhar os bits de forma mais uniforme, reduzindo a probabilidade de colisões. O número 0x85ebca6b é um número primo recomendado para a multiplicação no finalizador MurmurHash3, que é uma etapa adicional para misturar os bits do hash e garantir uma melhor distribuição, especialmente para strings semelhantes como CPFs que diferem apenas em um dígito
    hash ^= hash >> 13; // Realiza um XOR entre o hash atual e o hash deslocado 13 bits para a direita, misturando ainda mais os bits do hash
    hash *= 0xc2b2ae35; // Multiplica o hash por outro número primo 
    hash ^= hash >> 16; // Realiza um XOR final entre o hash atual e o hash deslocado 16 bits para a direita

    // 4: Retorna o hash calculado para a string fornecida
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

        // printf("DIRETORIO DUPLICADO --> Novo tamanho: %d | Profundidade global: %d\n", dir->tam_dir, dir->prof_global);
    }

    return 0;
}

int redistribuirRegistrosPM(hashPM* dir, int indice_dir, Bucket* bucket_antigo, Bucket* bucket_novo, int bit_divisor){
    // 1: Colocamos os 5 registros antigos num buffer temporário
    Pessoas buffer[TAM_BUCKET];
    for(int i = 0; i < TAM_BUCKET; i++) buffer[i] = bucket_antigo->regs[i];

    // 2: Esvazia o bucket antigo para preencher de novo com os registros corretos
    bucket_antigo->qntd_regs = 0;
    
    // 3: Redistribuir os 5 registros
    for(int i = 0; i < TAM_BUCKET; i++){
        // 4.1: Calcula o índice do bucket para o registro atual usando a função de hash e o bit divisor
        unsigned int h = hashFuncPM(buffer[i].cpf);
        
        // 4.2: Verifica o bit divisor para determinar se o registro deve permanecer no bucket antigo ou ser movido para o novo bucket
        if((h & bit_divisor) == 0) bucket_antigo->regs[bucket_antigo->qntd_regs++] = buffer[i];
        else                       bucket_novo->regs[bucket_novo->qntd_regs++]     = buffer[i];
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

    // printf("SPLIT CONCLUIDO: Bucket Velho ficou com %d regs, Bucket Novo com %d regs.\n", 
    //        bucket_antigo->qntd_regs, bucket_novo->qntd_regs);

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

            // IMPORTANTE: Volta o ponteiro e sobrescreve o bucket atualizado no disco
            fseek(dir->arq_hf, offset, SEEK_SET);
            fwrite(&balde_atual, sizeof(Bucket), 1, dir->arq_hf);
            
            // printf("Habitante %s agora e morador no CEP %s\n", cpf, cep);
            return 0;
        }
    }

    // 4: Não encontrou o habitante com o CPF fornecido. Impossível adicionar moradia.
    // printf("ALERTA: CPF %s nao encontrado. Impossivel adicionar moradia.\n", cpf);
    return -1;
}

int buscarPessoa(hashPM* dir, char* cpf, Pessoas* resultado){
    // 1: Calcular o Hash e o Índice no diretório
    int valor_hash = hashFuncPM(cpf);

    // 2: Aplicamos a máscara para pegar apenas os bits da prof_global atual
    int indice = valor_hash & ((1 << dir->prof_global) - 1); 
    /**
     * Calcula o índice do diretório usando os últimos 'p' bits do hash do CPF, onde 'p' é a profundidade global da tabela hash. 
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

    // 5: Procurar o CPF dentro do balde (máximo 4 iterações)
    for(int i = 0; i < b.qntd_regs; i++){
        if(strcmp(b.regs[i].cpf, cpf) == 0){
            *resultado = b.regs[i]; // Copia os dados para o retorno
            return 1;
        }
    }

    return 0;
}

int atualizarPessoa(hashPM* dir, Pessoas* pessoaAtualizada){
    // 1: Calcular o Hash e o Índice no diretório
    int valor_hash = hashFuncPM(pessoaAtualizada->cpf);          // Calcula o hash do CPF usando a função de hash definida anteriormente
    int indice = valor_hash & ((1 << dir->prof_global) - 1);    // Calcula o índice do diretório usando os últimos 'p' bits do hash do CPF, onde 'p' é a profundidade global da tabela hash
    
    // 2: Acessa o bucket correspondente ao índice do diretório e lê os registros armazenados no bucket a partir do arquivo físico da tabela hash
    long offset = dir->endr_disco[indice];

    // 3: Procura pelo CPF no bucket atual. Se encontrar, atualiza os dados da pessoa e salva o bucket atualizado no disco.
    Bucket b;
    fseek(dir->arq_hf, offset, SEEK_SET);
    fread(&b, sizeof(Bucket), 1, dir->arq_hf);

    // 4: Procura pelo CPF no bucket atual. Se encontrar, atualiza os dados da pessoa e salva o bucket atualizado no disco.
    for(int i = 0; i < b.qntd_regs; i++){
        if(strcmp(b.regs[i].cpf, pessoaAtualizada->cpf) == 0){
            // 4.1: Encontrou a pessoa. Agora atualiza os dados dela com os dados da pessoaAtualizada
            b.regs[i] = *pessoaAtualizada; 
            
            // 4.2: Volta o ponteiro e sobrescreve o balde atualizado no disco
            fseek(dir->arq_hf, offset, SEEK_SET);
            fwrite(&b, sizeof(Bucket), 1, dir->arq_hf);
            
            return 1;
        }
    }
    return 0;
}

int removerPessoa(hashPM* dir, char* cpf){
    // 1: Calcular o Hash e o Índice no diretório
    int valor_hash = hashFuncPM(cpf);                           // Calcula o hash do CPF usando a função de hash definida anteriormente
    int indice = valor_hash & ((1 << dir->prof_global) - 1);    // Calcula o índice do diretório usando os últimos 'p' bits do hash do CPF, onde 'p' é a profundidade global da tabela hash

    // 2: Acessa o bucket correspondente ao índice do diretório e lê os registros armazenados no bucket a partir do arquivo físico da tabela hash
    long offset = dir->endr_disco[indice];

    // 3: Declara estrutura para armazenar os dados do bucket lido do disco
    Bucket b; 

    // 4: Posiciona o ponteiro do arquivo no início do bucket correspondente ao índice do diretório
    fseek(dir->arq_hf, offset, SEEK_SET);

    // 5: Lê os dados do bucket do arquivo físico da tabela hash para a estrutura de dados do bucket na memória RAM
    fread(&b, sizeof(Bucket), 1, dir->arq_hf);

    // 6: Procura pelo CPF no bucket atual. Se encontrar, remove a pessoa do bucket e salva o bucket atualizado no disco.
    for(int i = 0; i < b.qntd_regs; i++){
        // 6.1: Encontrou a pessoa. Agora remove ela do bucket.
        if(strcmp(b.regs[i].cpf, cpf) == 0){
            // 6.1.1: Substitui o registro da pessoa a ser removida pelo último registro do bucket.
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
    return 0;
}

void calcularEstatisticas(hashPM* dir, int* totHab, int* totMor, int* totSemTeto, 
                            int* masc, int* fem, int* morMasc, int* morFem, 
                            int* semTetoMasc, int* semTetoFem){
    
    // 1: Inicializa todas as estatísticas com zero
    *totHab = 0; *totMor = 0; *totSemTeto = 0; *masc = 0; *fem = 0; *morMasc = 0; *morFem = 0; *semTetoMasc = 0; *semTetoFem = 0;

    // 2: Itera por todos os buckets do diretório para calcular as estatísticas
    for(int i = 0; i < dir->tam_dir; i++){
        bool primeiro_visto = true; 
        // Flag para verificar se é a primeira vez que vemos esse bucket, 
        // para evitar contar o mesmo bucket mais de uma vez devido ao espelhamento no diretório
        
        // 2.1: Verifica se o bucket do índice atual já foi visto antes no diretório 
        // (devido ao espelhamento dos buckets no diretório, vários índices podem apontar para o mesmo bucket no disco)
        for(int j = 0; j < i; j++){
            if(dir->endr_disco[i] == dir->endr_disco[j]){
                primeiro_visto = false;
                break;
            }
        }

        // 2.2: Se for a primeira vez que vemos esse bucket, lemos os registros do bucket do disco e atualizamos as estatísticas
        if(primeiro_visto){
            Bucket b;                                           // Declaração de uma variável do tipo Bucket para armazenar os dados lidos do disco
            fseek(dir->arq_hf, dir->endr_disco[i], SEEK_SET);   // Posiciona o ponteiro do arquivo no início do bucket correspondente ao índice do diretório
            fread(&b, sizeof(Bucket), 1, dir->arq_hf);          // Lê os dados do bucket do arquivo físico da tabela hash para a estrutura de dados do bucket na memória RAM

            // 2.2.1: Itera por todos os registros do bucket para atualizar as estatísticas
            for(int r = 0; r < b.qntd_regs; r++){
                Pessoas p = b.regs[r];
                
                (*totHab)++;
                bool eMorador = (strlen(p.cep) > 0);

                if(eMorador) (*totMor)++;
                else         (*totSemTeto)++;

                // Atualiza as estatísticas de gênero para moradores e sem-teto
                if(strcmp(p.sexo, "M") == 0){
                    (*masc)++;
                    if(eMorador) (*morMasc)++;
                    else         (*semTetoMasc)++;
                } 
                
                else{
                    (*fem)++;
                    if(eMorador) (*morFem)++;
                    else         (*semTetoFem)++;                
                }
            }
        }
    }
}

void calcularMoradoresQuadra(hashPM* dir, char* cep, int* total, int* morN, int* morS, int* morL, int* morO){
    // 1: Inicializa as contagens de moradores para cada face com zero
    *total = 0; *morN = 0; *morS = 0; *morL = 0; *morO = 0;

    // 2: Itera por todos os buckets do diretório para encontrar os moradores da quadra especificada pelo CEP
    for(int i = 0; i < dir->tam_dir; i++){
        bool primeiro_visto = true; 
        // Flag para verificar se é a primeira vez que vemos esse bucket, 
        // para evitar contar o mesmo bucket mais de uma vez devido ao espelhamento no diretório
        
        // 2.1: Verifica se o bucket do índice atual já foi visto antes no diretório 
        // (devido ao espelhamento dos buckets no diretório, vários índices podem apontar para o mesmo bucket no disco)
        for(int j = 0; j < i; j++){
            if(dir->endr_disco[i] == dir->endr_disco[j]){
                primeiro_visto = false;
                break;
            }
        }

        // 2.2: Se for a primeira vez que vemos esse bucket, lemos os registros do bucket do disco e verificamos se são moradores da quadra especificada pelo CEP
        if(primeiro_visto){
            Bucket b;                                           // Declaração de uma variável do tipo Bucket para armazenar os dados lidos do disco
            fseek(dir->arq_hf, dir->endr_disco[i], SEEK_SET);   // Posiciona o ponteiro do arquivo no início do bucket correspondente ao índice do diretório
            fread(&b, sizeof(Bucket), 1, dir->arq_hf);          // Lê os dados do bucket do arquivo físico da tabela hash para a estrutura de dados do bucket na memória RAM

            // 2.2.1: Itera por todos os registros do bucket para verificar se são moradores da quadra especificada pelo CEP
            for(int r = 0; r < b.qntd_regs; r++){
                Pessoas p = b.regs[r];
                
                if(strcmp(p.cep, cep) == 0){
                    (*total)++;
                    if     (strcmp(p.face, "N") == 0) (*morN)++;
                    else if(strcmp(p.face, "S") == 0) (*morS)++;
                    else if(strcmp(p.face, "L") == 0) (*morL)++;
                    else if(strcmp(p.face, "O") == 0) (*morO)++;
                }
            }
        }
    }
}

void despejarMoradoresQuadra(hashPM* dir, char* cep, FILE* txt){
    // 1: Itera por todos os buckets do diretório para encontrar os moradores da quadra especificada pelo CEP e despejá-los 
    // (remover a moradia, mas manter o habitante)
    for(int i = 0; i < dir->tam_dir; i++){
        bool primeiro_visto = true; 
        // Flag para verificar se é a primeira vez que vemos esse bucket, 
        // para evitar despejar moradores do mesmo bucket mais de uma vez devido ao espelhamento no diretório
        
        // 2.1: Verifica se o bucket do índice atual já foi visto antes no diretório 
        // (devido ao espelhamento dos buckets no diretório, vários índices podem apontar para o mesmo bucket no disco)
        for(int j = 0; j < i; j++){
            if(dir->endr_disco[i] == dir->endr_disco[j]){
                primeiro_visto = false;
                break;
            }
        }

        // 2.2: Se for a primeira vez que vemos esse bucket, 
        // lemos os registros do bucket do disco e verificamos se são moradores da quadra especificada pelo CEP.
        if(primeiro_visto){
            Bucket b;
            bool balde_alterado = false; // Flag para otimizar a escrita no disco

            fseek(dir->arq_hf, dir->endr_disco[i], SEEK_SET);   // Posiciona o ponteiro do arquivo no início do bucket correspondente ao índice do diretório
            fread(&b, sizeof(Bucket), 1, dir->arq_hf);          // Lê os dados do bucket do arquivo físico da tabela hash para a estrutura de dados do bucket na memória RAM

            // 2.2.1: Itera por todos os registros do bucket para verificar se são moradores da quadra especificada pelo CEP e despejá-los
            for(int r = 0; r < b.qntd_regs; r++){
                Pessoas *p = &b.regs[r]; 

                // Se o registro for de um morador da quadra especificada pelo CEP, 
                // reporta os dados dele no arquivo de texto e depois limpa os dados de moradia (CEP, face, num, compl) do registro,
                if(strcmp(p->cep, cep) == 0){
                    // Reporta os dados do morador despejado no arquivo de texto
                    fprintf(txt, "Morador despejado: CPF: %s | Nome: %s %s\n", 
                            p->cpf, p->nome, p->sobrenome);

                    // Limpa os dados diretamente no balde
                    strcpy(p->cep, "");
                    strcpy(p->face, "");
                    strcpy(p->num, "");
                    strcpy(p->compl, "");

                    // Marca que o balde foi alterado para otimizar a escrita no disco
                    balde_alterado = true; 
                }
            }

            // 2.2.2: Se pelo menos um morador foi despejado neste balde, salvamos ele de volta
            if(balde_alterado){
                fseek(dir->arq_hf, dir->endr_disco[i], SEEK_SET);
                fwrite(&b, sizeof(Bucket), 1, dir->arq_hf);
            }
        }
    }
}

// Funções getter para acessar os campos de Pessoas
char* getPessoaNome     (Pessoas* p) { return p->nome;      }
char* getPessoaSobrenome(Pessoas* p) { return p->sobrenome; }
char* getPessoaSexo     (Pessoas* p) { return p->sexo;      }
char* getPessoaNasc     (Pessoas* p) { return p->nasc;      }
char* getPessoaCep      (Pessoas* p) { return p->cep;       }
char* getPessoaCpf      (Pessoas* p) { return p->cpf;       }
char* getPessoaFace     (Pessoas* p) { return p->face;      }
char* getPessoaNum      (Pessoas* p) { return p->num;       }
char* getPessoaCompl    (Pessoas* p) { return p->compl;     }

// Funções setter para modificar os campos de Pessoas
void setPessoaEndereco(Pessoas* p, char* cep, char* face, char* num, char* compl){
    // Atualiza os dados de moradia da pessoa apenas se os novos dados forem diferentes dos atuais, 
    // para evitar escritas desnecessárias no disco
    if(p->cep != cep)       strcpy(p->cep, cep);
    if(p->face != face)     strcpy(p->face, face);
    if(p->num != num)       strcpy(p->num, num);
    if(p->compl != compl)   strcpy(p->compl, compl);
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
    // 1: Aloca a estrutura de Pessoas na memória RAM
    Pessoas* p = (Pessoas*)malloc(sizeof(Pessoas));
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Pessoas\n");
        return NULL;
    }

    // 2: Inicializa os campos da estrutura Pessoas com valores padrão
    strcpy(p->cpf,  "");        // Inicializa o CPF como string vazia
    strcpy(p->nome, "");        // Inicializa o nome como string vazia
    strcpy(p->sobrenome, "");   // Inicializa a sobrenome como string vazia
    strcpy(p->sexo, "");        // Inicializa a sexo como string vazia
    strcpy(p->nasc, "");        // Inicializa a data de nascimento como string vazia

    return p;
}

void freeHashPM(hashPM* dir){
    // 1: Verifica se o ponteiro para a tabela hash é nulo antes de tentar liberar a memória
    if(dir == NULL) return;

    // 2: Fecha o arquivo físico associado à tabela hash
    if(dir->arq_hf != NULL) fclose(dir->arq_hf);

    // 3: Libera a memória alocada para o Diretório da RAM
    if(dir->endr_disco != NULL) free(dir->endr_disco);

    // 4: Libera a memória alocada para a estrutura da tabela hash
    free(dir);
}

void freePessoa(Pessoas* p){
    // 1: Verifica se o ponteiro para a pessoa é nulo antes de tentar liberar a memória
    if(p == NULL) return;

    // 2: Libera a memória alocada para a estrutura de Pessoas
    free(p);
}

int splitBucketPM(hashPM* dir, int indice_dir){
    // 1: Ler o bucket do disco para a memória
    long offset_bucket_antigo = dir->endr_disco[indice_dir];

    Bucket bucket_antigo;
    fseek(dir->arq_hf, offset_bucket_antigo, SEEK_SET);
    fread(&bucket_antigo, sizeof(Bucket), 1, dir->arq_hf);

    // 2: Duplicar o diretório (Se necessário) | Aumentar a Profundidade Global e o Tamanho do Diretório
    // printf("ESTOROU O BUCKET --> Indice do diretorio: %d | Profundidade local do bucket: %d | Profundidade global da tabela hash: %d\n", 
    //     indice_dir, bucket_antigo.prof_local, dir->prof_global);
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
    // printf("REDISTRIBUINDO OS REGISTROS...\n");
    if(redistribuirRegistrosPM(dir, indice_dir, &bucket_antigo, &bucket_novo, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao redistribuir os registros durante o slipBucket.\n");
        return -1;
    }
    // printf("REGISTROS REDISTRIBUIDOS COM SUCESSO!\n");

    // 5: Atualizar o diretório para apontar para os buckets correto (antigo e novo) de acordo com a nova profundidade local
    // printf("ATUALIZANDO O DIRETORIO...\n");
    if(atualizarDiretorioPM(dir, offset_bucket_antigo, offset_bucket_novo, &bucket_antigo, &bucket_novo, bit_divisor) != 0){
        fprintf(stderr, "ERRO: Falha ao atualizar o diretorio durante o slipBucket.\n");
        return -1;
    }
    // printf("DIRETORIO ATUALIZADO COM SUCESSO!\n\n");

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
        // printf("Pessoa %s salva no disco (Balde indice %d)\n", novaPessoa.cpf, indice_dir);
        
        return 0;        
    }
    // 8: Se não houver espaço, é necessário fazer o SPLIT do balde
    else{
        // 8.1: OVERFLOW - O balde estourou a capacidade de TAM_BUCKET
        // printf("\n");
        // printf("ALERTA: O Balde %d esta cheio! Iniciando SPLIT...\n", indice_dir);        
        splitBucketPM(dir, indice_dir);

        // 8.2: Após o split, a nova pessoa deve ser inserida novamente, pois o splitBucketPM apenas redistribui os registros existentes, mas não insere a nova pessoa que causou o split.
        return inserirRegPM(dir, cpf, nome, sobrenome, sexo, nasc);
    }
}

int salvarDiretorioHFC_PM(hashPM* dir, char* nomeArquivoHFC){
    // 1: Abre o arquivo de diretório para escrita em modo binário
    FILE* f = fopen("pessoas.hfc", "wb");
    if(f == NULL){
        printf("ERRO: Nao foi possivel criar o arquivo %s\n", nomeArquivoHFC);
        return -1;
    }

    // 2: Escreve a profundidade global, o tamanho do diretório e os endereços dos buckets no arquivo
    fwrite(&(dir->prof_global), sizeof(int), 1, f);
    fwrite(&(dir->tam_dir), sizeof(int), 1, f);
    fwrite(dir->endr_disco, sizeof(long), dir->tam_dir, f);

    // 3: Fecha o arquivo e retorna sucesso
    fclose(f);
    return 0;
}

hashPM* carregarDiretorioPM(char* nomeArquivoHFC, char* nomeArquivoHF){
    // 1: Abre o arquivo de diretório para leitura em modo binário
    FILE* f = fopen(nomeArquivoHFC, "rb");
    if(f == NULL){
        printf("ERRO: Nao foi possivel abrir o arquivo pessoas.hfc\n");
        return NULL;
    }

    // 2: Aloca a estrutura do Diretório na memória RAM
    hashPM* dir = malloc(sizeof(hashPM));
    if(dir == NULL){
        printf("ERRO: Nao foi possivel alocar memoria para o diretorio\n");
        fclose(f);
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
        free(dir);
        fclose(f);
        return NULL;
    }

    // 5: Lê os endereços dos buckets do arquivo para o vetor de endereços na RAM
    fread(dir->endr_disco, sizeof(long), dir->tam_dir, f);
    
    // 6: Abre o arquivo de dados (.hf) que já existe
    dir->arq_hf = fopen(nomeArquivoHF, "rb+"); 
    if(dir->arq_hf == NULL){
        printf("ERRO: Nao foi possivel abrir o arquivo %s\n", nomeArquivoHF);
        free(dir->endr_disco);
        free(dir);
        fclose(f);
        return NULL;
    }

    // 7: Fecha o arquivo de diretório e retorna o ponteiro para a estrutura do diretório carregada na RAM
    fclose(f);
    return dir;
}

int gerarRelatorioHFD(hashPM* dir, char* nomeArquivoHFD){
    // 1: Abre o arquivo de relatório para escrita em modo texto
    FILE* f = fopen("relatorio.hfd", "w"); 
    if(f == NULL){
        printf("ERRO ao criar arquivo de relatorio: relatorio.hfd\n");
        return -1;
    }

    // 2: Escreve as informações da tabela hash no arquivo de relatório
    fprintf(f, "--- RELATORIO DE DUMP DA TABELA HASH (PESSOAS) ---\n");
    fprintf(f, "Profundidade Global: %d\n", dir->prof_global);
    fprintf(f, "Tamanho do Diretorio: %d\n\n", dir->tam_dir);
    
    // 3: Mostrar o MAPA do Diretorio
    fprintf(f, "=== MAPA DO DIRETORIO (Indice -> Offset no Disco) ===\n");
    for(int i = 0; i < dir->tam_dir; i++){
        fprintf(f, "Indice [%03d]: Offset %ld\n", i, dir->endr_disco[i]);
    }
    
    // 4: Mostrar o CONTEUDO dos Baldes
    fprintf(f, "\n=== CONTEUDO DOS BUCKETS ===\n");
    
    // 5: Para evitar imprimir o mesmo balde várias vezes, só imprimimos o balde se ele for a primeira ocorrência daquele offset no diretório
    for(int i = 0; i < dir->tam_dir; i++){
        bool primeiro_visto = true;

        // 5.1: Verifica se 1 dos índices 0..i-1 do diretório já apontou para este mesmo offset de balde. Se sim, não imprime de novo
        for(int j = 0; j < i; j++){
            if (dir->endr_disco[i] == dir->endr_disco[j]){
                primeiro_visto = false;
                break;
            }
        }
        
        // 5.2: Se for a primeira vez que vemos este offset de balde, lemos o balde do disco e imprimimos seu conteúdo no relatório
        if(primeiro_visto){

            // 5.2.1: Lê o balde do disco usando o offset encontrado no diretório
            Bucket b;
            fseek(dir->arq_hf, dir->endr_disco[i], SEEK_SET);
            fread(&b, sizeof(Bucket), 1, dir->arq_hf);

            // 5.2.2 (APENAS PARA O RELATÓRIO): Imprime o número do bucket (índice do diretório), o offset do bucket no disco, a profundidade local do bucket e a quantidade de registros armazenados no bucket
            fprintf(f, "--------------------------------------------------\n");
            fprintf(f, "BUCKET no Offset: %ld | Prof. Local: %d | Qntd: %d/4\n", 
                dir->endr_disco[i], b.prof_local, b.qntd_regs);

            // 5.2.3: Imprime os registros do balde, mostrando o CPF, nome e sobrenome de cada pessoa armazenada no balde
            for(int r = 0; r < b.qntd_regs; r++){
                fprintf(f, "  Reg %d: CPF: %s | Nome: %s %s\n", 
                    r, b.regs[r].cpf, b.regs[r].nome, b.regs[r].sobrenome);
            }
        }
    }

    // 6: Fecha o arquivo de relatório e retorna sucesso
    fclose(f);
    printf("Relatorio textual .hfd gerado com sucesso: relatorio.hfd\n");

    return 0;
}
/*###############################################################################################*/