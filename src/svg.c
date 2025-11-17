#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"
#include "params.h"

/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoSvg(Param* param, char* caminhoSvg){
    char* dirSaida = getDirSaidaCompleto(param);
    char* geoSvg   = getNomeGeo           (param); // a.geo
    char* pathSvg;                             // a.svg

    // Monta pathSvg | a.svg
    size_t lenTxtCompleto = strlen(geoSvg) + 1; // +1 para o '\0'
    pathSvg = (char*)malloc(lenTxtCompleto * sizeof(char));
    if(pathSvg == NULL){
        fprintf(stderr, "Erro: Falha na alocação de memória para o nome do arquivo svg completo.\n");
        return -1;
    }

    // Remove a extensão .geo e adiciona .svg
    strcpy(pathSvg, geoSvg);                                  // Copia o nome do arquivo .geo
    pathSvg[strlen(geoSvg) - 3] = '\0';                       // Remove a extensão .geo
    strcat(pathSvg, "svg");                                   // Adiciona a extensão .svg
    strcat(dirSaida, pathSvg);                                // Concatena o diretório de saída completo com o nome do arquivo .svg
    strcat(dirSaida, "\0");                                   // Adiciona o caractere nulo ao final
    strcpy(caminhoSvg, dirSaida);                             // Copia o caminho completo do arquivo
    printf("Nome do arquivo svg completo: %s\n", caminhoSvg); // Caminho completo do arquivo .svg (Sem o qry)
    
    return 0;
}
int montarCaminhoSvgQry(Param* param, char* caminhoSvgQry){
    char* dirSaida = getDirSaidaCompleto(param);
    char* geoSvg   = getNomeGeo           (param); // a.geo
    char* qrySvg   = getNomeQry           (param); // b.qry
    char* pathSvgQry;                              // a-b.svg

    // Monta pathSvgQry | a-b.svg
    size_t lenPathSvgQry = (strlen(geoSvg) - 3) + strlen(qrySvg) + 2; // +1 para o '-' e +1 para o '\0'
    pathSvgQry = (char*)malloc(lenPathSvgQry * sizeof(char));
    if(pathSvgQry == NULL){
        fprintf(stderr, "Erro: Falha na alocação de memória para o nome do arquivo svg completo.\n");
        return -1;
    }

    strcpy(pathSvgQry, geoSvg);                // Copia o nome do arquivo .geo
    pathSvgQry[strlen(geoSvg) - 4] = '\0';     // Remove a extensão .geo (Inclusive o .)
    strcat(pathSvgQry, "-");                   // Adiciona o '-'
    strcat(pathSvgQry, qrySvg);                // Concatena o nome do arquivo .qry
    pathSvgQry[strlen(pathSvgQry) - 3] = '\0'; // Remove a extensão qry (mantém o .)
    strcat(pathSvgQry, "svg");                 // Adiciona a extensão .svg
    strcat(dirSaida, pathSvgQry);              // Concatena o diretório de saída completo com o nome do arquivo .svg
    strcpy(caminhoSvgQry, dirSaida);           // Copia o caminho completo do arquivo .svg (Com o qry)
    strcat(pathSvgQry, "\0");                  // Adiciona o caractere nulo ao final
    printf("Nome do arquivo svg completo: %s\n", pathSvgQry);
    
    return 0;
}
int montarCaminhoTxt(Param* param, char* caminhoTxt){
    char* dirSaida = getDirSaidaCompleto(param);
    char* geoSvg   = getNomeGeo           (param); // a.geo
    char* qrySvg   = getNomeQry           (param); // b.qry
    char* txtCompleto;                             // a-b.txt

    // Monta txtCompleto | a-b.txt
    size_t lenTxtCompleto = (strlen(geoSvg) - 3) + strlen(qrySvg) + 2; // +1 para o '-' e +1 para o '\0'
    txtCompleto = (char*)malloc(lenTxtCompleto * sizeof(char));
    if(txtCompleto == NULL){
        fprintf(stderr, "Erro: Falha na alocação de memória para o nome do arquivo txt completo.\n");
        return -1;
    }

    strcpy(txtCompleto, geoSvg);                 // Copia o nome do arquivo .geo
    txtCompleto[strlen(geoSvg) - 4] = '\0';      // Remove a extensão .geo (Inclusive o .)
    strcat(txtCompleto, "-");                    // Adiciona o '-'
    strcat(txtCompleto, qrySvg);                 // Concatena o nome do arquivo .qry
    txtCompleto[strlen(txtCompleto) - 3] = '\0'; // Remove a extensão qry (mantém o .)
    strcat(txtCompleto, "txt");                  // Adiciona a extensão .txt
    strcat(dirSaida, txtCompleto);               // Concatena o diretório de saída completo com o nome do arquivo .txt
    strcpy(caminhoTxt, dirSaida);                // Copia o caminho completo do arquivo .txt
    strcat(txtCompleto, "\0");                   // Adiciona o caractere nulo ao final
    printf("Nome do arquivo txt completo: %s\n", txtCompleto);

    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarSvg(Param* param){
    char caminhoSvg[512];    // Inicializa o buffer para o caminho completo do arquivo .svg
    char caminhoSvgQry[512]; // Inicializa o buffer para o caminho completo do arquivo .svg (Com o qry)
    char caminhoTxt[512];    // Inicializa o buffer para o caminho completo do arquivo .txt

    // Monta o caminho completo do arquivo .svg (Sem o qry)
    if(montarCaminhoSvg(param, caminhoSvg) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .svg (Sem o qry)\n");
        return -1;
    }

    // Monta o caminho completo do arquivo .svg (Com o qry) (Verificar se é nulo)
    if(getNomeQry(param) != NULL){
        if(montarCaminhoSvgQry(param, caminhoSvgQry) != 0){
            fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .svg (Com o qry)\n");
            return -1;
        }
    }else{
        printf("Arquivo .qry nao fornecido. Pulando a montagem do caminho completo do arquivo .svg com o qry.\n");
    }
    
    // Monta o caminho completo do arquivo .txt (Verificar se o QRY é nulo)
    if(getNomeQry(param) != NULL){
        if(montarCaminhoTxt(param, caminhoTxt) != 0){
            fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .txt\n");
            return -1;
        }
    }else{
        printf("Arquivo .qry nao fornecido. Pulando a montagem do caminho completo do arquivo .txt.\n");
    }

    return 0;
}
/*###############################################################################################*/
