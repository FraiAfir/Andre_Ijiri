#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pm.h"
#include "params.h"

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct pm{
    // 1: Comando do arquivo .pm
    char* comando;
    // p - Insere um habitante
    // m - Informa que um dado habitante (cpf) mora num dado endereço (cep,face,num,compl)

    // 2: Parâmetros associados aos comandos
    char* cpf;
    // 2.1: Parâmetros do comando p (Insere um habitante)
    char* nome;
    char* sobrenome;
    char  sexo;
    char* nasc;

    // 2.2: Parâmetros do comando m (Informa que um dado habitante mora num dado endereço)
    char* cep;
    char  face;
    int   num;
    char* compl;
}PM;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoPM(Param* param, char* caminhoPM){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomePM    = getNomePM           (param);

    // Imprime o nome do arquivo .pm original para depuração
    printf("Arquivo .pm fornecido: \t\t\t%s\n", nomePM);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .pm
    strcpy(caminhoPM, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoPM, nomePM);     // Concatena o nome do arquivo .pm ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .pm: %s\n", caminhoPM);

    return 0;
}
int readFilePM(FILE* arquivoPM){
    char linha[256];

    // Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoPM) != NULL){
        // 1: Inicializa variáveis temporárias para armazenar os dados lidos de cada linha do arquivo .pm
        char comando[2], cpf[15];
        char nome[256], sobrenome[256], sexo, nasc[11];
        char cep[7], face, compl[256];

        // 2: Remove o ENTER do final da linha, se existir. Ignora linhas em branco
        linha[strcspn(linha, "\n")] = '\0';
        if(strlen(linha) == 0) continue;

        // 3: Processa o comando lido do arquivo .pm
        sscanf(linha, "%s %s %s %s %c %s", 
            comando, cpf, nome, sobrenome, &sexo, nasc);

        // 4: Cria uma instância de PM para armazenar os dados de uma das linha do arquivo .pm
        PM* pm = criarPM();
        if(pm == NULL){
            fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto PM\n");
            return -1;
        }

        // 5: Armazena os dados lidos do arquivo .pm na instância de PM criada
        pm->comando   = strdup(comando);
        pm->cpf       = strdup(cpf);
        pm->nome      = strdup(nome);
        pm->sobrenome = strdup(sobrenome);
        pm->sexo      = sexo;
        pm->nasc      = strdup(nasc);

    }

    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarPM(Param* param){
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
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo .pm: %s\n", caminhoPM);
        return -1;
    }

    // 3: Lê e processa os dados do arquivo .pm
    if(readFilePM(arquivoPM) != 0){  
        fprintf(stderr, "ERRO: Leitura do arquivo .pm.\n");
        fclose(arquivoPM);
        return -1;
    }

    // 4: Fecha o arquivo .pm após o processamento
    fclose(arquivoPM);
    printf("Arquivo .pm processado com sucesso!\n");
    return 0;
}
PM* criarPM(){
    PM* pm = (PM*)malloc(sizeof(PM));
    if(pm == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto PM\n");
        return NULL;
    }

    pm->comando = NULL;
    pm->cpf     = NULL;

    pm->nome      = NULL;
    pm->sobrenome = NULL;
    pm->sexo      = '\0';
    pm->nasc      = NULL;

    pm->cep     = NULL;
    pm->face    = '\0';
    pm->num     = 0;
    pm->compl   = NULL;

    return pm;
}
int freePM(PM* pm){
    printf("\nLiberando a memoria alocada para a instancia de PM...\n");

    if(pm == NULL){
        fprintf(stderr, "ERRO: Ponteiro para PM NULL\n");
        return -1;
    }

    free(pm->comando);

    free(pm->cpf);
    free(pm->nome);
    free(pm->sobrenome);
    free(pm->nasc);

    free(pm->cep);
    free(pm->compl);

    free(pm);

    printf("\nMemoria alocada para a instancia de PM liberada com sucesso!\n");
    return 0;
}
/*###############################################################################################*/
