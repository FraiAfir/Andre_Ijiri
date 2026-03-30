#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pm.h"
#include "params.h"
// incluir a estrutura de dados a ser implementada para armazenar os dados do arquivo .via

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct pm{
    // 1: Comando do arquivo .pm
    char* comando;
    // p - Insere um habitante
    // m - Informa que um dado habitante (cpf) mora num dado endereço (cep,face,num,compl)

    // 2: Parâmetros associados aos comandos
    // 2.1: Parâmetros do comando p (Insere um habitante)
    char* cpf;
    char* nome;
    char* sobrenome;
    char  sexo;
    char* sexo;

    // 2.2: Parâmetros do comando m (Informa que um dado habitante mora num dado endereço)
    char* cpf;
    char* cep;
    char  face;
    int   num;
    char* compl;
}PM;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoVia(Param* param, char* caminhoVia){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeVia    = getNomeVia           (param);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .via
    strcpy(caminhoVia, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoVia, nomeVia);    // Concatena o nome do arquivo .via ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .via: %s\n", caminhoVia);

    return 0;
}
int readFileVia(FILE* arquivoVia){
    char linha[256];

    // Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoVia) != NULL){
        printf("Lendo linha do .via: %s", linha);

        // Adiciona a linha lida à estrutura de dados apropriada
        // *função para inserir os dados na estrutura de dados a ser implementada*
    }

    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarVia(Param* param){
    char caminhoVia[512];   // Inicializa o buffer para o caminho completo do arquivo .via

    // Monta o caminho completo do arquivo .via
    if(montarCaminhoVia(param, caminhoVia) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .via.\n");
        return -1;
    }

    // Abre o arquivo .via para leitura
    FILE* arquivoVia = fopen(caminhoVia, "r");
    if(arquivoVia == NULL){
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo .via: %s\n", caminhoVia);
        return -1;
    }

    // Lê e processa os dados do arquivo .via
    if(readFileVia(arquivoVia) != 0){  
        fprintf(stderr, "ERRO: Leitura do arquivo .via.\n");
        fclose(arquivoVia);
        return -1;
    }

    // Fecha o arquivo .via após o processamento
    fclose(arquivoVia);
    printf("Arquivo .via processado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/
