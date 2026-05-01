#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"
#include "params.h"

/*                                       FUNÇÕES PRINCIPAIS                                      */
FILE* criarSvg(char* caminhoSvg, double largura, double altura){
    // Abre o arquivo .svg para escrita
    FILE* arqSvg = fopen(caminhoSvg, "w"); 

    // Verifica se o arquivo foi aberto com sucesso
    if(arqSvg == NULL){ 
        fprintf(stderr, "ERRO: Nao foi possivel criar o arquivo .svg\n");
        return NULL;
    }

    // Escreve a declaração do elemento <svg> no arquivo .svg
    fprintf(arqSvg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%lf\" height=\"%lf\">\n", largura+100.0, altura+100.0);

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