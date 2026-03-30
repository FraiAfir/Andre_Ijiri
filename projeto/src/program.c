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
int bootProgram(Param** param, Tree** t){
    printf("Inicilizando as estruturas de dados para o programa e alocando os recursos necessarios...");

    // 1: Cria o objeto de parâmetros para armazenar os dados dos argumentos da linha de comando
    *param = criarParametro();
    if(*param == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Parametro\n");
        shutProgram(param, NULL, NULL, NULL);
        return -1;
    } fprintf(stdout, "\nObjeto de Parametro criado com sucesso para armazenar os dados dos argumentos da linha de comando\n");

    // 2: Cria a estrutura de dados necessária para armazenar os dados do arquivo .geo
    *t = criarTree();
    if(*t == NULL){
        fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .geo.\n");
        shutProgram(param, t, NULL, NULL);
        return -1;
    } fprintf(stdout, "Estrutura de dados  criada com sucesso para armazenar os dados do arquivo .geo\n");

    return 0;
}
int shutProgram(Param** param, Tree** t, Geo** geo, Qry** qry){
    printf("Encerrando o programa e liberando os recursos alocados...\n");

    // 1: Libera a memória alocada para os objetos de Parametro
    freeParametros(*param);

    // 2: Libera a estrutura de dados utilizada
    freeTree(*t);

    // 3: Libera a memória alocada para os objetos do Geo
    freeGeo(*geo);

    // 4: Libera a memória alocada para os objetos do Qry (Se necessário)
    freeQry(*qry);

    printf("Programa encerrado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/


