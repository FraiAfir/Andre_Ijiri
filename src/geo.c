#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"
#include "params.h"
// incluir a estrutura de dados a ser implementada para armazenar os dados do arquivo .geo

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
// Estrutura para armazenar os dados do arquivo .geo
typedef struct geo{
    char* comando; // Comando do arquivo .geo
    // q  - insere uma quadra (retângulo e CEP)
    // cq - cores da quadra (Cores do preenchimento (cfill) e da borda (cstrk) das quadras
    // espessura da borda (sw)

    char*  cep;    // Identificador único de cada quadra
    double x, y;   // Coordenadas das quadras
    double w, h;   // Largura e altura das quadras

    double sw;     // Espessura da borda das quadras
    char*  cfill;  // Cor de preenchimento das quadras
    char*  cstrk;  // Cor da borda das quadras
}Geo;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoGeo(Param* param, char* caminhoGeo){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeGeo    = getNomeGeo           (param);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .geo
    strcpy(caminhoGeo, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoGeo, nomeGeo);    // Concatena o nome do arquivo .geo ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .geo: %s\n", caminhoGeo);

    return 0;
}
int readFileGeo(FILE* arquivoGeo){
    char linha[256];

    // Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoGeo) != NULL){
        printf("Lendo linha do .geo: %s", linha);

        // Adiciona a linha lida à estrutura de dados apropriada
        // *função para inserir os dados na estrutura de dados a ser implementada*
    }
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarGeo(Param* param){
    char caminhoGeo[512];   // Inicializa o buffer para o caminho completo do arquivo .geo

    // Monta o caminho completo do arquivo .geo
    if(montarCaminhoGeo(param, caminhoGeo) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .geo.\n");
        return -1;
    }

    // Abre o arquivo .geo para leitura
    FILE* arquivoGeo = fopen(caminhoGeo, "r");
    if(arquivoGeo == NULL){
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo .geo: %s\n", caminhoGeo);
        return -1;
    }

    // Lê e processa os dados do arquivo .geo
    if(readFileGeo(arquivoGeo) != 0){  
        fprintf(stderr, "ERRO: Leitura do arquivo .geo.\n");
        fclose(arquivoGeo);
        return -1;
    }

    // Fecha o arquivo .geo após o processamento
    fclose(arquivoGeo);
    printf("Arquivo .geo processado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/
