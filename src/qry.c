#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "params.h"
// incluir a estrutura de dados a ser implementada para armazenar os dados do arquivo .qry

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct qry{
    char*  comando; // Comando do arquivo .qry
    
    // @o? - Armazena a posição geográfica do endereço cep/face/num
    char*  cep;     // Identificador único associado à quadra
    char   face;    // Face associada à quadra (Norte, Sul, Leste, Oeste)
    int    num;     // Número associado à quadra

    // catac - Remover as quadras contidas na região delimitada pelo retângulo x, y, w, h
    // Arestas incidentes em vértices dentro do retângulo especificado devem ser removidas
    double x, y;    // Coordenadas associadas ao comando
    double w, h;    // Largura e altura associadas ao comando

    // pnt - Modifica as cores de preenchimento e de borda da quadra
    // identificada por cep para cfill e cstrk
    char*  cfill;   // Cor de preenchimento da quadra
    char*  cstrk;   // Cor da borda da quadra

    // blq - Bloqueia o fluxo num determinado sentido (ou seja, desabilita a aresta) 
    // em todas arestas que se originarem num vértice dentro da região retangular especificada
    // Os possíveis valores de sentido são: 
    // ns (bloqueia arestas no sentido norte=>sul); 
    // sn (bloqueia sentido sul=>norte);
    // lo (bloqueia leste=>oeste); 
    // ol (bloqueia oeste=>leste)
    char* sentido;  // Sentido do bloqueio

    // rbl - Desbloqueia as arestas que foram bloqueadas por um comando blq anterior de mesmo nome
    char* nome;     // Nome do bloqueio

    // b - Faz o percurso em largura no grafo, a partir do nó mais próximo da coordenada (x,y), 
    // multiplicando a velocidade média pelo fator especificado, nas arestas de árvore.
    double fator;   // Fator de multiplicação da velocidade média

    // p? - Determina o melhor trajeto entre a origem (@o) e o destino especificado pelo endereço 
    // cep/face/num. O percurso na representação pictórica deve indicar o trajeto mais curto na cor
    // cmc e o trajeto mais rápido com a cor cmr
    char* cmc;      // Cor do trajeto mais curto
    char* cmr;      // Cor do trajeto mais rápido

}Qry;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoQry(Param* param, char* caminhoQry){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomeQry    = getNomeQry           (param);

    // Imprime o nome do arquivo .qry original para depuração
    printf("Arquivo .qry fornecido: \t\t\t%s\n", nomeQry);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .qry
    strcpy(caminhoQry, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoQry, nomeQry);    // Concatena o nome do arquivo .qry ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .qry: \t\t%s\n", caminhoQry);

    return 0;
}
int readFileQry(FILE* arquivoQry){
    char linha[256];

    // Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoQry) != NULL){

        // 1: Limpeza da linha
        linha[strcspn(linha, "\n")] = '\0'; // Remove o ENTER do final da linha, se existir
        linha[strcspn(linha, "\r")] = '\0'; // Previne bugs de quebra de linha do Windows
        if(strlen(linha) == 0) continue;    // Ignora linhas em branco

        printf("Lendo linha do .qry: %s\n", linha);

        // 2: Extrai apenas o comando para sabermos o que fazer
        char comando[20];
        sscanf(linha, "%s", comando);

        // 3: Processa o comando lido do arquivo .qry
        if(strcmp(comando, "o?") == 0){
            char buffer[20];
            char cep[50];
            char face;
            int  num;

        // Exemplo de leitura dos parâmetros do comando @o? (Ajustar o formato de leitura conforme a estrutura real do comando no arquivo .qry)
            sscanf(linha, "%s %s %c %d", buffer, cep, &face, &num);

            printf(" => COMANDO LIDO [@o?]: CEP = %s, Face = %c, Num = %d\n\n", cep, face, num);

            /* Chamar função que busca esse CEP na Estrutura de Dados e imprime no TXT */
        }
        // else if(strcmp(comando, "???") == 0){}
        // else if(strcmp(comando, "???") == 0){}
        // else if(strcmp(comando, "???") == 0){}

        // Lógica para processar cada linha do arquivo .qry e realizar as alterações necessárias na estrutura de dados do programa
        /* Função para processar cada linha do arquivo .qry a ser implementada */

    }

    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarQry(Param* param){
    char caminhoQry[512];   // Inicializa o buffer para o caminho completo do arquivo .qry

    // 1: Monta o caminho completo do arquivo .qry
    if(montarCaminhoQry(param, caminhoQry) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .qry.\n");
        return -1;
    }
    printf("Iniciando o processamento do arquivo .qry\n\n");

    // 2: Abre o arquivo .qry para leitura
    FILE* arquivoQry = fopen(caminhoQry, "r");
    if(arquivoQry == NULL){
        fprintf(stderr, "ERRO: Nao foi possivel abrir o arquivo .qry: %s\n", caminhoQry);
        return -1;
    }

    // 3: Lê e processa os dados do arquivo .qry
    if(readFileQry(arquivoQry) != 0){ 
        fprintf(stderr, "ERRO: Leitura do arquivo .qry.\n");
        fclose(arquivoQry);
        return -1;
    }

    // 4: Fecha o arquivo .qry após o processamento
    fclose(arquivoQry);
    printf("\nArquivo .qry processado com sucesso!\n");
    return 0;
}
Qry* criarQry(char* comando, char* cep, char* face, int num, double x, double y, double w, double h, 
    char* cfill, char* cstrk, char* sentido, char* nome, double fator, char* cmc, char* cmr){
    Qry* qry = (Qry*)malloc(sizeof(Qry));
    if(qry == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Qry\n");
        return NULL;
    }

    qry->comando = NULL;
    qry->cep     = NULL;
    qry->face    = '\0';
    qry->num     = 0;
    qry->x       = 0.0, qry->y = 0.0;
    qry->w       = 0.0, qry->h = 0.0;
    qry->cfill   = NULL;
    qry->cstrk   = NULL;
    qry->sentido = NULL;
    qry->nome    = NULL;
    qry->fator   = 0.0;
    qry->cmc     = NULL;
    qry->cmr     = NULL;

    return qry;
}
int freeQry(Qry* qry){
    if(qry == NULL){
        fprintf(stderr, "ERRO: Ponteiro para Qry NULL\n");
        return -1;
    }

    free(qry->comando);
    free(qry->cep);
    free(qry->cfill);
    free(qry->cstrk);
    free(qry->sentido);
    free(qry->nome);
    free(qry->cmc);
    free(qry->cmr);

    free(qry);

    return 0;
}
/*###############################################################################################*/