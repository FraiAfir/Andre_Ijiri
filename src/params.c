#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"

/*                            ESTRUTURA DE DADOS A SER IMPLEMENTADA                            */
// Estrutura para armazenar os parâmetros/argumentos da linha de comando
typedef struct parametro{
    char* dirEntrada;
    char* nomeGeo;
    char* nomeQry;
    char* dirSaida;

    char* dirEntradaCompleto;
    char* dirSaidaCompleto;
}Param;
/*#############################################################################################*/

/*                                      FUNÇÕES AUXILIARES                                     */
// Função auxiliar para tratar os caminhos completos dos arquivos e diretórios de entrada e saída
int tratarCaminhosCompletos(Param* param){
    // TRATA OS PARÂMETROS/ARGUMENTOS LIDOS DA LINHA DE COMANDO E MONTA OS CAMINHOS COMPLETOS DOS ARQUIVOS E DIRETÓRIOS DE ENTRADA E SAÍDA
    // OBS: Os parâmetros -f e -o são obrigatórios, enquanto os parâmetros -e e -q são opcionais
    // Se o parâmetro -e não for fornecido, assume o diretório atual "./"
    // Diretório de entrada
    if(param->dirEntrada != NULL){
        size_t lenDirEntrada = strlen(param->dirEntrada);

        if(param->dirEntrada[lenDirEntrada - 1] != '/'){
            // Adiciona '/' ao final do diretório de entrada, se não existir
            param->dirEntradaCompleto = (char*) malloc((lenDirEntrada + 2) * sizeof(char));
           
            if(param->dirEntradaCompleto == NULL){
                fprintf(stderr, "Erro: Falha na alocação de memória para o diretório de entrada completo->\n");
                return -1;
            }

            strcpy(param->dirEntradaCompleto, param->dirEntrada);
            strcat(param->dirEntradaCompleto, "/");

        }else param->dirEntradaCompleto = strdup(param->dirEntrada);
    }else{
        // Se o diretório de entrada não for fornecido, assume o diretório atual "->/"
        param->dirEntradaCompleto = strdup("./");
    }

    // Diretório de saída
    if(param->dirSaida != NULL){
        size_t lenDirSaida = strlen(param->dirSaida);

        if(param->dirSaida[lenDirSaida - 1] != '/'){
            // Adiciona '/' ao final do diretório de saída, se não existir
            param->dirSaidaCompleto = (char*) malloc((lenDirSaida + 2) * sizeof(char));
            
            if(param->dirSaidaCompleto == NULL){
                fprintf(stderr, "Erro: Falha na alocação de memória para o diretório de saída completo.\n");
                return -1;
            }

            strcpy(param->dirSaidaCompleto, param->dirSaida);
            strcat(param->dirSaidaCompleto, "/");

        }else param->dirSaidaCompleto = strdup(param->dirSaida);
    }else{
        // Se o diretório de saída não for fornecido, assume o diretório atual "->/"
        param->dirSaidaCompleto = strdup("./");
    }
}
// Função auxiliar para processar os argumentos da linha de comando
int processarArgumentosInternos(Param* param, int argc, char* argv[]){
    // LÊ OS PARÂMETROS/ARGUMENTOS DA LINHA DE COMANDOS
    // Parâmetros possíveis: -f (obrigatório), -o (obrigatório), -e (opcional), -q (opcional)
    // A ordem dos parâmetros pode variar
    // Exemplo de chamada: programa -f arquivo.geo -o dirSaida -e dirEntrada -q arquivo.qry
    int i = 1;
    while(i < argc){
        if(strcmp(argv[i], "-f") == 0){
            // Parâmetro -f (nome do arquivo .geo)
            if(argv[1 + 1] != NULL){
                param->nomeGeo = strdup(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "Erro: Nome do arquivo .geo não fornecido. (-f obrigatório)\n");
                return -1;
            }
        }
        if(strcmp(argv[i], "-o") == 0){
            // Parâmetro -o (diretório de saída)
            if(argv[i + 1] != NULL){
                param->dirSaida = strdup(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "Erro: Diretório de saída não fornecido. (-o obrigatório)\n");
                return -1;
            }
        }
        if(strcmp(argv[i], "-e") == 0){
            // Parâmetro -e (diretório de entrada)
            if(argv[1 + 1] != NULL){
                param->dirEntrada = strdup(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "Erro: Diretório de entrada não fornecido. (-e opcional)\n");
                return -1;
            }
        }
        if(strcmp(argv[i], "-q") == 0){
            // Parâmetro -q (nome do arquivo .qry)
            if(argv[i + 1] != NULL){
                param->nomeQry = strdup(argv[i + 1]);
                i++;
            } else {
                fprintf(stderr, "Erro: Nome do arquivo .qry não fornecido. (-q opcional)\n");
                return -1;
            }
        }
    }
}
/*#############################################################################################*/



/*                                      FUNÇÕES PRINCIPAIS                                     */
// Função para criar um novo objeto de Parametro
Param* criarParametro(){
    Param* param = (Param*) malloc(sizeof(Param));
    if(param == NULL){
        fprintf(stderr, "Erro: Falha na alocação de memória para o objeto Parametro.\n");
        return NULL;
    }

    param->dirEntrada = NULL;
    param->nomeGeo    = NULL;
    param->nomeQry    = NULL;
    param->dirSaida   = NULL;

    param->dirEntradaCompleto = NULL;
    param->dirSaidaCompleto   = NULL;

    return param;
}
// Função para processar os parâmetros/argumentos passados na linha de comando
int processarParametros(Param* param, int argc, char* argv[]){

    // Mínimo de argumentos da linha de comando: 5
    // Exemplo: -f arquivo.geo -o dirSaida (-f e -o são obrigatórios)
    if(argc < 5){
        fprintf(stderr, "Erro: Número insuficiente de argumentos.\n");
        return -1;
    }

    int res;
    res = processarArgumentosInternos(param, argc, argv);
    if(res == -1){
        printf("Erro ao processar os argumentos da linha de comando.\n");
        return -1;
    }
    res = tratarCaminhosCompletos(param);
    if(res == -1){
        printf("Erro ao tratar os caminhos completos dos arquivos e diretórios.\n");
        return -1;
    }

    return 0;
}
// Função para liberar a memória alocada para o objeto de Parametro
void freeParametros(Param* param){
    printf("Liberando parametros...\n");

    if(param == NULL){
        printf("\n\n#---------- Erro: Ponteiro para Parametro é NULL ----------#\n\n");
        return;
    }

    if(param->dirEntrada != NULL) free(param->dirEntrada);
    if(param->nomeGeo    != NULL) free(param->nomeGeo);
    if(param->nomeQry    != NULL) free(param->nomeQry);
    if(param->dirSaida   != NULL) free(param->dirSaida);

    if(param->dirEntradaCompleto != NULL) free(param->dirEntradaCompleto);
    if(param->dirSaidaCompleto   != NULL) free(param->dirSaidaCompleto);

    free(param);
    param = NULL;
    printf("Parametros liberados com sucesso!\n");
}
/*#############################################################################################*/
