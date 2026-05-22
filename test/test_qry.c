#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "unity.h"

// Variável global para ser usada nos testes
TabelaHash* tabelaQ_teste = NULL;
hashPM*     tabelaP_teste = NULL;
Param*      param_teste   = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes e
void setUp(void){
    freeHash(tabelaQ_teste);
    freeHashPM(tabelaP_teste);
    freeParametros(param_teste);
    tabelaQ_teste = NULL;
    tabelaP_teste = NULL;
    param_teste = NULL;

    remove("hashPM_teste.hf");
    remove("hashQ_teste.hfc");
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    freeHash(tabelaQ_teste);
    freeHashPM(tabelaP_teste);
    freeParametros(param_teste);
    tabelaQ_teste = NULL;
    tabelaP_teste = NULL;
    param_teste = NULL;

    remove("hashPM_teste.hf");
    remove("hashQ_teste.hfc");
}

// 1: Testar se a função processarQry retorna 0 em caso de sucesso
void test_ProcessarQry_DeveRetornarZero(void){
    // 1.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./src/");
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");

    // 1.2: Criar tabelas hash de teste para pessoas e quadras
    tabelaP_teste = criarHashPM("hashPM_teste.hf");
    tabelaQ_teste = criarHash("hashQ_teste.hf");

    // 1.3: Popular as tabelas hash de teste com dados fictícios 
    // para garantir que a função processarQry tenha dados para processar durante o teste
    inserirRegPM(tabelaP_teste, "p1", "Fulano", "Beltrano", "M", "01/01/1990");
    inserirReg(tabelaQ_teste, "q1", 10.0, 20.0, 30.0, 40.0, "1.0px", "red", "black");

    // 1.4: Chamar a função processarQry com os parâmetros e tabelas hash de teste
    int resultado = processarQry(param_teste, tabelaP_teste, tabelaQ_teste);

    // 1.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}

// 2: Testar se a função montarCaminhoQry retorna 0 em caso de sucesso
void test_MontarCaminhoQry_DeveRetornarZero(void){
    // 2.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./src/");
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    char caminhoQry[256];

    // 2.2: Chamar a função montarCaminhoQry com os parâmetros de teste
    int resultado = montarCaminhoQry(param_teste, caminhoQry);

    // 2.3: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}

// 3: Testar se a função readFileQry retorna 0 em caso de sucesso
void test_LerArquivoQry_DeveRetornarZero(void){
    // 3.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setDirEntradaCompleto(param_teste, "./src/");
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    
    // 3.2: Buffer para armazenar o caminho completo do arquivo .qry montado pela função montarCaminhoQry
    char caminhoQry[256];
    montarCaminhoQry(param_teste, caminhoQry);

    // 3.2: Criar tabelas hash de teste para pessoas e quadras
    tabelaP_teste = criarHashPM("hashPM_teste.hf");
    tabelaQ_teste = criarHash("hashQ_teste.hfc");

    // 3.3: Abrir o arquivo .qry para leitura
    FILE* arquivoQry = fopen(caminhoQry, "r");

    // 3.4: Chamar a função readFileQry com os parâmetros e tabelas hash de teste
    int resultado = readFileQry(arquivoQry, tabelaP_teste, tabelaQ_teste, param_teste);

    // 3.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");

    // 3.6: Fechar o arquivo .qry após o teste
    fclose(arquivoQry);
}

// 4: Testar se a função criarQry retorna um ponteiro válido (não NULL)
void test_CriarQry_DeveRetornarPonteiroValido(void){
    // 4.1: Chamar a função criarQry para criar um objeto de QRY
    Qry* qry = criarQry();

    // 4.2: Verificar se o ponteiro retornado é válido (não NULL)
    TEST_ASSERT_NOT_NULL(qry);
    printf("\n");

    // 4.3: Liberar o objeto de QRY criado para evitar vazamento de memória
    free(qry);
}

// 5: Testar se a função removerQuadra retorna 0 em caso de sucesso
void test_RemoverQuadra_DeveRetornarZero(void){
    // 5.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 5.2: Criar tabelas hash de teste para pessoas e quadras
    tabelaP_teste = criarHashPM("hashPM_teste.hf");
    tabelaQ_teste = criarHash("hashQ_teste.hfc");

    // 5.3: Abrir os arquivos de saída para os resultados do teste
    FILE* txt = fopen("saida_teste.txt", "w");    
    FILE* qrySVG = fopen("saida_teste.svg", "w");

    // 5.4: Chamar a função removerQuadra com os parâmetros e tabelas hash de teste
    int resultado = removerQuadraQRY(tabelaP_teste, "q1", txt, tabelaQ_teste, qrySVG);

    // 5.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
    
    // 5.6: Fechar os arquivos de saída após o teste
    fclose(txt);
    fclose(qrySVG);
}

// 6: Testar se a função calcMoradores retorna 0 em caso de sucesso
void test_CalcularMoradores_DeveRetornarZero(void){
    // 6.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 6.2: Criar tabelas hash de teste para pessoas e quadras
    tabelaP_teste = criarHashPM("hashPM_teste.hf");
    tabelaQ_teste = criarHash("hashQ_teste.hfc");

    // 6.3: Abrir os arquivos de saída para os resultados do teste
    FILE* txt = fopen("saida_teste.txt", "w");
    FILE* qrySVG = fopen("saida_teste.svg", "w");

    // 6.4: Chamar a função calcMoradores com os parâmetros e tabelas hash de teste
    int resultado = calcMoradores(tabelaP_teste, tabelaQ_teste, "q1", txt, qrySVG);

    // 6.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
    
    // 6.6: Fechar os arquivos de saída após o teste
    fclose(txt);
    fclose(qrySVG);
}

// 7: Testar se a função produzirCenso retorna 0 em caso de sucesso
void test_ProduzirCenso_DeveRetornarZero(void){
    // 7.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 7.2: Criar tabela hash de teste para pessoas
    tabelaP_teste = criarHashPM("hashPM_teste.hf");

    // 7.3: Abrir o arquivo de saída para os resultados do teste
    FILE* txt = fopen("saida_censo_teste.txt", "w");

    // 7.4: Chamar a função produzirCenso com os parâmetros e tabelas hash de teste
    int resultado = produzirCenso(tabelaP_teste, txt);

    // 7.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
    
    // 7.6: Fechar o arquivo de saída após o teste
    fclose(txt);
}

// 8: Testar se a função obterDadosHabitante retorna 0 em caso de sucesso
void test_ObterDadosHabitante_DeveRetornarZero(void){
    // 8.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 8.2: Criar tabela hash de teste para pessoas
    tabelaP_teste = criarHashPM("hashPM_teste.hf");

    // 8.3: Abrir o arquivo de saída para os resultados do teste
    FILE* txt = fopen("saida_dados_habitante_teste.txt", "w");

    // 8.4: Chamar a função obterDadosHabitante com os parâmetros e tabelas hash de teste
    int resultado = obterDadosHabitante(tabelaP_teste, "p1", txt);

    // 8.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");

    // 8.6: Fechar o arquivo de saída após o teste
    fclose(txt);
}

// 9: Testar se a função registrarNascimento retorna 0 em caso de sucesso
void test_RegistrarNascimento_DeveRetornarZero(void){
    // 9.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 9.2: Criar tabela hash de teste para pessoas
    tabelaP_teste = criarHashPM("hashPM_teste.hf");

    // 9.3: Abrir o arquivo de saída para os resultados do teste
    FILE* txt = fopen("saida_registrar_nascimento_teste.txt", "w");

    // 9.4: Chamar a função registrarNascimento com os parâmetros e tabelas hash de teste
    int resultado = registrarNascimento(tabelaP_teste, "p1", "Joao", "Silva", 'M', "01/01/2000", txt);

    // 9.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");

    // 9.6: Fechar o arquivo de saída após o teste
    fclose(txt);
}

// 10: Testar se a função registrarObito retorna 0 em caso de sucesso
void test_RegistrarObito_DeveRetornarZero(void){
    // 10.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 10.2: Criar tabela hash de teste para pessoas
    tabelaP_teste = criarHashPM("hashPM_teste.hf");

    // 10.3: Abrir os arquivos de saída para os resultados do teste
    FILE* txt = fopen("saida_registrar_obito_teste.txt", "w");
    FILE* qrySVG = fopen("saida_registrar_obito_teste.svg", "w");

    // 10.4: Chamar a função registrarObito com os parâmetros e tabelas hash de teste
    int resultado = registrarObito(tabelaP_teste, tabelaQ_teste, "p1", txt, qrySVG);

    // 10.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");

    // 10.6: Fechar os arquivos de saída após o teste
    fclose(txt);
    fclose(qrySVG);
}

// 11: Testar se a função registrarMudanca retorna 0 em caso de sucesso
void test_RegistrarMudanca_DeveRetornarZero(void){
    // 11.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 11.2: Criar tabelas hash de teste para pessoas e quadras
    tabelaP_teste = criarHashPM("hashPM_teste.hf");
    tabelaQ_teste = criarHash("hashQ_teste.hfc");

    // 11.3: Abrir os arquivos de saída para os resultados do teste
    char face[2] = "N";
    FILE* txt = fopen("saida_registrar_mudanca_teste.txt", "w");
    FILE* qrySVG = fopen("saida_registrar_mudanca_teste.svg", "w");

    // 11.4: Chamar a função registrarMudanca com os parâmetros e tabelas hash de teste
    int resultado = registrarMudanca(tabelaP_teste, tabelaQ_teste, "p1", "q1", *face, 10, "compl", txt, qrySVG);

    // 11.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");

    // 11.6: Fechar os arquivos de saída após o teste
    fclose(txt);
    fclose(qrySVG);
}

// 12: Testar se a função registrarDespejo retorna 0 em caso de sucesso
void test_RegistrarDespejo_DeveRetornarZero(void){
    // 12.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 12.2: Criar tabelas hash de teste para pessoas e quadras
    tabelaP_teste = criarHashPM("hashPM_teste.hf");
    tabelaQ_teste = criarHash("hashQ_teste.hfc");

    // 12.3: Abrir os arquivos de saída para os resultados do teste
    FILE* txt = fopen("saida_registrar_despejo_teste.txt", "w");
    FILE* qrySVG = fopen("saida_registrar_despejo_teste.svg", "w");

    // 12.4: Chamar a função registrarDespejo com os parâmetros e tabelas hash de teste
    int resultado = registrarDespejo(tabelaP_teste, tabelaQ_teste, "p1", txt, qrySVG);

    // 12.5: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
    
    // 12.6: Fechar os arquivos de saída após o teste
    fclose(txt);
    fclose(qrySVG);
}

// 13: Testar se a função clonarSvgBase retorna um ponteiro válido (não NULL)
void test_ClonarSvgBase_DeveRetornarPonteiroValido(void){
    // 13.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 13.2: Chamar a função clonarSvgBase para criar um objeto de SVG
    FILE* svg = clonarSvgBase("./src/t1.svg", "saida_clonar_svg_base_teste.svg");

    // 13.3: Verificar se o ponteiro retornado é válido (não NULL)
    TEST_ASSERT_NOT_NULL(svg);
    printf("\n");

    // 13.4: Fechar o arquivo de saída após o teste
    fclose(svg);
}

// 14: Testar se a função removerPessoa retorna 0 em caso de sucesso
void test_RemoverPessoa_DeveRemoverRegistroCorreto(void){
    // 14.1: Criar uma estrutura de parâmetros de teste
    param_teste = criarParametro();
    setNomeGeo(param_teste, "t1.geo");
    setNomeQry(param_teste, "t2.qry");
    setDirEntrada(param_teste, "./src");
    setDirSaida(param_teste, "./");

    // 14.2: Criar tabela hash de teste para pessoas
    tabelaP_teste = criarHashPM("hashPM_teste.hf");

    // 14.3: Chamar a função removerPessoa com os parâmetros e tabelas hash de teste
    int resultado = removerPessoa(tabelaP_teste, "p1");

    // 14.4: Verificar se o resultado é 0 (sucesso)
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes de QRY
    printf("\n\n\n#================================ INICIO DOS TESTES DE QRY ================================#\n\n");
    
    printf("|---------------------- Teste 01: Processar o QRY - Deve Retornar 0 -----------------------|\n\n");
    RUN_TEST(test_ProcessarQry_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|-------------------- Teste 01: Montar o Caminho QRY - Deve Retornar 0 --------------------|\n\n");
    RUN_TEST(test_MontarCaminhoQry_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|--------------------- Teste 01: Ler o Arquivo QRY - Deve Retornar 0 ----------------------|\n\n");
    RUN_TEST(test_LerArquivoQry_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");
    
    printf("|----------- Teste 02: Criar QRY - Deve Retornar um Ponteiro Valido (Nao NULL) ------------|\n");
    RUN_TEST(test_CriarQry_DeveRetornarPonteiroValido);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");
    
    printf("|----------------------- Teste 03: Remover Quadra - Deve Retornar 0 -----------------------|\n\n");
    RUN_TEST(test_RemoverQuadra_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|------------------- Teste 04: Calcular os Moradores - Deve retornar 0 --------------------|\n\n");
    RUN_TEST(test_CalcularMoradores_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|---------------------- Teste 05: Produzir o Censo - Deve retornar 0 ----------------------|\n\n");
    RUN_TEST(test_ProduzirCenso_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|---------------- Teste 06: Obter Dados de um Habitante - Deve retornar 0 -----------------|\n\n");
    RUN_TEST(test_ObterDadosHabitante_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|-------------------- Teste 07: Registrar Nascimento - Deve retornar 0 --------------------|\n\n");
    RUN_TEST(test_RegistrarNascimento_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|----------------------- Teste 08: Registrar Obito - Deve retornar 0 ----------------------|\n\n");
    RUN_TEST(test_RegistrarObito_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|--------------------- Teste 09: Registrar Mudanca - Deve retornar 0 ----------------------|\n\n");
    RUN_TEST(test_RegistrarMudanca_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n");

    printf("|---------------------- Teste 09: Registrar Despejo - Deve retornar 0 ---------------------|\n\n");
    RUN_TEST(test_RegistrarDespejo_DeveRetornarZero);
    printf("|------------------------------------------------------------------------------------------|\n\n");

    printf("|----------- Teste 09: Clonar o SVG Base - Deve retornar um ponteiro valido ---------------|\n\n");
    RUN_TEST(test_ClonarSvgBase_DeveRetornarPonteiroValido);
    printf("|------------------------------------------------------------------------------------------|\n\n");

    printf("|------------------- Teste 09: Calcular Coordenadas do Endereco - Deve  -------------------|\n\n");
    RUN_TEST(test_RemoverPessoa_DeveRemoverRegistroCorreto);
    printf("|------------------------------------------------------------------------------------------|\n\n");

    printf("#================================== FIM DOS TESTES DE QRY =================================#\n\n\n");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}