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
    printf("#-------------------- INICIO DA EXECUCAO DO PROGRAMA --------------------#\n\n");
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1. PROCESSAR PARÂMETROS DA LINHA DE COMANDOS
    Param* param = criarParametro();
    if(param == NULL) return -1;

    printf("\n#---------- PROCESSANDO OS PARAMETROS DA LINHA DE COMANDO... ----------#\n");
    if(processarParametros(param, argc, argv) == -1) {
        printf("ERRO: Processamento dos parâmetros da linha de comando.\n");
        freeParametros(param);
        return -1;
    }

    // 2. PROCESSAR O GEO
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .geo
    // Verificar se o objeto foi criado com sucesso

    printf("\n#---------- PROCESSANDO O ARQUIVO .GEO... ----------#\n");
    if(processarGeo(param) == -1){
        printf("ERRO: Processamento do arquivo .geo.\n");
        freeParametros(param);
        return -1;
    }

    // 3. PROCESSAR O VIA (Se fornecido)
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .via
    // Verificar se o objeto foi criado com sucesso
    if(getNomeVia(param) == NULL){
        printf("\n#---------- ARQUIVO .VIA NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    }else{
        printf("\n#---------- PROCESSANDO O ARQUIVO .VIA... ----------#\n");
        if(processarVia(param) == -1){
            printf("ERRO: Processamento do arquivo .via.\n");
            freeParametros(param);
            return -1;
        }
    }

    // 4. PROCESSAR O QRY (Se fornecido)
    // Criar objeto da estrutura de dados necessária para armazenar os dados do arquivo .qry
    // Verificar se o objeto foi criado com sucesso
    if (getNomeQry(param) == NULL){
        printf("\n#---------- ARQUIVO .QRY NAO FORNECIDO. PULANDO ESTA ETAPA... ----------#\n");
    }else{
        printf("\n#---------- PROCESSANDO O ARQUIVO .QRY... ----------#\n");
        if(processarQry(param) == -1){
            printf("ERRO: Processamento do arquivo .qry.\n");
            freeParametros(param);
            return -1;
        }
    }

    // 5. GERAR ARQUIVOS DE SAÍDA (SVG E TXT)
    printf("\n#---------- GERANDO OS ARQUIVOS DE SAIDA... ----------#\n");
    if(processarSvg(param) == -1){
        printf("ERRO: Geração dos arquivos de saída.\n");
        freeParametros(param);
        return -1;
    }

    // 6. LIBERAR MEMÓRIA ALOCADA PARA PARÂMETROS E ENCERRAR PROGRAMA
    freeParametros(param);
    printf("\n#-------------------- FIM DA EXECUCAO DO PROGRAMA --------------------#\n\n");

    return 0;
}