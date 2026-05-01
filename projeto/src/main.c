#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pm.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "params.h"
#include "program.h"
#include "hashTable.h"
#include "hashPM.h"

int main(int argc, char* argv[]){
    system("cls");

    printf("##################### INICIO DA EXECUCAO DO PROGRAMA #####################\n\n");

    printf("Argumentos da linha de comando:\n");
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1: Cria os objetos das estruturas necssárias para a execução do código
    printf("\n\n\n\n\n");
    Param*      param  = NULL; // Declara o objeto de parâmetros

    TabelaHash* htq    = NULL; // Declara o diretório da tabela hash para armazenar os dados do arquivo .geo
    Quadras*    q      = NULL; // Declara o objeto para armazenar os dados lidos do arquivo .geo

    hashPM* htp        = NULL; // Declara o diretório da tabela hash para armazenar os dados do arquivo .pm
    Pessoas*    p      = NULL; // Declara o objeto para armazenar os dados lidos do arquivo .pm

    // 1.1: Inicializa os objetos de Parametro e da ED para armazenar os dados do arquivo .geo
    if(bootProgram(&param, &htq, &q, &htp, &p) == -1){
        printf("ERRO: Inicializacao dos objetos.\n");
        return -1;
    }

    // 2. PROCESSAR PARÂMETROS DA LINHA DE COMANDOS
    printf("\n\n\n\n\n#------------ PROCESSANDO OS PARAMETROS DA LINHA DE COMANDO... ----------#\n");
    if(processarParametros(param, argc, argv) == -1) {
        printf("ERRO: Processamento dos parametros da linha de comando.\n");
        shutProgram(&param, &htq, &q, &htp, &p);
        return -1;
    }
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");

    // 3. PROCESSAR O GEO
    // Processa o arquivo .geo e armazena os dados em uma estrutura de dados adequada (Lista, Árvore, etc.)
    // Verificar se o objeto foi criado com sucesso
    printf("\n#-------------------- PROCESSANDO O ARQUIVO .GEO... ---------------------#\n");
    if(processarGeo(param, htq, q) != 0){
        printf("ERRO: Processamento do arquivo .geo.\n");
        shutProgram(&param, &htq, &q, &htp, &p);
        return -1;
    }
    if(salvarDiretorioHFC(htq, getNomeGeo(param)) != 0){
        printf("ERRO: Salvamento do diretório da tabela hash do arquivo .geo.\n");
        shutProgram(&param, &htq, &q, &htp, &p);
        return -1;
    }
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    
    // 4. PROCESSAR O PM (Se fornecido)
    // Processa o arquivo .pm e realiza as operações de inserção e modificação nos dados armazenados a partir do arquivo .geo, conforme as instruções do arquivo .pm
    // Verificar se o objeto foi criado com sucesso
    if(getNomePM(param) == NULL){
        printf("\n#----------- ARQUIVO .PM NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    }else{
        printf("\n#--------------------- PROCESSANDO O ARQUIVO .PM... --------------------#\n");
        if(processarPM(param, htp, p) != 0){
            printf("ERRO: Processamento do arquivo .pm.\n");
            shutProgram(&param, &htq, &q, &htp, &p);
            return -1;
        }
        if(salvarDiretorioHFC_PM(htp, getNomePM(param)) != 0){
            printf("ERRO: Salvamento do diretório da tabela hash do arquivo .pm.\n");
            shutProgram(&param, &htq, &q, &htp, &p);
            return -1;
        }
        if(gerarRelatorioHFD(htp, getNomePM(param)) != 0){
            printf("ERRO: Geracao do relatorio .hfd do arquivo .pm.\n");
            shutProgram(&param, &htq, &q, &htp, &p);
            return -1;
        }
    }
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");

    // 4. PROCESSAR O QRY (Se fornecido)
    // Processa o arquivo .qry e realiza as operações de consulta e modificação nos dados armazenados a partir do arquivo .geo, 
    // conforme as instruções do arquivo .qry
    // Verificar se o objeto foi criado com sucesso
    if (getNomeQry(param) == NULL){
        printf("\n#----------- ARQUIVO .QRY NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    }else{
        printf("\n#-------------------- PROCESSANDO O ARQUIVO .QRY... ---------------------#\n");
        if(processarQry(param, htp, htq) != 0){
            printf("ERRO: Processamento do arquivo .qry.\n");
            shutProgram(&param, &htq, &q, &htp, &p);
            return -1;
        }
        printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    }
    
    // 5: LIBERAR MEMÓRIA ALOCADA PARA PARÂMETROS E ENCERRAR PROGRAMA
    // Implementar a função de liberação de memória para os objetos de Param, da ED utilizada, do Geo e do Qry
    printf("#---------------------- ENCERRANDO O PROGRAMA... ------------------------#\n");
    shutProgram(&param, &htq, &q, &htp, &p);
    printf("#------------------------------------------------------------------------#\n\n\n\n\n");
    printf("\n##################### FIM DA EXECUCAO DO PROGRAMA ########################\n\n");

    return 0;
}