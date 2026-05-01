#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pm.h"
#include "params.h"
#include "hashPM.h"

/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoPM(Param* param, char* caminhoPM){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomePM     = getNomePM(param);

    // Imprime o nome do arquivo .pm original para depuração
    printf("Arquivo .pm fornecido: \t\t\t%s\n", nomePM);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .pm
    strcpy(caminhoPM, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoPM, nomePM);     // Concatena o nome do arquivo .pm ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .pm: \t%s\n", caminhoPM);

    return 0;
}

int readFilePM(FILE* arquivoPM, hashPM* dir, Pessoas* p){
    // Buffers para armazenar os dados lidos de cada linha do arquivo .pm
    char linha[256];
    char comando[5];

    // Variáveis temporárias para armazenar os dados lidos de cada linha do arquivo .pm
    char cpf[20];
    char nome[50];
    char sobrenome[50];
    char sexo[5];       // String para evitar problemas de leitura com sscanf
    char nasc[20];
    char cep[5];
    char face[5];       // String para evitar problemas de leitura com sscanf
    char num[10];
    char compl[50];
    
    // 1: Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoPM) != NULL){

        // 1.1: Lê o comando da linha para determinar o tipo de dado a ser processado
        sscanf(linha, "%s", comando);

        // 1.2: Processa o comando lido do arquivo .pm
        if(strcmp(comando, "p") == 0){
            // Comando p - Insere um habitante
            // '*' é usado para ignorar a primeira string (p) e ler os próximos valores
            // Exemplo de linha: comando    cpf      nome  sobrenome  sexo     nasc
            // Exemplo de linha:   p    12345678901  João  Silva       M    1990-01-01
            sscanf(linha, "%*s %s %s %s %s %s", cpf, nome, sobrenome, sexo, nasc);

            // 1.2.1: Insere a pessoa na estrutura de dados (Tabela Hash e Pessoas)
            inserirRegPM(dir, cpf, nome, sobrenome, sexo, nasc);
        }else if(strcmp(comando, "m") == 0){
            // Comando m - Informa que um dado habitante (cpf) mora num dado endereço (cep,face,num,compl)
            // '*' é usado para ignorar a primeira string (m) e ler os próximos valores
            // Exemplo de linha: comando cpf  cep  face  num  compl
            // Exemplo de linha:   m    12345678901  Q1   10    20    Apt 101
            sscanf(linha, "%*s %s %s %s %s %s", cpf, cep, face, num, compl);

            // 1.2.2: Adiciona a moradia à pessoa
            adicionarMoradia(dir, cpf, cep, face, num, compl);
        }
    }

    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarPM(Param* param, hashPM* h, Pessoas* p){
    // Inicializa o buffer para o caminho completo do arquivo .pm
    char caminhoPM[512];   

    // 1: Monta o caminho completo do arquivo .pm
    if(montarCaminhoPM(param, caminhoPM) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .pm.\n");
        return -1;
    }

    printf("Iniciando o processamento do arquivo .pm\n\n");

    // 2: Abre o arquivo .pm para leitura
    FILE* arquivoPM = fopen(caminhoPM, "r");
    if(arquivoPM == NULL){
        fprintf(stderr, "ERRO: Nao foi possivel abrir o arquivo .pm: %s\n", caminhoPM);
        return -1;
    }

    // 3: Verifica se a estrutura de dados para armazenar os dados do arquivo .pm foi criada com sucesso
    if(p == NULL){
        fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .pm.\n");
        fclose(arquivoPM);
        return -1;
    }

    // 4: Lê e processa os dados do arquivo .pm
    if(readFilePM(arquivoPM, h, p) != 0){
        fprintf(stderr, "ERRO: Leitura do arquivo .pm.\n");
        fclose(arquivoPM);
        return -1;
    }

    // 5: Fecha o arquivo .pm após o processamento
    fclose(arquivoPM);
    printf("\nArquivo .pm processado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/