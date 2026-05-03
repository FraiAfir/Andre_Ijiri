#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"
#include "unity.h"

// Declaração de variáveis globais para os testes
FILE* svg_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes e
void setUp(void){
    // Limpa o ambiente de teste antes de cada teste ser executado, garantindo que cada teste tenha um ambiente limpo e controlado
    svg_teste = NULL; // Define o ponteiro para o arquivo .svg como NULL para garantir que cada teste comece com um ambiente limpo e controlado
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    // Libera os recursos alocados durante os testes para evitar vazamentos de memória e garantir a confiabilidade dos resultados dos testes
    if(svg_teste != NULL){
        fecharSvg(svg_teste);   // Fecha o arquivo .svg criado durante os testes para liberar os recursos alocados
        svg_teste = NULL;       // Define o ponteiro para o arquivo .svg como NULL para garantir que cada teste comece com um ambiente limpo e controlado
    }
}

// 1: Testa se a função criarSvg retorna um ponteiro válido ao criar um arquivo .svg
void test_CriarSvg_DeveRetornarPonteiroValido(void){
    // 1.1: Chama a função criarSvg para criar um arquivo .svg de teste
    svg_teste = criarSvg("teste.svg", 500.0, 500.0);

    // 1.2: Verifica se o ponteiro retornado pela função criarSvg é diferente de NULL, indicando que o arquivo .svg foi criado com sucesso
    TEST_ASSERT_NOT_NULL(svg_teste);
    printf("\n");
}

// 2: Testa se a função desenharFormaSvg retorna 0 ao desenhar uma forma geométrica no arquivo .svg
void test_DesenharFormaSvg_DeveRetornarZero(void){
    // 2.1: Chama a função criarSvg para criar um arquivo .svg de teste
    svg_teste = criarSvg("teste.svg", 500.0, 500.0);

    // 2.2: Verifica se o ponteiro retornado pela função criarSvg é diferente de NULL, indicando que o arquivo .svg foi criado com sucesso
    TEST_ASSERT_NOT_NULL(svg_teste);

    // 2.3: Chama a função desenharFormaSvg para desenhar um retângulo no arquivo .svg de teste e verifica se o retorno é 0, indicando que a forma foi desenhada com sucesso
    int resultado = desenharFormaSvg(svg_teste, "r", 50.0, 50.0, 100.0, 100.0, 2.0, "black", "red");
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}

// 3: Testa se a função fecharSvg retorna 0 ao fechar o arquivo .svg criado durante os testes
void test_FecharSvg_DeveRetornarZero(void){
    // 3.1: Chama a função criarSvg para criar um arquivo .svg de teste
    svg_teste = criarSvg("teste.svg", 500.0, 500.0);

    // 3.2: Verifica se o ponteiro retornado pela função criarSvg é diferente de NULL, indicando que o arquivo .svg foi criado com sucesso
    TEST_ASSERT_NOT_NULL(svg_teste);

    // 3.3: Chama a função fecharSvg para fechar o arquivo .svg criado durante os testes e verifica se o retorno é 0, indicando que o arquivo foi fechado com sucesso
    int resultado = fecharSvg(svg_teste);
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes do SVG, cada teste é executado entre uma chamada de setUp e tearDown 
    // para garantir que cada teste tenha um ambiente limpo e controlado, 
    // evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
    printf("\n\n\n#=================== INICIO DOS TESTES DO SVG ===================#\n\n");
    
    printf("|----- Teste 01: Criar SVG - Deve Retornar Ponteiro Valido ------|\n");
    RUN_TEST(test_CriarSvg_DeveRetornarPonteiroValido);
    printf("|----------------------------------------------------------------|\n\n\n\n");
    
    printf("|-------- Teste 02: Desenhar Forma SVG - Deve Retornar 0 --------|\n");
    RUN_TEST(test_DesenharFormaSvg_DeveRetornarZero);
    printf("|----------------------------------------------------------------|\n\n\n\n");

    printf("|------------ Teste 03: Fechar SVG - Deve Retornar 0 ------------|\n\n");
    RUN_TEST(test_FecharSvg_DeveRetornarZero);
    printf("|----------------------------------------------------------------|\n\n");

    printf("#==================== FIM DOS TESTES DO SVG =====================#\n\n\n");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}