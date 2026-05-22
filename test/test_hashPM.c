#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "hashPM.h"

// Variável global para ser usada nos testes
hashPM* tabela_teste = NULL;

/************************************** FUNÇÕES A SEREM TESTADAS ***************************************/
// Função de configuração (setUp) para inicializar o ambiente de teste antes de cada teste ser executado
// A função setUp é chamada automaticamente pelo framework de teste Unity antes de cada teste ser executado, 
// garantindo que cada teste tenha um ambiente limpo e controlado, evitando interferências entre os testes
void setUp(void){
    freeHashPM(tabela_teste);
    tabela_teste = NULL;
    
    // Remove os arquivos de teste da tabela hash, caso existam
    remove("hashPM_teste.hf");
    remove("hashPM_teste.hfc");
}

// Função de limpeza (tearDown) para liberar os recursos alocados durante os testes
// A função tearDown é chamada automaticamente pelo framework de teste Unity após cada teste ser executado, 
// garantindo que os recursos alocados durante os testes sejam liberados, 
// evitando vazamentos de memória e garantindo a confiabilidade dos resultados dos testes.
void tearDown(void){
    freeHashPM(tabela_teste);
    tabela_teste = NULL;

    // Remove os arquivos de teste da tabela hash, caso existam
    remove("hashPM_teste.hf");
    remove("hashPM_teste.hfc");
}

// 1: Testa se a função criarHash realmente cria um objeto de tabela hash e não retorna NULL
void test_CriarHash_NaoDeveRetornarNull(){
    // 1.1: Cria uma tabela hash para ser usada no teste de criação da tabela hash
    tabela_teste = criarHashPM("hashPM_teste.hf"); 
    
    // 1.2: Verifica a criação de uma tabela hash usando a função criarHash, passando um nome de arquivo de teste
    TEST_ASSERT_NOT_NULL(tabela_teste);
    printf("\n");
}

// 2: Testa se a função criarPessoa realmente cria um objeto de pessoa e não retorna NULL
void test_CriarPessoa_NaoDeveRetornarNull(){
    // 2.1: Cria uma pessoa usando a função criarPessoa
    Pessoas* p = criarPessoa();

    // 2.2: Verifica a criação da pessoa usando a função criarPessoa
    TEST_ASSERT_NOT_NULL(p);
    printf("\n");

    // 2.3: Libera a memória alocada para a pessoa criada durante o teste para evitar vazamentos de memória
    freePessoa(p);
}

// 3: Testa se a função inserirReg insere um novo registro corretamente e retorna 0
void test_InserirRegPM_DeveRetornarZero(){
    // 3.1: Cria uma tabela hash para ser usada no teste de inserção de registro
    tabela_teste = criarHashPM("hashPM_teste.hf");

    // 3.2: Tenta inserir um novo registro com uma chave única
    int resultado = inserirRegPM(tabela_teste, "123.456.789-00", "João", "Silva", "M", "1990-01-01");

    // 3.3: Verifica se a função retornar 0 (sucesso) para a inserção de uma nova chave
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}

// 4: Testa se a função splitBucket divide corretamente um bucket cheio e retorna 0
void test_DividirBucket_DeveRetornarZero(){
    // 4.1: Cria uma tabela hash para ser usada no teste de divisão de bucket
    tabela_teste = criarHashPM("hashPM_teste.hf");

    // 4.2: Insere registros suficientes para preencher um bucket e causar um split
    inserirRegPM(tabela_teste, "123.456.789-00", "João", "Silva", "M", "1990-01-01");
    inserirRegPM(tabela_teste, "987.654.321-00", "Maria", "Santos", "F", "1985-05-15");
    inserirRegPM(tabela_teste, "456.789.123-00", "Pedro", "Oliveira", "M", "1992-12-10");
    inserirRegPM(tabela_teste, "321.654.987-00", "Ana", "Costa", "F", "1988-08-20");
    int resultado = inserirRegPM(tabela_teste, "654.321.987-00", "Carlos", "Almeida", "M", "1995-03-25");

    // 4.3: Verifica se a função retornar 0 (sucesso) para a inserção que causou o split do bucket
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}

