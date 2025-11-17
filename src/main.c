#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "via.h"

int main(int argc, char* argv[]){
    system("cls");
    printf("#-------------------- INÍCIO DA EXECUÇÃO DO PROGRAMA --------------------#\n\n");
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1. PROCESSAR PARÂMETROS DA LINHA DE COMANDOS
    Param* param = criarParametro();
    if(param == NULL) return -1;

    printf("#---------- PROCESSANDO OS PARÂMETROS DA LINHA DE COMANDO... ----------#\n");
    if(processarParametros(param, argc, argv) == -1) {
        printf("ERRO: Processamento dos parâmetros da linha de comando.\n");
        freeParametros(param);
        return -1;
    }

    // 2. PROCESSAR O GEO
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .geo
    // Verificar se o objeto foi criado com sucesso

    printf("#---------- PROCESSANDO O ARQUIVO .GEO... ----------#\n");
    if(processarGeo(param) == -1){
        printf("ERRO: Processamento do arquivo .geo.\n");
        freeParametros(param);
        return -1;
    }

    // 3. PROCESSAR O VIA
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .via
    // Verificar se o objeto foi criado com sucesso

    printf("#---------- PROCESSANDO O ARQUIVO .VIA... ----------#\n");
    if(processarVia(param) == -1){
        printf("ERRO: Processamento do arquivo .via.\n");
        freeParametros(param);
        return -1;
    }

    // 4. PROCESSAR O QRY
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .qry
    // Verificar se o objeto foi criado com sucesso

    printf("#---------- PROCESSANDO O ARQUIVO .QRY... ----------#\n");
    if(processarQry(param) == -1){
        printf("ERRO: Processamento do arquivo .qry.\n");
        freeParametros(param);
        return -1;
    }


    // 5. GERAR ARQUIVOS DE SAÍDA (SVG E TXT)
    printf("#---------- GERANDO OS ARQUIVOS DE SAÍDA... ----------#\n");
    if(processarSvg(param) == -1){
        printf("ERRO: Geração dos arquivos de saída.\n");
        freeParametros(param);
        return -1;
    }

    // 6. LIBERAR MEMÓRIA ALOCADA PARA PARÂMETROS E ENCERRAR PROGRAMA
    freeParametros(param);
    printf("\n#-------------------- FIM DA EXECUÇÃO DO PROGRAMA --------------------#\n");

    return 0;
}