#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"
#include "params.h"
// incluir a estrutura de dados a ser implementada para armazenar os dados do arquivo .geo

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
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

    // Imprime o nome do arquivo .geo original para depuração
    printf("Arquivo .geo fornecido: \t\t\t%s\n", nomeGeo);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .geo
    strcpy(caminhoGeo, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoGeo, nomeGeo);    // Concatena o nome do arquivo .geo ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .geo: \t\t%s\n", caminhoGeo);

    return 0;
}
int readFileGeo(FILE* arquivoGeo){
    char linha[256];

    // Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoGeo) != NULL){
        // Inicializa variáveis temporárias para armazenar os dados lidos de cada linha do arquivo .geo
        char comando[5], cep[5];
        double x, y, w, h, sw;
        char cfill[256], cstrk[256];

        
        // 1: Remove o ENTER do final da linha, se existir. Ignora linhas em branco
        linha[strcspn(linha, "\n")] = '\0';
        if(strlen(linha) == 0) continue;

        // Imprime a linha lida do arquivo .geo para depuração
        printf("Lendo linha do .geo: %s", linha);
        
        // 2: Processa o comando lido do arquivo .geo
        sscanf(linha, "%s %s %lf %lf %lf %lf %lf %s %s", 
            comando, cep, &x, &y, &w, &h, &sw, cfill, cstrk);

        // Cria uma instância de Geo para armazenar os dados de uma das linha do arquivo .geo
        Geo* geo = (Geo*)malloc(sizeof(Geo));
        if(geo == NULL){
            fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Geo\n");
            return -1;
        }fprintf(stdout, "Instancia de Geo criada com sucesso para armazenar os dados da linha\n");

        // Armazena os dados lidos do arquivo .geo na instância de Geo
        geo->comando = strdup(comando);
        geo->cep     = strdup(cep);
        geo->x       = x;
        geo->y       = y;
        geo->w       = w;
        geo->h       = h;
        geo->sw      = sw;
        geo->cfill   = strdup(cfill);
        geo->cstrk   = strdup(cstrk);

        // Imprime os dados lidos do arquivo .geo para depuração
        printf("Linha lida: %s %s %lf %lf %lf %lf %lf %s %s\n", 
            geo->comando, geo->cep, geo->x, geo->y, geo->w, geo->h, geo->sw, geo->cfill, geo->cstrk);

        // Adiciona a linha lida à estrutura de dados apropriada
        // *função para inserir os dados na estrutura de dados a ser implementada*
    }

    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarGeo(Param* param){
    // Inicializa o buffer para o caminho completo do arquivo .geo
    char caminhoGeo[512];   

    // Monta o caminho completo do arquivo .geo
    if(montarCaminhoGeo(param, caminhoGeo) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .geo.\n");
        return -1;
    }

    printf("Iniciando o processamento do arquivo .geo\n");

    // Abre o arquivo .geo para leitura
    FILE* arquivoGeo = fopen(caminhoGeo, "r");
    if(arquivoGeo == NULL){
        fprintf(stderr, "ERRO: Nao foi possivel abrir o arquivo .geo: %s\n", caminhoGeo);
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