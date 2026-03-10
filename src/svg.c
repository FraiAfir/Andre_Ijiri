#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"
#include "params.h"

/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoSvg(Param* param, char* caminhoSvg){
    char* dirSaida = getDirSaidaCompleto(param); // Diretório de saída completo (Ex: ./saida/)
    char* geoSvg   = getNomeGeo         (param); // a.geo

    // 1: Copia o diretório de saída completo para o caminho do arquivo .svg
    strcpy(caminhoSvg, dirSaida); 

    // 2: Busca o nome base do arquivo (Sem o ".geo")
    char nomeBase[256];                      // Buffer para armazenar o nome base do arquivo (Sem a extensão .geo)
    strcpy(nomeBase, geoSvg);                // Copia o nome do arquivo .geo para o buffer nomeBase
    char* extensao = strrchr(nomeBase, '.'); // Busca a última ocorrência do caractere '.' para encontrar a extensão do arquivo
    if(extensao != NULL)                    // Verifica se a extensão foi encontrada
        *extensao = '\0';                    // Remove a extensão .geo

    // 3: Concatena o nome base com a extensão .svg e o diretório de saída para formar o caminho completo do arquivo .svg
    strcat(caminhoSvg, nomeBase); // Concatena o diretório de saída com o nome base do arquivo
    strcat(caminhoSvg, ".svg");   // Adiciona a extensão .svg

    printf("Nome do arquivo svg completo: %s\n", caminhoSvg);
    
    return 0;
}
int montarCaminhoSvgQry(Param* param, char* caminhoSvgQry){
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

    // Exibe o caminho completo do arquivo .svg gerado (Com o qry)
    printf("Nome do arquivo svg completo: %s\n", caminhoSvgQry);

    return 0;
}
int montarCaminhoTxt(Param* param, char* caminhoTxt){
    char* dirSaida    = getDirSaidaCompleto(param); // Diretório de saída completo (Ex: ./saida/)
    char* geoSvg      = getNomeGeo         (param); // a.geo
    char* qrySvg      = getNomeQry         (param); // b.qry

    // 1: Copia o diretório de saída completo para o caminho do arquivo .txt
    strcpy(caminhoTxt, dirSaida);

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
    // Adiciona a extensão .txt e o diretório de saída para formar o caminho completo do arquivo .txt
    strcat(caminhoTxt, nomeBaseGeo); // Concatena o diretório de saída com o nome base do arquivo .geo
    strcat(caminhoTxt, "-");         // Adiciona um '-' entre os nomes base dos arquivos .geo e .qry
    strcat(caminhoTxt, nomeBaseQry); // Concatena o nome base do arquivo .qry
    strcat(caminhoTxt, ".txt");      // Adiciona a extensão .txt

    // Exibe o caminho completo do arquivo .txt gerado (Com o qry)
    printf("Nome do arquivo txt completo: %s\n", caminhoTxt);
    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarSvg(Param* param){
    char caminhoSvg[512];    // Inicializa o buffer para o caminho completo do arquivo .svg
    char caminhoSvgQry[512]; // Inicializa o buffer para o caminho completo do arquivo .svg (Com o qry)
    char caminhoSvgTxt[512]; // Inicializa o buffer para o caminho completo do arquivo .txt (Com o qry)

    // Monta o caminho completo do arquivo .svg (Sem o qry)
    if(montarCaminhoSvg(param, caminhoSvg) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .svg (Sem o qry)\n");
        return -1;
    }

    // Monta o caminho completo do arquivo .svg (Com o qry) (Verificar se é nulo)
    if(getNomeQry(param) != NULL){
        if(montarCaminhoSvgQry(param, caminhoSvgQry) != 0){
            fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .svg (Com o qry)\n");
            return -1;
        }
    }else printf("Arquivo .qry nao fornecido. Pulando a montagem do caminho completo do arquivo .svg com o qry.\n");
    
    // Monta o caminho completo do arquivo .txt (Verificar se o QRY é nulo)
    if(getNomeQry(param) != NULL){
        if(montarCaminhoTxt(param, caminhoSvgTxt) != 0){
            fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .txt\n");
            return -1;
        }
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

void desenharFormaSvg(FILE* arqSvg, char* tipoForma, 
    double x, double y, double w, double h, double sw,
    char* cstrk, char* cfill){

    // Verifica se o ponteiro para o arquivo .svg é nulo antes de tentar escrever nele
    if(arqSvg == NULL) return;
    
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
        fprintf(arqSvg, "<text style=\"font-size:5px;fill:%s\" font-size=\"5\" y=\"%lf\" x=\"%lf\"> 1 />\n",
            cfill, y, x);
    }

    else fprintf(stderr, "ERRO: Tipo de forma desconhecido: %s\n", tipoForma);
}

void fecharSvg(FILE* arqSvg){
    // Verifica se o ponteiro para o arquivo .svg é nulo antes de tentar escrever nele
    if(arqSvg == NULL) return;

    // Escreve a tag de fechamento do elemento <svg> no arquivo .svg
    fprintf(arqSvg, "</svg>\n");

    // Fecha o arquivo .svg
    fclose(arqSvg);
}
/*###############################################################################################*/
