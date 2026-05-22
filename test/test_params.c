#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "unity.h"

// Variável global para ser usada nos testes
Param* param_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes e
void setUp(void){
    freeParametros(param_teste);
    param_teste = NULL;
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    freeParametros(param_teste);
    param_teste = NULL;
}

// 1: Testa se a função criarParametro() retorna um ponteiro não NULL,
// indicando que o objeto de parâmetros foi criado com sucesso
void test_CriarParametro_DeveRetornarNaoNULL(void){
    // 1.1: Cria um objeto de parâmetros usando a função criarParametro() para ser usado nos testes subsequentes
    param_teste = criarParametro();

    // 1.2: Verifica se o ponteiro retornado pela função criarParametro() é diferente de NULL, 
    // indicando que o objeto de parâmetros foi criado com sucesso
    TEST_ASSERT_NOT_NULL(param_teste);
    printf("\n");
}

// 2: Testa se a função processarParametros() retorna 0, 
// indicando que os parâmetros da linha de comando foram processados com sucesso
void test_ProcessarParametros_DeveRetornarZero(void){
    // 2.1: Cria um objeto de parâmetros usando a função criarParametro() para ser usado nos testes subsequentes
    param_teste = criarParametro();

    // 2.2: Cria um array de argumentos simulando a linha de comando para testar a função processarParametros()
    char* argv[] = {"programa", "-f", "arquivo.geo", "-o", "dirSaida"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    // 2.3: Chama a função processarParametros() para processar os parâmetros da linha de comando
    int resultado = processarParametros(param_teste, argc, argv);

    // 2.4: Verifica se o resultado retornado pela função processarParametros() é igual a 0, indicando que os parâmetros foram processados com sucesso
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}

// 3: Testa se a função freeParametros() retorna 0, 
// indicando que a memória alocada para o objeto de parâmetros foi liberada com sucesso
void test_LiberarMemoria_DeveRetornarZero(void){
    // 3.1: Cria um objeto de parâmetros usando a função criarParametro() para ser usado nos testes subsequentes
    param_teste = criarParametro();

    // 3.2: Chama a função freeParametros() para liberar a memória alocada para o objeto de parâmetros
    int resultado = freeParametros(param_teste);

    // 3.3: Verifica se o resultado retornado pela função freeParametros() é igual a 0, 
    // indicando que a memória foi liberada com sucesso
    TEST_ASSERT_EQUAL_INT(0, resultado);

    param_teste = criarParametro();
    printf("\n");
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes dos parâmetros
    printf("\n\n\n#===================== INICIO DOS TESTES DOS PARAMETROS ======================#\n\n");
    
    printf("|----- Teste 01: Criar um Objeto de Parametros - Deve Retornar Nao NULL ------|\n");
    RUN_TEST(test_CriarParametro_DeveRetornarNaoNULL);
    printf("|-----------------------------------------------------------------------------|\n\n\n\n");
    
    printf("|----------- Teste 02: Processar os Parametros da Linha de Comando -----------|\n");
    RUN_TEST(test_ProcessarParametros_DeveRetornarZero);
    printf("|-----------------------------------------------------------------------------|\n\n\n\n");

    printf("|--------------- Teste 03: Liberar a Memoria Alocado ao Objeto ---------------|\n\n");
    RUN_TEST(test_LiberarMemoria_DeveRetornarZero);
    printf("|-----------------------------------------------------------------------------|\n\n");

    printf("#======================= FIM DOS TESTES DOS PARAMETROS =======================#\n\n\n");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}