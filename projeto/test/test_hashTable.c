#include <stdio.h>
#include <string.h>

#include "unity.h"
#include "hashTable.h"

// Variável global para ser usada nos testes
HashTable* tabela_teste = NULL;

// 1: O que acontece ANTES de cada teste
void setUp(void){
    // 1.1: Cria uma tabela nova antes de cada função "test_" rodar.
    // Usamos HASH_SIZE (se ainda estiver no .h) ou qualquer número inteiro como 1009
    tabela_teste = criarTabela(HASH_SIZE); 
}

// 2: O que acontece DEPOIS de cada teste
void tearDown(void){
    // Verifica se a tabela foi criada antes de tentar liberar a memória
    if (tabela_teste != NULL){
        // 2.1: Libera a memória alocada para a tabela hash após cada teste
        freeHash(tabela_teste);
        // 2.2: Define o ponteiro da tabela como NULL para evitar acessos acidentais após a liberação
        tabela_teste = NULL;
    }
}

// 3. OS TESTES EM SI
// 3.1: Testa se a função criarTabela realmente cria uma tabela e salva o tamanho dinâmico
void test_CriarTabela_NaoDeveRetornarNull(void){
    TEST_ASSERT_NOT_NULL(tabela_teste);                         // Verifica se a tabela foi criada com sucesso (não é NULL)
    TEST_ASSERT_EQUAL_INT(HASH_SIZE, getTamanho(tabela_teste)); // Verifica se o tamanho da tabela é o esperado
}

// 3.2: Testa a proteção contra tamanhos inválidos
void test_CriarTabela_TamanhoInvalido_DeveRetornarNull(void){
    HashTable* tabela_invalida_zero     = criarTabela(0);  // Tamanho zero é inválido    
    HashTable* tabela_invalida_negativo = criarTabela(-5); // Tamanho negativo é inválido
    
    TEST_ASSERT_NULL(tabela_invalida_zero);     // Deve retornar NULL para tamanho zero
    TEST_ASSERT_NULL(tabela_invalida_negativo); // Deve retornar NULL para tamanho negativo
}

// 3.3: Testa se a função inserirReg insere um novo registro corretamente e retorna 0
void test_InserirReg_NovaChave_DeveRetornarZero(void){
    int resultado = inserirReg(tabela_teste, "cep_123", "Rua A"); // Inserção de um novo registro
    TEST_ASSERT_EQUAL_INT(0, resultado);                          // Verifica se a chave foi inserida corretamente
}

// 3.4: Testa se a função existeKey retorna TRUE para uma chave que foi inserida
void test_ExisteKey_ChaveInserida_DeveRetornarTrue(void){
    inserirReg(tabela_teste, "cep_123", "Rua A");     // Insere um registro para garantir que a chave exista na tabela hash
    bool existe = existeKey(tabela_teste, "cep_123"); // Verifica se a função existeKey retorna TRUE para a chave inserida
    TEST_ASSERT_TRUE(existe);                         // A chave deve existir, então a função deve retornar TRUE
}

// 3.5: Testa se a função existeKey retorna FALSE para uma chave que não foi inserida
void test_ExisteKey_ChaveInexistente_DeveRetornarFalse(void){
    bool existe = existeKey(tabela_teste, "chave_fantasma"); // Verifica se a função existeKey retorna FALSE para uma chave que não foi inserida
    TEST_ASSERT_FALSE(existe);                               // A chave não foi inserida, então a função deve retornar FALSE
}

// 3.6: Testa se a função getRegistro retorna o valor correto para uma chave existente
void test_GetRegistro_ChaveExistente_DeveRetornarValorCorreto(void){
    inserirReg(tabela_teste, "cep_456", "Rua B");        // Insere um registro para garantir que a chave exista na tabela hash
    HashNode* no = getRegistro(tabela_teste, "cep_456"); // Obtém o nó associado à chave inserida

    TEST_ASSERT_NOT_NULL(no);                                             // Verifica se o nó retornado não é NULL (a chave existe)
    TEST_ASSERT_EQUAL_STRING("Rua B", getValue(tabela_teste, "cep_456")); // Verifica se o valor associado à chave é o esperado
}

// 3.7: Testa se a função removerKey remove um registro existente e retorna 0
void test_RemoverKey_ChaveExistente_DeveRetornarZeroENaoExistirMais(void){
    inserirReg(tabela_teste, "cep_789", "Rua C");                // Insere um registro para garantir que a chave exista na tabela hash

    int resultado_remocao = removerKey(tabela_teste, "cep_789"); // Remove o registro associado à chave inserida
    TEST_ASSERT_EQUAL_INT(0, resultado_remocao);                 // Verifica se a função removerKey retornou 0 (sucesso)
    
    TEST_ASSERT_FALSE(existeKey(tabela_teste, "cep_789"));       // Verifica se a chave foi realmente removida (deve retornar FALSE)
}

// 3.8: Sobrescrever valor (Update)
void test_InserirReg_ChaveJaExistente_DeveAtualizarValor(void){
    inserirReg(tabela_teste, "cep_duplicado", "Valor Antigo"); // Insere um registro com uma chave específica
    
    // Tenta inserir um novo value para a mesma chave (deve atualizar o value existente)
    int resultado = inserirReg(tabela_teste, "cep_duplicado", "Valor Novo"); 
    // Verifica se a função retornar 0 (sucesso) mesmo quando a chave já existe, indicando que o valor foi atualizado
    TEST_ASSERT_EQUAL_INT(0, resultado);

    // Obtém o nó associado à chave para verificar se o valor foi atualizado
    HashNode* no = getRegistro(tabela_teste, "cep_duplicado");
    // Verifica se o nó retornado não é NULL (a chave existe)
    TEST_ASSERT_NOT_NULL(no);
    // Verifica se o valor associado à chave foi atualizado para "Valor Novo"
    TEST_ASSERT_EQUAL_STRING("Valor Novo", getValue(tabela_teste, "cep_duplicado"));
}

// 4. A Função Principal do Teste
int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_CriarTabela_NaoDeveRetornarNull);
    RUN_TEST(test_CriarTabela_TamanhoInvalido_DeveRetornarNull);
    RUN_TEST(test_InserirReg_NovaChave_DeveRetornarZero);
    RUN_TEST(test_ExisteKey_ChaveInserida_DeveRetornarTrue);
    RUN_TEST(test_ExisteKey_ChaveInexistente_DeveRetornarFalse);
    RUN_TEST(test_GetRegistro_ChaveExistente_DeveRetornarValorCorreto);
    RUN_TEST(test_RemoverKey_ChaveExistente_DeveRetornarZeroENaoExistirMais);
    RUN_TEST(test_InserirReg_ChaveJaExistente_DeveAtualizarValor);

    return UNITY_END();
}