#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "geo.h"

int main(int argc, char* argv[]){
    system("cls");
    printf("#-------------------- INÍCIO DA EXECUÇÃO DO PROGRAMA --------------------#\n\n");

    // Imprime os parâmetros recebidos da linha de comando
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

    // 3. PROCESSAR O QRY

    // 4. IMPLEMENTAR OS TADS E CONSTRUIR O TRABALHO

    return 0;
}