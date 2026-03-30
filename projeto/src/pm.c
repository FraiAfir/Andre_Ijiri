#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pm.h"
#include "params.h"

/*                           ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                           */
typedef struct pm{
    // 1: Comando do arquivo .pm
    char* comando;
    // p - Insere um habitante
    // m - Informa que um dado habitante (cpf) mora num dado endereço (cep,face,num,compl)

    // 2: Parâmetros associados aos comandos
    char* cpf;
    // 2.1: Parâmetros do comando p (Insere um habitante)
    char* nome;
    char* sobrenome;
    char  sexo;
    char* nasc;

    // 2.2: Parâmetros do comando m (Informa que um dado habitante mora num dado endereço)
    char* cep;
    char  face;
    int   num;
    char* compl;
}PM;
/*###############################################################################################*/



/*                                       FUNÇÕES AUXILIARES                                      */
int montarCaminhoPM(Param* param, char* caminhoPM){
    char* dirEntrada = getDirEntradaCompleto(param);
    char* nomePM    = getNomePM           (param);

    // Imprime o nome do arquivo .pm original para depuração
    printf("Arquivo .pm fornecido: \t\t\t%s\n", nomePM);
    
    // Concatena o diretório de entrada completo com o nome do arquivo .pm
    strcpy(caminhoPM, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoPM, nomePM);     // Concatena o nome do arquivo .pm ao caminho completo do diretório de entrada
    printf("Caminho completo do arquivo .pm: \t\t%s\n", caminhoPM);

    return 0;
}
int readFilePM(FILE* arquivoPM){
    char linha[256];

    // Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoPM) != NULL){

        // 1: Limpeza da linha
        linha[strcspn(linha, "\n")] = '\0'; // Remove o ENTER do final da linha, se existir
        linha[strcspn(linha, "\r")] = '\0'; // Previne bugs de quebra de linha do Windows
        if(strlen(linha) == 0) continue;    // Ignora linhas em branco

        // printf("Lendo linha do .pm: %s\n", linha);

        // 2: Extrai apenas o comando (string) para sabermos o que fazer
        char* comando = strtok(linha, " \n\r");

        // 3: Processa o comando lido do arquivo .qry
        // 3.1: Verifica se o comando lido é válido
        if(comando != NULL){

            // 3.1.1: Comando p - Insere um habitante
            if(strcmp(comando, "p") == 0){
                // Extrai os parâmetros do comando p (CPF, nome, sobrenome, sexo e data de nascimento do habitante a ser inserido)
                char* cpf        = strtok(NULL, " \n\r");
                char* nome       = strtok(NULL, " \n\r");
                char* sobrenome  = strtok(NULL, " \n\r");
                char* bufferSexo = strtok(NULL, " \n\r");
                char* nasc       = strtok(NULL, " \n\r");

                // Converte o buffer de sexo para um caractere, se não for NULL
                char sexo = '\0';
                if(bufferSexo != NULL) sexo = bufferSexo[0];

                // Verifica se os parâmetros do comando p foram lidos corretamente
                if(cpf != NULL && nome != NULL && sobrenome != NULL && sexo != '\0' && nasc != NULL){
                    // printf(" => COMANDO LIDO [%s]: CPF = %s, Nome = %s %s, Sexo = %c, Nasc = %s\n", 
                    //     comando, cpf, nome, sobrenome, sexo, nasc);

                    // Registra o nascimento do habitante, de acordo com as instruções do arquivo .pm
                    if(inserirHabitante(cpf, nome, sobrenome, sexo, nasc) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o nascimento do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando p lidos do arquivo .pm sao invalidos (NULL ou sexo == '\\0').\n");
                    return -1;
                }
            }

            // 3.1.2: Comando m - Informa que um dado habitante (cpf) mora num dado endereço (cep,face,num,compl)
            if(strcmp(comando, "m") == 0){
                // Extrai os parâmetros do comando m (CPF do habitante, CEP, face, número e complemento do endereço onde o habitante mora)
                char* cpf        = strtok(NULL, " \n\r");
                char* cep        = strtok(NULL, " \n\r");
                char* bufferFace = strtok(NULL, " \n\r");
                int   num        = atoi(strtok(NULL, " \n\r"));
                char* cmpl       = strtok(NULL, " \n\r");

                // Converte o buffer de face para um caractere, se não for NULL
                char face = '\0';
                if(bufferFace != NULL) face = bufferFace[0];

                // Verifica se os parâmetros do comando m foram lidos corretamente
                if(cpf != NULL && cep != NULL && face != '\0' && num > 0 && cmpl != NULL){
                    // printf(" => COMANDO LIDO [%s]: CPF = %s, CEP = %s, Face = %c, Número = %d, Complemento = %s\n", 
                    //     comando, cpf, cep, face, num, cmpl);

                    // Informa que um dado habitante (cpf) mora num dado endereço (cep,face,num,compl), de acordo com as instruções do arquivo .pm
                    if(informarHabitanteMoraEndereco(cpf, cep, face, num, cmpl) != 0){
                        fprintf(stderr, "ERRO: Falha ao informar o endereco do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando m lidos do arquivo .pm sao invalidos (NULL ou valores invalidos).\n");
                    return -1;
                }
            }
            
        }else{
            printf("ERRO: Comando lido do arquivo .qry é NULL.\n");
            return -1;
        }
    }

    return 0;
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarPM(Param* param){
    // Inicializa o buffer para o caminho completo do arquivo .pm
    char caminhoPM[512];

    // 1: Monta o caminho completo do arquivo .pm
    if(montarCaminhoPM(param, caminhoPM) != 0){
        fprintf(stderr, "ERRO: Montar o caminho completo do arquivo .pm.\n");
        return -1;
    }

    printf("Iniciando o processamento do arquivo .pm\n\n");

    // 2: Abre o arquivo .pm para leitura
    FILE* arquivoPM = fopen(caminhoPM, "r");
    if(arquivoPM == NULL){
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo .pm: %s\n", caminhoPM);
        return -1;
    }

    // 3: Lê e processa os dados do arquivo .pm
    if(readFilePM(arquivoPM) != 0){  
        fprintf(stderr, "ERRO: Leitura do arquivo .pm.\n");
        fclose(arquivoPM);
        return -1;
    }

    // 4: Fecha o arquivo .pm após o processamento
    fclose(arquivoPM);
    printf("Arquivo .pm processado com sucesso!\n");
    return 0;
}
PM* criarPM(){
    PM* pm = (PM*)malloc(sizeof(PM));
    if(pm == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto PM\n");
        return NULL;
    }

    pm->comando = NULL;
    pm->cpf     = NULL;

    pm->nome      = NULL;
    pm->sobrenome = NULL;
    pm->sexo      = '\0';
    pm->nasc      = NULL;

    pm->cep     = NULL;
    pm->face    = '\0';
    pm->num     = 0;
    pm->compl   = NULL;

    return pm;
}
int freePM(PM* pm){
    printf("\nLiberando a memoria alocada para a instancia de PM...\n");

    if(pm == NULL){
        fprintf(stderr, "ERRO: Ponteiro para PM NULL\n");
        return -1;
    }

    free(pm->comando);

    free(pm->cpf);
    free(pm->nome);
    free(pm->sobrenome);
    free(pm->nasc);

    free(pm->cep);
    free(pm->compl);

    free(pm);

    printf("\nMemoria alocada para a instancia de PM liberada com sucesso!\n\n");
    return 0;
}
int inserirHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc){
    // printf("Inserindo habitante no sistema...\n");
    // printf("CPF: %s | Nome: %s %s | Sexo: %c | Nascimento: %s\n", cpf, nome, sobrenome, sexo, nasc);
    // *Implementar a lógica para inserir um habitante no sistema, de acordo com as instruções do arquivo .pm*
    return 0;
}
int informarHabitanteMoraEndereco(char* cpf, char* cep, char face, int num, char* cmpl){
    // printf("Informando que o habitante mora no endereco...\n");
    // printf("CPF: %s | CEP: %s | Face: %c | Número: %d | Complemento: %s\n", cpf, cep, face, num, cmpl);
    // *Implementar a lógica para informar que um dado habitante (cpf) mora num dado endereço (cep,face,num,compl), de acordo com as instruções do arquivo .pm*
    return 0;
}
/*###############################################################################################*/