// 5: Testa se a função salvarDiretorioHFC salva corretamente o diretório da tabela hash em um arquivo .hfc e retorna 0
void test_SalvarDiretorioHFC_PM_DeveRetornarZero(){
    // 5.1: Cria uma tabela hash para ser usada no teste de salvamento do diretório HFC
    tabela_teste = criarHashPM("hashPM_teste.hf");

    // 5.2: Tenta salvar o diretório da tabela hash em um arquivo de saída no formato .hfc
    int resultado = salvarDiretorioHFC_PM(tabela_teste, "hashPM_teste.hfc");

    // 5.3: Verifica se a função retornar 0 (sucesso) para o salvamento do diretório da tabela hash em um arquivo .hfc
    TEST_ASSERT_EQUAL_INT(0, resultado);
    printf("\n");
}

// 6: Testa se a função carregarDiretorioHFC carrega corretamente o diretório da tabela hash a partir de um arquivo .hfc
// e retorna um ponteiro válido
void test_CarregarDiretorioPM_DeveRetornarPonteiroValido(){
    // 6.1: Cria uma tabela hash para ser usada no teste de carregamento do diretório HFC
    tabela_teste = criarHashPM("hashPM_teste.hf");

    // 6.2: Salva o diretório da tabela hash em um arquivo de saída no formato .hfc 
    // para garantir que o arquivo exista para o teste de carregamento
    salvarDiretorioHFC_PM(tabela_teste, "hashPM_teste.hfc");

    // 6.3: Tenta carregar o diretório da tabela hash a partir do arquivo .hfc criado
    hashPM* dir_carregado = carregarDiretorioPM("hashPM_teste.hfc", "hashPM_teste.hf");

    // 6.4: Verifica se a função retorna um ponteiro válido (não NULL) para o diretório carregado a partir do arquivo .hfc
    TEST_ASSERT_NOT_NULL(dir_carregado);
    printf("\n");

    // 6.5: Libera a memória alocada para a tabela hash carregada durante o teste para evitar vazamentos de memória
    freeHashPM(dir_carregado);
}

// 7: Testa se a função hashFuncPM gera um valor de hash consistente para a mesma chave
void test_HashFuncPM_DeveGerarValorConsistente(){
    // 7.1: Define uma chave de teste para calcular o hash
    char* chave_teste = "123.456.789-00";

    // 7.2: Calcula o valor de hash para a chave de teste usando a função hashFuncPM
    int hash1 = hashFuncPM(chave_teste);
    int hash2 = hashFuncPM(chave_teste);

    // 7.3: Verifica se os valores de hash gerados para a mesma chave são iguais, 
    // garantindo que a função de hash seja consistente
    TEST_ASSERT_EQUAL_INT(hash1, hash2);
    printf("\n");
}

// 8: Testa se a função buscarPessoa retorna o registro correto para uma chave existente 
// e retorna 0 para uma chave inexistente
void test_BuscarPessoa_DeveRetornarRegistroCorreto(){
    // 8.1: Cria uma tabela hash para ser usada no teste de busca de pessoa
    tabela_teste = criarHashPM("hashPM_teste.hf");
    
    // 8.2: Insere um registro para garantir que haja um registro existente para a busca, 
    // e verifica se a função retornar 0 (sucesso) para a inserção do registro
    int res_ins = inserirRegPM(tabela_teste, "123.456.789-00", "João", "Silva", "M", "1990-01-01");
    TEST_ASSERT_EQUAL_INT(0, res_ins);

    // 8.3: Cria uma estrutura de pessoa para armazenar o resultado da busca e tenta buscar a pessoa usando a função buscarPessoa,
    // passando a chave do registro inserido e o ponteiro para a estrutura de pessoa onde os dados devem ser armazenados, 
    // e verifica se a função retornar 1 (registro encontrado) para a busca da pessoa existente
    Pessoas* p_busca = criarPessoa(); 
    int res_busca = buscarPessoa(tabela_teste, "123.456.789-00", p_busca);    
    TEST_ASSERT_EQUAL_INT(1, res_busca);

    // 8.4: Verifica se os dados retornados pela função buscarPessoa correspondem aos dados do registro inserido, 
    // garantindo que a busca retorne o registro correto para uma chave existente
    TEST_ASSERT_EQUAL_STRING("123.456.789-00", getPessoaCpf(p_busca));
    TEST_ASSERT_EQUAL_STRING("João", getPessoaNome(p_busca));
    TEST_ASSERT_EQUAL_STRING("Silva", getPessoaSobrenome(p_busca));
    TEST_ASSERT_EQUAL_STRING("M", getPessoaSexo(p_busca));
    TEST_ASSERT_EQUAL_STRING("1990-01-01", getPessoaNasc(p_busca));

    // 8.5: Destrói a tabela hash e libera a memória alocada para a estrutura de pessoa criada durante o teste
    free(p_busca);
}

