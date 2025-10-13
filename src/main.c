#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "digraph.h"
#include "streap.h"
#include "params.h"

int main(int argc, char* argv[]){
    // Imprime os parâmetros recebidos da linha de comando
    for(int i = 0; i < argc; i++) {printf("Argumento do argv[%d]: %s\n", i, argv[i]);}

    // 1. PROCESSAR PARÂMETROS
    Param* param = criarParametro();
    if(param == NULL) return -1;

    if(processarParametros(param, argc, argv) == -1) {
        freeParametros(param);
        return -1;
    }

    // 2. PROCESSAR O GEO


    // 3. VERIFICAR SE O PROCESSAMENTO ESTÁ CORRETO E ARMAZENAR OS DADOS

    // 4. PROCESSAR O QRY

    // 5. IMPLEMENTAR OS TADS E CONSTRUIR O TRABALHO

    return 0;
}