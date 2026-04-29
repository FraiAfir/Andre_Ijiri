#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"
#include "svg.h"
#include "params.h"
#include "hashTable.h"

/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoGeo(Param* param, char* caminhoGeo){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeGeo    = getNomeGeo(param);

    // Imprime o nome do arquivo .geo original para depuração
    printf("Arquivo .geo fornecido: \t\t\t%s\n", nomeGeo);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .geo
    strcpy(caminhoGeo, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoGeo, nomeGeo);    // Concatena o nome do arquivo .geo ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .geo: \t\t%s\n", caminhoGeo);

    return 0;
}

int readFileGeo(FILE* arquivoGeo, TabelaHash* dir, Quadras* q, Param* param){
    // 1: Cria o arquivo .svg para escrita do conteúdo do arquivo .geo
    FILE* arqSvg = criarSvg("geo.svg");
    if(arqSvg == NULL) {
        fprintf(stderr, "ERRO: Não foi possível criar o SVG.\n");
        return -1;
    }

    // 2: Buffers para armazenar os dados lidos de cada linha do arquivo .geo
    char linha[256];
    char comando[5];

    // 3: Variáveis temporárias para armazenar os dados lidos de cada linha do arquivo .geo
    double sw = 1.0;            // Largura da borda padrão
    char cfill[256] = "white";  // Cor de preenchimento padrão
    char cstrk[256] = "black";  // Cor da borda padrão
    
    // 4: Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoGeo) != NULL){

        // 4.1: Lê o comando da linha para determinar o tipo de dado a ser processado
        sscanf(linha, "%s", comando);

        // 4.2: Processa o comando lido do arquivo .geo
        if(strcmp(comando, "cq") == 0){
            // Comando cq - Define a largura da borda(sw), a cor de preenchimento (cfill) e a cor da borda (cstrk) para os próximos comandos q
            // '*' é usado para ignorar a primeira string (cq) e ler os próximos valores
            // Exemplo de linha: comando sw  cfill cstrk
            // Exemplo de linha:   cq    2.0  red   blue
            sscanf(linha, "%*s %lf %s %s", &sw, cfill, cstrk);
        }else if(strcmp(comando, "q") == 0){
            // Comando q - Insere uma quadra (retângulo e CEP)
            // '*' é usado para ignorar a primeira string (q) e ler os próximos valores
            // Exemplo de linha: comando cep  x    y    w    h
            // Exemplo de linha:    q    Q1 10.0 20.0 30.0 40.0
            char cep[5];
            double x, y, w, h;
            sscanf(linha, "%*s %s %lf %lf %lf %lf", cep, &x, &y, &w, &h);

            // 4.2.1: Insere a quadra na estrutura de dados (Tabela Hash e Quadras)
            inserirReg(dir, cep, x, y, w, h, sw, cfill, cstrk);

            // 4.2.2: Desenha a quadra no arquivo SVG
            desenharFormaSvg(arqSvg, "r", x, y, w, h, sw, cstrk, cfill);
        }
    }

    // 5: Fecha o arquivo .svg após a geração do conteúdo
    if(fecharSvg(arqSvg) != 0){
        fprintf(stderr, "ERRO: Fechar o arquivo .svg após a geração do conteúdo.\n");
        return -1;
    } printf("Arquivo .svg fechado com sucesso após a geração do conteúdo.\n");
    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarGeo(Param* param, TabelaHash* h, Quadras* q){
    // Inicializa o buffer para o caminho completo do arquivo .geo
    char caminhoGeo[512];   

    // 1: Monta o caminho completo do arquivo .geo
    if(montarCaminhoGeo(param, caminhoGeo) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .geo.\n");
        return -1;
    }

    printf("Iniciando o processamento do arquivo .geo\n\n");

    // 2: Abre o arquivo .geo para leitura
    FILE* arquivoGeo = fopen(caminhoGeo, "r");
    if(arquivoGeo == NULL){
        fprintf(stderr, "ERRO: Nao foi possivel abrir o arquivo .geo: %s\n", caminhoGeo);
        return -1;
    }

    // 3: Verifica se a estrutura de dados para armazenar os dados do arquivo .geo foi criada com sucesso
    if(q == NULL){
        fprintf(stderr, "ERRO: Criar a estrutura de dados para armazenar os dados do arquivo .geo.\n");
        fclose(arquivoGeo);
        return -1;
    } fprintf(stdout, "Estrutura de dados existente para armazenar os dados do arquivo .geo\n\n");

    // 4: Lê e processa os dados do arquivo .geo
    if(readFileGeo(arquivoGeo, h, q, param) != 0){
        fprintf(stderr, "ERRO: Leitura do arquivo .geo.\n");
        fclose(arquivoGeo);
        return -1;
    }

    // 5: Fecha o arquivo .geo após o processamento
    fclose(arquivoGeo);
    printf("\nArquivo .geo processado com sucesso!\n");
    return 0;
}
/*###############################################################################################*/