// 9: Testa se a função removerPessoa remove corretamente um registro existente e retorna 1
// e retorna 0 para uma chave inexistente
void test_RemoverPessoa_DeveRemoverRegistroCorreto(){
    // 9.1: Cria uma tabela hash para ser usada no teste de remoção de pessoa
    tabela_teste = criarHashPM("hashPM_teste.hf");
    
    // 9.2: Insere um registro para garantir que haja um registro existente para a remoção, 
    // e verifica se a função retornar 0 (sucesso) para a inserção do registro
    int res_ins = inserirRegPM(tabela_teste, "123.456.789-00", "João", "Silva", "M", "1990-01-01");
    TEST_ASSERT_EQUAL_INT(0, res_ins);

    // 9.3: Tenta remover a pessoa usando a função removerPessoa, passando a chave do registro inserido, 
    // e verifica se a função retornar 1 (registro encontrado e removido) para a remoção da pessoa existente
    int res_remocao = removerPessoa(tabela_teste, "123.456.789-00");
    TEST_ASSERT_EQUAL_INT(1, res_remocao);

    // 9.4: Tenta remover uma pessoa usando uma chave inexistente e verifica se a função retornar 0 (registro não encontrado) para a remoção de uma chave inexistente
    int res_remocao_inexistente = removerPessoa(tabela_teste, "987.654.321-00");
    TEST_ASSERT_EQUAL_INT(0, res_remocao_inexistente);
}
/*******************************************************************************************************/



// Cada teste é executado entre uma chamada de setUp e tearDown para garantir que cada teste tenha um ambiente limpo e controlado, 
// evitando interferências entre os testes e garantindo a confiabilidade dos resultados.
int main(void){
    // 1: Inicia o framework de teste Unity
    UNITY_BEGIN();

    // 2: Executa os testes da tabela hash
    printf("\n\n\n#============================= INICIO DOS TESTES DA TABELA HASH =============================#\n");
    
    printf("|----- Teste 01: Criar Tabela Hash - Deve Retornar um Ponteiro Valido (Nao NULL)------|\n\n");
    RUN_TEST(test_CriarHash_NaoDeveRetornarNull);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");
    
    printf("|----- Teste 02: Criar Pessoa - Deve Retornar um Ponteiro Valido (Nao NULL) ----------|\n");
    RUN_TEST(test_CriarPessoa_NaoDeveRetornarNull);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");
    
    printf("|-------------- Teste 03: Inserir Registro - Deve Retornar 0 (Sucesso) ---------------|\n\n");
    RUN_TEST(test_InserirRegPM_DeveRetornarZero);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|-------------- Teste 04: Dividir o bucket - Deve retornar 0 (Sucesso) ---------------|\n\n");
    RUN_TEST(test_DividirBucket_DeveRetornarZero);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|------------ Teste 05: Salvar Diretorio HFC - Deve retornar 0 (Sucesso) -------------|\n\n");
    RUN_TEST(test_SalvarDiretorioHFC_PM_DeveRetornarZero);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|----------- Teste 06: Carregar Diretorio HFC - Deve retornar 0 (Sucesso) ------------|\n\n");
    RUN_TEST(test_CarregarDiretorioPM_DeveRetornarPonteiroValido);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|----------- Teste 07: Hash Function - Deve gerar valor consistente ------------------|\n\n");
    RUN_TEST(test_HashFuncPM_DeveGerarValorConsistente);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|----------- Teste 08: Buscar Pessoa - Deve gerar valor consistente ------------------|\n\n");
    RUN_TEST(test_BuscarPessoa_DeveRetornarRegistroCorreto);
    printf("|-------------------------------------------------------------------------------------|\n\n\n\n");

    printf("|----------- Teste 09: Remover Pessoa - Deve remover registro correto ----------------|\n\n");
    RUN_TEST(test_RemoverPessoa_DeveRemoverRegistroCorreto);
    printf("|-------------------------------------------------------------------------------------|\n\n");

    printf("#=============================-- FIM DOS TESTES DA TABELA HASH =============================-#\n\n\n");

    // 3: Retorna o resultado dos testes
    return UNITY_END();
}