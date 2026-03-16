#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "params.h"
#include "program.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"

int main(int argc, char* argv[]){
    system("cls");
    printf("##################### INICIO DA EXECUCAO DO PROGRAMA #####################\n\n");
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1: Cria os objetos das estruturas necssárias para a execução do código
    Param* param = NULL; // Declara o objeto de parâmetros
    Tree*  t     = NULL; // Declara o objeto da ED para armazenar os dados dos arquivos de entrada
    Geo*   geo   = NULL; // Declara o objeto para armazenar os dados de uma das linha do arquivo .geo
    Qry*   qry   = NULL; // Declara o objeto para armazenar os dados de uma das linha do arquivo .qry (Se necessário)

    // Inicializa os objetos de Parametro e da ED para armazenar os dados do arquivo .geo
    int init = bootProgram(&param, &t, &geo, &qry);
    if(init == -1) {
        printf("ERRO: Inicializacao dos objetos.\n");
        return -1;
    }

    // 2. PROCESSAR PARÂMETROS DA LINHA DE COMANDOS
    printf("\n#------------ PROCESSANDO OS PARAMETROS DA LINHA DE COMANDO... ----------#\n");
    if(processarParametros(param, argc, argv) == -1) {
        printf("ERRO: Processamento dos parametros da linha de comando.\n");
        shutProgram(&param, &t, &geo, &qry);
        return -1;
    }
    printf("#------------------------------------------------------------------------#\n\n");

    // 3. PROCESSAR O GEO
    // Processa o arquivo .geo e armazena os dados em uma estrutura de dados adequada (Lista, Árvore, etc.)
    // Verificar se o objeto foi criado com sucesso
    printf("\n#-------------------- PROCESSANDO O ARQUIVO .GEO... ---------------------#\n");
    if(processarGeo(param, t) == -1){
        printf("ERRO: Processamento do arquivo .geo.\n");
        shutProgram(&param, &t, &geo, &qry);
        return -1;
    } 
    printf("#------------------------------------------------------------------------#\n\n");

    // 4. PROCESSAR O VIA (Se fornecido)
    // Processa o arquivo .via e armazena os dados em uma estrutura de dados adequada (Lista, Árvore, etc.)
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
    // Processa o arquivo .qry e realiza as operações de consulta e modificação nos dados armazenados a partir do arquivo .geo, 
    // conforme as instruções do arquivo .qry
    // Verificar se o objeto foi criado com sucesso
    if (getNomeQry(param) == NULL){
        printf("\n#----------- ARQUIVO .QRY NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    }else{
        printf("\n#-------------------- PROCESSANDO O ARQUIVO .QRY... ---------------------#\n");
        if(processarQry(param) == -1){
            printf("ERRO: Processamento do arquivo .qry.\n");
            shutProgram(&param, &t, &geo, &qry);
            return -1;
        }
        printf("#------------------------------------------------------------------------#\n\n");
    }

    // 5. GERAR ARQUIVOS DE SAÍDA (SVG E TXT)
    printf("\n#------------------- GERANDO OS ARQUIVOS DE SAIDA... --------------------#\n");
    if(processarSvg(param) == -1){
        printf("ERRO: Geracao dos arquivos de saida.\n");
        shutProgram(&param, &t, &geo, &qry);
        return -1;
    }
    // if(processarTxt(param) == -1){
    //     printf("ERRO: Geracao do arquivo .txt de saida.\n");
    //     freeParametros(param, t);
    //     return -1;
    // }
    printf("#------------------------------------------------------------------------#\n\n");

    // 6. LIBERAR MEMÓRIA ALOCADA PARA PARÂMETROS E ENCERRAR PROGRAMA
    // Implementar a função de liberação de memória para os objetos de Param, da ED utilizada, do Geo e do Qry    
    shutProgram(&param, &t, &geo, &qry);
    printf("\n##################### FIM DA EXECUCAO DO PROGRAMA ########################\n\n");

    return 0;
}