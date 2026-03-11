#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"
#include "txt.h"
#include "params.h"

/*                                       FUNÇÕES AUXILIARES                                      */
char* montarCaminhoSvg   (Param* param, char* caminhoSvg){
    char* dirSaida = getDirSaidaCompleto(param); // Diretório de saída completo (Ex: ./saida/)
    char* geoSvg   = getNomeGeo         (param); // a.geo

    // 1: Copia o diretório de saída completo para o caminho do arquivo .svg
    strcpy(caminhoSvg, dirSaida); 

    // 2: Busca o nome base do arquivo (Sem o ".geo")
    char nomeBase[256];                      // Buffer para armazenar o nome base do arquivo (Sem a extensão .geo)
    strcpy(nomeBase, geoSvg);                // Copia o nome do arquivo .geo para o buffer nomeBase
    char* extensao = strrchr(nomeBase, '.'); // Busca a última ocorrência do caractere '.' para encontrar a extensão do arquivo
    if(extensao != NULL)                     // Verifica se a extensão foi encontrada
        *extensao = '\0';                    // Remove a extensão .geo

    // 3: Concatena o nome base com a extensão .svg e o diretório de saída para formar o caminho completo do arquivo .svg
    strcat(caminhoSvg, nomeBase); // Concatena o diretório de saída com o nome base do arquivo
    strcat(caminhoSvg, ".svg");   // Adiciona a extensão .svg

    // Imprime o nome do arquivo .geo original para depuração
    printf("Arquivo .geo fornecido: \t\t\t%s\n", geoSvg);

    // Exibe o caminho completo do arquivo .svg gerado (Sem o qry)
    printf("Nome do arquivo svg completo: \t\t\t%s\n", caminhoSvg);
    
    return caminhoSvg;
}
char* montarCaminhoSvgQry(Param* param, char* caminhoSvgQry){
    char* dirSaida   = getDirSaidaCompleto(param); // Diretório de saída completo (Ex: ./saida/)
    char* geoSvg     = getNomeGeo         (param); // a.geo
    char* qrySvg     = getNomeQry         (param); // b.qry

    // 1: Copia o diretório de saída completo para o caminho do arquivo .svg
    strcpy(caminhoSvgQry, dirSaida); 

    // 2: Busca o nome base dos arquivos .geo e .qry (Sem as extensões)
    char nomeBaseGeo[256];                          // Buffer para armazenar o nome base do arquivo .geo (Sem a extensão .geo)
    char nomeBaseQry[256];                          // Buffer para armazenar o nome base do arquivo .qry (Sem a extensão .qry)
    strcpy(nomeBaseGeo, geoSvg);                    // Copia o nome do arquivo .geo para o buffer nomeBaseGeo
    strcpy(nomeBaseQry, qrySvg);                    // Copia o nome do arquivo .qry para o buffer nomeBaseQry
    char* extensaoGeo = strrchr(nomeBaseGeo, '.');  // Busca a última ocorrência do caractere '.' para encontrar a extensão do arquivo .geo
    char* extensaoQry = strrchr(nomeBaseQry, '.');  // Busca a última ocorrência do caractere '.' para encontrar a extensão do arquivo .qry
    if(extensaoGeo != NULL)                        // Verifica se a extensão do arquivo .geo foi encontrada
        *extensaoGeo = '\0';                        // Remove a extensão .geo
    
    if(extensaoQry != NULL)                        // Verifica se a extensão do arquivo .qry foi encontrada
        *extensaoQry = '\0';                        // Remove a extensão .qry

    // 3: Concatena os nomes base dos arquivos .geo e .qry com um '-' entre eles
    // Adiciona a extensão .svg e o diretório de saída para formar o caminho completo do arquivo .svg
    strcat(caminhoSvgQry, nomeBaseGeo); // Concatena o diretório de saída com o nome base do arquivo .geo
    strcat(caminhoSvgQry, "-");         // Adiciona um '-' entre os nomes base dos arquivos .geo e .qry
    strcat(caminhoSvgQry, nomeBaseQry); // Concatena o nome base do arquivo .qry
    strcat(caminhoSvgQry, ".svg");      // Adiciona a extensão .svg

    // Imprime o nome do arquivo .qry original para depuração
    printf("Arquivo .qry fornecido: \t\t\t%s\n", qrySvg);

    // Exibe o caminho completo do arquivo .svg gerado (Com o qry)
    printf("Nome do arquivo svg (.geo-.qry) completo: \t%s\n", caminhoSvgQry);

    return caminhoSvgQry;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarSvg(Param* param){
    char caminhoSvg   [512]; // Inicializa o buffer para o caminho completo do arquivo .svg
    char caminhoSvgQry[512]; // Inicializa o buffer para o caminho completo do arquivo .svg (Com o qry)
    char caminhoTxt   [512]; // Inicializa o buffer para o caminho completo do arquivo .txt (Com o qry)

    /* 1 - Processamento do arquivo geo.svg */ 
    // Monta o caminho completo do arquivo .svg (Sem o qry)
    montarCaminhoSvg(param, caminhoSvg);
    // Acessa a estrutura de dados com as informações do arquivo .geo e gera o conteúdo do arquivo .svg
    FILE* arqSvg = criarSvg(caminhoSvg);
    if(arqSvg == NULL){
        fprintf(stderr, "ERRO: Criar o arquivo .svg.\n");
        return -1;
    }
    /* Função para acessar a estrutura de dados e gerar o conteúdo do arquivo .svg a ser implementada */
    while(0){ // Loop de exemplo para desenhar formas no arquivo .svg (Substituir pela lógica real de acesso à estrutura de dados)
        // Exemplo de chamada da função para desenhar um retângulo no arquivo .svg
        if(desenharFormaSvg(arqSvg, "r", 10, 10, 100, 50, 2, "black", "red") != 0){
            fprintf(stderr, "ERRO: Desenhar forma no arquivo .svg.\n");
            return -1;
        }
    }
    // Fecha o arquivo .svg após a geração do conteúdo
    if(fecharSvg(arqSvg) != 0){
        fprintf(stderr, "ERRO: Fechar o arquivo .svg apos a geracao do conteudo.\n");
        return -1;
    } printf("Arquivo .svg fechado com sucesso apos a geracao do conteudo.\n\n");

    /* 2 - Processamento do arquivo geo-qry.svg */ 
    // Verifica se o arquivo .qry foi fornecido (Não é nulo) antes de montar o caminho completo do arquivo .svg (Com o qry)
    if(getNomeQry(param) != NULL){
        // Monta o caminho completo do arquivo .svg (Com o qry)
        montarCaminhoSvgQry(param, caminhoSvgQry);
        // Acessa a estrutura de dados com as informações do arquivo .geo e gera o conteúdo do arquivo .svg
        FILE* arqSvgQry = criarSvg(caminhoSvgQry);
        if(arqSvgQry == NULL){
            fprintf(stderr, "ERRO: Criar o arquivo .svg (Com o qry).\n");
            return -1;
        }
        /* Função para acessar a estrutura de dados e gerar o conteúdo do arquivo .svg a ser implementada */
        while(0){ // Loop de exemplo para desenhar formas no arquivo .svg (Substituir pela lógica real de acesso à estrutura de dados)
            // Exemplo de chamada da função para desenhar um retângulo no arquivo .svg
            if(desenharFormaSvg(arqSvgQry, "r", 10, 10, 100, 50, 2, "black", "red") != 0){
                fprintf(stderr, "ERRO: Desenhar forma no arquivo .svg.\n");
                return -1;
            }
        }

        // Fecha o arquivo .svg após a geração do conteúdo
        if(fecharSvg(arqSvgQry) != 0){
            fprintf(stderr, "ERRO: Fechar o arquivo .svg apos a geracao do conteudo.\n");
            return -1;
        } printf("Arquivo .svg fechado com sucesso apos a geracao do conteudo.\n\n");
    }else printf("Arquivo .qry nao fornecido. Pulando a montagem do caminho completo do arquivo .svg com o qry.\n");
    
    /* 3 - Processamento do arquivo geo-qry.txt */
    // Monta o caminho completo do arquivo .txt (Verificar se o QRY é nulo)
    if(getNomeQry(param) != NULL){
        montarCaminhoTxt(param, caminhoTxt);
        // Acessa a estrutura de dados com as informações do arquivo .geo e gera o conteúdo do arquivo .txt
        FILE* arqTxt= criarTxt(caminhoTxt);
        if(arqTxt == NULL){
            fprintf(stderr, "ERRO: Criar o arquivo .txt.\n");
            return -1;
        }
        /* Função para acessar a estrutura de dados e gerar o conteúdo do arquivo .txt a ser implementada */
        while(0){ // Loop de exemplo para desenhar formas no arquivo .txt (Substituir pela lógica real de acesso à estrutura de dados)
            // Exemplo de chamada da função para desenhar um retângulo no arquivo .txt
            if(escreverComandoTxt(arqTxt, "Retângulo") != 0){
                fprintf(stderr, "ERRO: Escrever comando no arquivo .txt.\n");
                return -1;
            }
        }

        // Fecha o arquivo .txt após a geração do conteúdo
        if(fecharTxt(arqTxt) != 0){
            fprintf(stderr, "ERRO: Fechar o arquivo .txt apos a geracao do conteudo.\n");
            return -1;
        } printf("Arquivo .txt fechado com sucesso apos a geracao do conteudo.\n\n");
    }else printf("Arquivo .qry nao fornecido. Pulando a montagem do caminho completo do arquivo .txt.\n");

    return 0;
}
FILE* criarSvg(char* caminhoSvg){
    // Abre o arquivo .svg para escrita
    FILE* arqSvg = fopen(caminhoSvg, "w"); 

    // Verifica se o arquivo foi aberto com sucesso
    if(arqSvg == NULL){ 
        fprintf(stderr, "ERRO: Nao foi possivel criar o arquivo .svg\n");
        return NULL;
    }

    // Escreve a declaração do elemento <svg> no arquivo .svg
    fprintf(arqSvg, "<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");

    // Retorna o ponteiro para o arquivo .svg criado
    return arqSvg;
}
int desenharFormaSvg(FILE* arqSvg, char* tipoForma, 
    double x, double y, double w, double h, double sw,
    char* cstrk, char* cfill){

    // Verifica se o ponteiro para o arquivo .svg é nulo antes de tentar escrever nele
    if(arqSvg == NULL) return -1;
    
    // Verifica o tipo da forma a ser desenhada e escreve a tag correspondente no arquivo .svg
    if(strcmp(tipoForma, "r") == 0){      // Retângulo
        fprintf(arqSvg, "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%lf\" fill=\"%s\" />\n",
            x, y, w, h, cstrk, sw, cfill);
    }

    else if(strcmp(tipoForma, "c") == 0){ // Círculo
        // Calcula o raio do círculo com base na largura (w)
        double raio = w / 2.0;

        fprintf(arqSvg, "\t<circle fill=\"%s\" fill-opacity=\"0.5\" stroke=\"%s\" r=\"%lf\" cy=\"%lf\" cx=\"%lf\" />\n", 
            cfill, cstrk, raio, y + raio, x + raio);
    }

    else if(strcmp(tipoForma, "l") == 0){ // Texto
        fprintf(arqSvg, "<text style=\"font-size:5px;fill:%s\" font-size=\"5\" y=\"%lf\" x=\"%lf\"> Text here />\n",
            cfill, y, x);
    }

    else fprintf(stderr, "ERRO: Tipo de forma desconhecido: %s\n", tipoForma);
    
    return 0;
}
int fecharSvg(FILE* arqSvg){
    // Verifica se o ponteiro para o arquivo .svg é nulo antes de tentar escrever nele
    if(arqSvg == NULL) return -1;

    // Escreve a tag de fechamento do elemento <svg> no arquivo .svg
    fprintf(arqSvg, "</svg>\n");

    // Fecha o arquivo .svg
    fclose(arqSvg);

    return 0;
}
/*###############################################################################################*/