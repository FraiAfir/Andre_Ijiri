#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"
#include "params.h"
#include "program.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "via.h"

int main(int argc, char* argv[]){
    system("cls");
    printf("##################### INICIO DA EXECUCAO DO PROGRAMA #####################\n\n");
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1: Cria os objetos das estruturas necssárias para a execução do código
    Param* param = NULL;              // Declara o objeto de parâmetros
    Lista* l     = NULL;              // Declara o objeto da ED para armazenar os dados dos arquivos de entrada
    Geo*   geo   = NULL;              // Declara o objeto para armazenar os dados de uma das linha do arquivo .geo
    Qry*   qry   = NULL;              // Declara o objeto para armazenar os dados de uma das linha do arquivo .qry (Se necessário)
    bootProgram(param, l, geo, qry);  // Inicializa os objetos declarados acima

    // 2. PROCESSAR PARÂMETROS DA LINHA DE COMANDOS
    printf("\n#------------ PROCESSANDO OS PARAMETROS DA LINHA DE COMANDO... ----------#\n");
    if(processarParametros(param, argc, argv) == -1) {
        printf("ERRO: Processamento dos parametros da linha de comando.\n");
        shutProgram(param, l, geo, qry);
        return -1;
    }
    printf("#------------------------------------------------------------------------#\n\n");

    // 3. PROCESSAR O GEO
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .geo
    // Verificar se o objeto foi criado com sucesso
    printf("\n#-------------------- PROCESSANDO O ARQUIVO .GEO... ---------------------#\n");
    if(processarGeo(param, l) == -1){
        printf("ERRO: Processamento do arquivo .geo.\n");
        shutProgram(param, l, geo, qry);
        return -1;
    }
    printf("#------------------------------------------------------------------------#\n\n");

    // 4. PROCESSAR O VIA (Se fornecido)
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .via
    // Verificar se o objeto foi criado com sucesso
    // if(getNomeVia(param) == NULL){
    //     printf("\n#----------- ARQUIVO .VIA NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    // }else{
    //     printf("\n#---------- PROCESSANDO O ARQUIVO .VIA... ----------#\n");
    //     if(processarVia(param) == -1){
    //         printf("ERRO: Processamento do arquivo .via.\n");
    //         freeParametros(param);
    //         return -1;
    //     }
    // }

    // 4. PROCESSAR O QRY (Se fornecido)
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .qry
    // Verificar se o objeto foi criado com sucesso
    if (getNomeQry(param) == NULL){
        printf("\n#----------- ARQUIVO .QRY NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    }else{
        printf("\n#-------------------- PROCESSANDO O ARQUIVO .QRY... ---------------------#\n");
        if(processarQry(param) == -1){
            printf("ERRO: Processamento do arquivo .qry.\n");
            shutProgram(param, l, geo, qry);
            return -1;
        }
        printf("#------------------------------------------------------------------------#\n\n");
    }

    // 5. GERAR ARQUIVOS DE SAÍDA (SVG E TXT)
    printf("\n#------------------- GERANDO OS ARQUIVOS DE SAIDA... --------------------#\n");
    if(processarSvg(param) == -1){
        printf("ERRO: Geracao dos arquivos de saida.\n");
        shutProgram(param, l, geo, qry);
        return -1;
    }
    // if(processarTxt(param) == -1){
    //     printf("ERRO: Geracao do arquivo .txt de saida.\n");
    //     freeParametros(param, l);
    //     return -1;
    // }
    printf("#------------------------------------------------------------------------#\n\n");

    // 6. LIBERAR MEMÓRIA ALOCADA PARA PARÂMETROS E ENCERRAR PROGRAMA
    // Implementar a função de liberação de memória para os objetos de Param, da ED utilizada, do Geo e do Qry    
    shutProgram(param, l, geo, qry);
    printf("\n##################### FIM DA EXECUCAO DO PROGRAMA ########################\n\n");

    return 0;
}