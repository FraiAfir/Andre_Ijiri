#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "txt.h"
#include "params.h"
/*                                       FUNÇÕES AUXILIARES                                      */
char* montarCaminhoTxt   (Param* param, char* caminhoTxt){
    char* dirSaida    = getDirSaidaCompleto(param); // Diretório de saída completo (Ex: ./saida/)
    char* geoSvg      = getNomeGeo         (param); // a.geo
    char* qrySvg      = getNomeQry         (param); // b.qry

    // 1: Copia o diretório de saída completo para o caminho do arquivo .txt
    strcpy(caminhoTxt, dirSaida);

    // 2: Busca o nome base dos arquivos .geo e .qry (Sem as extensões)
    char nomeBaseGeo[256];                          // Buffer para armazenar o nome base do arquivo .geo (Sem a extensão .geo)
    char nomeBaseQry[256];                          // Buffer para armazenar o nome base do arquivo .qry (Sem a extensão .qry)
    strcpy(nomeBaseGeo, geoSvg);                    // Copia o nome do arquivo .geo para o buffer nomeBaseGeo
    strcpy(nomeBaseQry, qrySvg);                    // Copia o nome do arquivo .qry para o buffer nomeBaseQry
    char* extensaoGeo = strrchr(nomeBaseGeo, '.');  // Busca a última ocorrência do caractere '.' para encontrar a extensão do arquivo .geo
    char* extensaoQry = strrchr(nomeBaseQry, '.');  // Busca a última ocorrência do caractere '.' para encontrar a extensão do arquivo .qry
    if(extensaoGeo != NULL)                        // Verifica se a extensão do arquivo .geo foi encontrada
        *extensaoGeo = '\0';                        // Remove a extensão .geo

    if(extensaoQry != NULL)                        // Verifica se a extensão do arquivo .qry foi encontrada
        *extensaoQry = '\0';                        // Remove a extensão .qry

    // 3: Concatena os nomes base dos arquivos .geo e .qry com um '-' entre eles
    // Adiciona a extensão .txt e o diretório de saída para formar o caminho completo do arquivo .txt
    strcat(caminhoTxt, nomeBaseGeo); // Concatena o diretório de saída com o nome base do arquivo .geo
    strcat(caminhoTxt, "-");         // Adiciona um '-' entre os nomes base dos arquivos .geo e .qry
    strcat(caminhoTxt, nomeBaseQry); // Concatena o nome base do arquivo .qry
    strcat(caminhoTxt, ".txt");      // Adiciona a extensão .txt

    // Exibe o caminho completo do arquivo .txt gerado (Com o qry)
    printf("Nome do arquivo txt (.geo-.qry) completo: \t%s\n", caminhoTxt);

    return caminhoTxt;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
FILE* criarTxt(char* pathTxt){
    FILE* arqTxt = fopen(pathTxt, "w");
        if(arqTxt == NULL){
            fprintf(stderr, "ERRO: Nao foi possivel criar o arquivo .txt em %s\n", pathTxt);
            return NULL;
        } return arqTxt;
}   

int escreverComandoTxt(FILE* arqTxt, char* txt){
    if(fprintf(arqTxt, "%s\n", txt) < 0){
        fprintf(stderr, "ERRO: Nao foi possivel escrever no arquivo .txt\n");
        return -1;
    } return 0;
}

int fecharTxt(FILE* arqTxt){
    if(fclose(arqTxt) != 0){
        fprintf(stderr, "ERRO: Nao foi possivel fechar o arquivo .txt\n");
        return -1;
    } return 0;
}
/*###############################################################################################*/