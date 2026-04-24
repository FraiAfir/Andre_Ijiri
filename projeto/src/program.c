#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "program.h"

/*                                       FUNÇÕES AUXILIARES                                      */
// bool cmpFuncGeo(void* info1, void* info2){

//     // Converter os ponteiros void* para o tipo específico de informação armazenada nos nós da árvore
//     // Atribuir os ponteiros convertidos a variáveis do tipo específico para facilitar a comparação
//     Geo* key1 = (Geo*)info1;
//     Geo* key2 = (Geo*)info2;

//     // Implementação da função de comparação entre as informações dos nós
//     // ...

//     return true; // ou false, dependendo da comparação
// }
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int bootProgram(Param** param, TabelaHash** h, Quadras** q){
    printf("Inicilizando os objetos e alocando os recursos necessarios:\n");

    // 1: Cria o objeto de parâmetros para armazenar os dados dos argumentos da linha de comando
    printf("\tCriando o objeto de Parametro para armazenar os dados dos argumentos da linha de comando...\n");
    *param = criarParametro();
    if(*param == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Parametro\n");
        shutProgram(param, NULL, NULL);
        return -1;
    } fprintf(stdout, "\tObjeto de Parametro criado com sucesso para armazenar os dados dos argumentos da linha de comando\n");

    printf("\n");

    // 2: Cria a estrutura de dados necessária para armazenar os dados do arquivo .geo
    printf("\tCriando a Tabela Hash para armazenar os dados do arquivo .geo...\n");
    *h = criarHash("quadras.hf");
    if(*h == NULL){
        fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .geo.\n");
        shutProgram(param, h, NULL);
        return -1;
    } fprintf(stdout, "\tTabela Hash criada com sucesso para armazenar os dados do arquivo .geo\n");

    printf("\n");

    // 3: Criar o objeto para armazenar os dados lidos do arquivo .geo
    printf("\tCriando o objeto de Quadras para armazenar os dados do arquivo .geo...\n");
    *q = criarQuadra();
    if(*q == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Quadras\n");
        shutProgram(param, h, q);
        return -1;
    } fprintf(stdout, "\tObjeto de Quadras criado com sucesso para armazenar os dados do arquivo .geo\n");

    printf("Objetos e recursos alocados com sucesso!\n");
    return 0;
}
int shutProgram(Param** param, TabelaHash** h, Quadras** q){
    printf("Encerrando o programa e liberando os recursos alocados...\n");

    // 1: Libera a memória alocada para os objetos de Parametro
    freeParametros(*param);

    // 2: Libera a estrutura de dados utilizada
    freeHash(*h);

    // 3: Libera a memória alocada para o objeto de Quadras
    freeQuadras(*q);

    printf("\nPrograma encerrado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/


