#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "params.h"
// incluir a estrutura de dados a ser implementada para armazenar os dados do arquivo .qry

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
// Estrutura para armazenar os dados do arquivo .qry
typedef struct qry{
    char*  cep;   // Identificador único de cada quadra
    double x, y;  // Coordenadas das quadras
    double w, h;  // Largura e altura das quadras

    double sw;    // Espessura da borda das quadras
    char*  cfill; // Cor de preenchimento das quadras
    char*  cstrk; // Cor da borda das quadras
}Geo;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoQry(Param* param, char* caminhoQry){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeQry    = getNomeQry           (param);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .qry
    strcpy(caminhoQry, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoQry, nomeQry);    // Concatena o nome do arquivo .qry ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .qry: %s\n", caminhoQry);

    return 0;
}
int readFileQry(FILE* arquivoQry){
    char linha[256];

    // Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoQry) != NULL){
        // Processa cada linha do arquivo .qry
        printf("Lendo linha do .qry: %s", linha);
        // Adiciona a linha lida à estrutura de dados apropriada
        // *função para inserir os dados na estrutura de dados a ser implementada*
    }
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarQry(Param* param){
    char caminhoQry[512];   // Inicializa o buffer para o caminho completo do arquivo .qry

    // Monta o caminho completo do arquivo .qry
    if(montarCaminhoQry(param, caminhoQry) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .qry.\n");
        return -1;
    }

    // Abre o arquivo .qry para leitura
    FILE* arquivoQry = fopen(caminhoQry, "r");
    if(arquivoQry == NULL){
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo .qry: %s\n", caminhoQry);
        return -1;
    }

    // Lê e processa os dados do arquivo .qry
    if(readFileQry(arquivoQry) != 0){  
        fprintf(stderr, "ERRO: Leitura do arquivo .qry.\n");
        fclose(arquivoQry);
        return -1;
    }

    // Fecha o arquivo .qry após o processamento
    fclose(arquivoQry);
    printf("Arquivo .qry processado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/
