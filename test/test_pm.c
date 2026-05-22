#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pm.h"
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

// 1: Testa se a função montarCaminhoPM realmente monta o caminho completo do arquivo .pm e retorna 0
void test_MontarCaminhoPM_DeveRetornarZero(){
    // 1.1: Cria uma estrutura de parâmetros para ser usada no teste de montagem do caminho do arquivo .pm
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./src/");
    setNomePM(param_teste, "t3.pm");

    // 1.2: Buffer para armazenar o caminho completo do arquivo .pm montado pela função montarCaminhoPM 
    char caminhoPM[512];

    // 1.3: Tenta montar o caminho completo do arquivo .pm 
    // usando a função montarCaminhoPM e verifica se a função retornar 0 (sucesso)
    int resultado = montarCaminhoPM(param_teste, caminhoPM);
    TEST_ASSERT_EQUAL_INT(0, resultado);

    // 1.4: Verifica se o caminho completo do arquivo .pm montado pela função montarCaminhoPM corresponde ao esperado, garantindo que a função monte o caminho corretamente
    TEST_ASSERT_EQUAL_STRING("./src/t3.pm", caminhoPM);
    printf("\n");
}

// 2: Testa se a função readFilePM realmente lê o arquivo .pm e retorna 0
void test_LerArquivoPM_DeveRetornarZero(){
    // 2.1: Cria uma estrutura de parâmetros para ser usada no teste de leitura do arquivo .pm
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./src/");
    setNomePM(param_teste, "t3.pm");

    // 2.2: Buffer para armazenar o caminho completo do arquivo .pm montado pela função montarCaminhoPM
    char caminhoPM[512];
    montarCaminhoPM(param_teste, caminhoPM);

    // 2.3: Tenta abrir o arquivo .pm para leitura e verifica se o arquivo foi aberto com sucesso
    FILE* arquivoPM = fopen(caminhoPM, "r");
    TEST_ASSERT_NOT_NULL(arquivoPM);

    // 2.4: Tenta ler o arquivo .pm usando a função readFilePM e verifica se a função retornar 0 (sucesso)
    TabelaHash* hq = criarHash("teste.hf");
    hashPM* hp = criarHashPM("testePM.hf");
    Pessoas* p = criarPessoa();
    TEST_ASSERT_EQUAL_INT(0, readFilePM(arquivoPM, hp, hq, p));
    printf("\n");

    // 2.5: Fecha o arquivo .pm após a leitura para liberar os recursos alocados durante o teste
    fclose(arquivoPM);
}

// 3: Testa se a função processarPM realmente processa o arquivo .pm e retorna 0
void test_ProcessarArquivoPM_DeveRetornarZero(){
    // 3.1: Cria uma estrutura de parâmetros para ser usada no teste de processamento do arquivo .pm
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./src/");
    setNomePM(param_teste, "t3.pm");

    // 3.2: Tenta processar o arquivo .pm usando a função processarPM e verifica se a função retornar 0 (sucesso)
    TabelaHash* hq = criarHash("teste.hf");
    hashPM* hp = criarHashPM("testePM.hf");
    Pessoas* p = criarPessoa();
    TEST_ASSERT_EQUAL_INT(0, processarPM(param_teste, hp, hq, p));
    printf("\n");
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes do PM
    printf("\n\n\n#================= INICIO DOS TESTES DO PM ================#\n\n");
    
    printf("|----- Teste 01: Montar Caminho PM - Deve Retornar 0 ------|\n");
    RUN_TEST(test_MontarCaminhoPM_DeveRetornarZero);
    printf("|----------------------------------------------------------|\n\n\n\n");
    
    printf("|------- Teste 02: Ler Arquivo PM - Deve Retornar 0 -------|\n");
    RUN_TEST(test_LerArquivoPM_DeveRetornarZero);
    printf("|----------------------------------------------------------|\n\n\n\n");

    printf("|-------- Teste 03: Processar Arquivo PM (Wrapper) --------|\n\n");
    RUN_TEST(test_ProcessarArquivoPM_DeveRetornarZero);
    printf("|----------------------------------------------------------|\n\n");

    printf("#================== FIM DOS TESTES DO PM ==================#\n\n\n");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}