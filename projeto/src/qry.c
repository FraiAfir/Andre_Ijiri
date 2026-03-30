#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "params.h"
// incluir a estrutura de dados a ser implementada para armazenar os dados do arquivo .qry

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct qry{
    // 1: Comando do arquivo .qry
    char*  comando;
    // 1.1: rq - Remove  quadra cujo cep é cep. Moradores da quadra passam a ser sem-tetos.
    //  TXT: Informar cpf e nome dos moradores.
    //  SVG: colocar um pequeno X vermelho no local da âncora da quadra removida.
    //  Parâmetros: 
    //      - cep;

    // 1.2: pq - Calcula o número de moradores da quadra (por face e total).
    //  SVG: colocar número de moradores de cada face (próximo ao limite da face) e, no centro da quadra, o número total de moradores da quadra.
    //  Parâmetros:
    //      - cep;

    // 1.3: censo - Produz várias estatísticas sobre habitantes de Bitnópolis.
    //  TXT: Reportar estatísticas: 
    //      - Número total de habitantes; Número total de moradores; Número total de sem-tetos
    //      - Proporção moradores/habitantes
    //      - Número de homens; Número de mulheres
    //      - % de habitantes homens; % habitantes mulheres
    //      - % de moradores homens; % de moradores mulheres
    //      - % sem-tetos homens; % sem - tetos mulheres
    //  Parâmetros: Nenhum

    // 1.4: h? - Dados sobre habitante identificado por cpf.
    //  TXT: reporta todos os dados sobre a respectiva pessoa. Se for morador, reportar também o endereço.
    //  Parâmetros:
    //      - cep;

    // 1.5: nasc - Pessoa nasce.
    //  Parâmetros:
    //      - cpf; 
    //      - nome;
    //      - sobrenome; 
    //      - sexo; 
    //      - nasc;

    // 1.6: rip - Pessoa falece.
    //  TXT: Reportar dados da pessoa falecida. Se for morador, reportar também o endereço.
    //  SVG: Colocar uma pequena cruz vermelha no local do endereço (se morador).
    //  Parâmetros:
    //      - cpf;

    // 1.7: mud - Morador identificado por cpf muda-se para novo endereço.
    //  SVG: marcar o endereço de destino com um pequeno quadrado vermelho no local de destino. 
    //  Colocar o cpf dentro do quadrado (usar fonte minúscula)
    //  Parâmetros:
    //      - cpf;
    //      - cep;
    //      - face;
    //      - num;
    //      - compl;

    // 1.8: dspj - Morador identificado por cpf  é despejado.
    //  TXT: reportar dados do habitante e endereço onde ocorreu o despejo.
    //  SVG: colocar um pequeno círculo preto no local do despejo.
    //  Parâmetros:
    //      - cpf;

    // 2: Parâmetros associados aos comandos
    char* cep;
    char* cpf;
    char* nome;
    char* sobrenome;
    char* sexo;
    char* nasc;
    char  face;
    int   num;
    char* cmpl;
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
    // Inicializa o buffer para o caminho completo do arquivo .qry
    char caminhoQry[512];

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
Qry* criarQry(){
    Qry* qry = (Qry*)malloc(sizeof(Qry));
    if(qry == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Qry\n");
        return NULL;
    }

    qry->comando   = NULL;
    qry->cep       = NULL;
    qry->cpf       = NULL;
    qry->nome      = NULL;
    qry->sobrenome = NULL;
    qry->sexo      = NULL;
    qry->nasc      = NULL;
    qry->face      = '\0';
    qry->num       = 0;
    qry->cmpl      = NULL;

    return qry;
}
int freeQry(Qry* qry){
    printf("\nLiberando a memoria alocada para a instancia de Qry...\n");

    if(qry == NULL){
        fprintf(stderr, "ERRO: Ponteiro para Qry NULL\n");
        return -1;
    }

    free(qry->comando);
    free(qry->cep);
    free(qry->cpf);
    free(qry->nome);
    free(qry->sobrenome);
    free(qry->sexo);
    free(qry->nasc);
    free(qry->cmpl);

    free(qry);

    printf("Memoria alocada para a instancia de Qry liberada com sucesso!\n");
    return 0;
}
/*###############################################################################################*/