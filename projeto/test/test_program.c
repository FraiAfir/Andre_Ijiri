#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"
#include "unity.h"

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes e
void setUp(void){
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
}

// 1: Testa se a função bootProgram inicializa os objetos do projeto corretamente, retornando 0 em caso de sucesso
void test_IniciarObjetosProjeto_DeveRetornarZero(void){
    // 1: Declara os ponteiros para os objetos a serem inicializados
    Param* param   = NULL;
    TabelaHash* ht = NULL;
    Quadras* q     = NULL;
    hashPM* hp     = NULL;
    Pessoas* p     = NULL;

    // 2: Chama a função bootProgram para inicializar os objetos do projeto
    int resultado = bootProgram(&param, &ht, &q, &hp, &p);

    // 3: Verifica se a função retornou 0, indicando que os objetos foram inicializados corretamente
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");

    // 4: Libera os recursos alocados durante o teste
    shutProgram(&param, &ht, &q, &hp, &p);
}

// 2: Testa se a função shutProgram libera os recursos alocados durante a execução do programa, retornando 0 em caso de sucesso
void test_LiberarRecursos_DeveRetornarZero(void){
    // 1: Declara os ponteiros para os objetos a serem inicializados
    Param* param   = NULL;
    TabelaHash* ht = NULL;
    Quadras* q     = NULL;
    hashPM* hp     = NULL;
    Pessoas* p     = NULL;

    // 2: Chama a função bootProgram para inicializar os objetos do projeto
    int resultadoBoot = bootProgram(&param, &ht, &q, &hp, &p);
    TEST_ASSERT_EQUAL_INT(0, resultadoBoot);

    // 3: Chama a função shutProgram para liberar os recursos alocados durante a execução do programa
    int resultadoShut = shutProgram(&param, &ht, &q, &hp, &p);

    // 4: Verifica se a função retornou 0, indicando que os recursos foram liberados corretamente
    TEST_ASSERT_EQUAL_INT(0, resultadoShut);
    printf("\n");
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes do program, cada teste é executado entre uma chamada de setUp e tearDown 
    // para garantir que cada teste tenha um ambiente limpo e controlado, 
    // evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
    printf("\n\n\n#================= INICIO DOS TESTES DO PROGRAM ================#\n\n");
    
    printf("|----- Teste 01: Iniciar os Objetos do Projeto - Deve Retornar 0 ------|\n");
    RUN_TEST(test_IniciarObjetosProjeto_DeveRetornarZero);
    printf("|----------------------------------------------------------------------|\n\n\n\n");
    
    printf("|------------ Teste 02: Liberar Recursos - Deve Retornar 0 ------------|\n");
    RUN_TEST(test_LiberarRecursos_DeveRetornarZero);
    printf("|----------------------------------------------------------------------|\n\n\n\n");

    printf("#================== FIM DOS TESTES DO PROGRAM ==================#\n\n\n");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}