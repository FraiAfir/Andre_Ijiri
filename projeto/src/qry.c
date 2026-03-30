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

        // 2: Extrai apenas o comando (string) para sabermos o que fazer
        char* comando = strtok(linha, " \n\r");

        // 3: Processa o comando lido do arquivo .qry
        // 3.1: Verifica se o comando lido é válido
        if(comando != NULL){

            // 3.1.1: Comando rq - Remove quadra cujo cep é cep. Moradores da quadra passam a ser sem-tetos.
            if(strcmp(comando, "rq") == 0){
                // Extrai o parâmetro do comando rq (CEP da quadra a ser removida)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando rq foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);
                    
                    // Remove a quadra do sistema, de acordo com as instruções do arquivo .qry
                    if(removerQuadra(cpf) != 0){
                        fprintf(stderr, "ERRO: Falha ao remover a quadra do sistema.\n");
                        return -1;
                    }

                }else{
                    printf("ERRO: Parametro do comando rq lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 3.1.2: Comando pq - Calcula o número de moradores da quadra (por face e total).
            if(strcmp(comando, "pq") == 0){
                // Extrai o parâmetro do comando pq (CEP da quadra a ser removida)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando pq foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);
                    
                    // Calcula o número de moradores da quadra, de acordo com as instruções do arquivo .qry
                    if(calcMoradores(cpf) != 0){
                        fprintf(stderr, "ERRO: Falha ao calcular o numero de moradores da quadra.\n");
                        return -1;
                    }

                }else{
                    printf("ERRO: Parametro do comando pq lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 3.1.3: Comando censo - Produz várias estatísticas sobre habitantes de Bitnópolis.
            if(strcmp(comando, "censo") == 0){
                printf(" => COMANDO LIDO [%s]\n", comando);

                // Produz várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry
                if(produzirCenso() != 0){
                    fprintf(stderr, "ERRO: Falha ao produzir as estatisticas do censo.\n");
                    return -1;
                }
            }

            // 3.1.4: Comando h? - Dados sobre habitante identificado por cpf.
            if(strcmp(comando, "h?") == 0){
                // Extrai o parâmetro do comando h? (CPF do habitante)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando h? foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    // Obtém os dados sobre habitante identificado por cpf, de acordo com as instruções do arquivo .qry
                    if(obterDadosHabitante(cpf) != 0){
                        fprintf(stderr, "ERRO: Falha ao obter os dados do habitante identificado pelo CPF.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando h? lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 3.1.5: Comando nasc - Pessoa nasce.
            if(strcmp(comando, "nasc") == 0){
                // Extrai os parâmetros do comando nasc (CPF, nome, sobrenome, sexo e data de nascimento do habitante)
                char* cpf        = strtok(NULL, " \n\r");
                char* nome       = strtok(NULL, " \n\r");
                char* sobrenome  = strtok(NULL, " \n\r");
                char* bufferSexo = strtok(NULL, " \n\r");
                char* nasc       = strtok(NULL, " \n\r");

                // Converte o buffer de sexo para um caractere, se não for NULL
                char sexo = '\0';
                if(bufferSexo != NULL) sexo = bufferSexo[0];

                // Verifica se os parâmetros do comando nasc foram lidos corretamente
                if(cpf != NULL && nome != NULL && sobrenome != NULL && sexo != '\0' && nasc != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s | Nome = %s | Sobrenome = %s | Sexo = %c | Nascimento = %s\n", 
                        comando, cpf, nome, sobrenome, sexo, nasc);
                    
                    
                    // Registra o nascimento do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarNascimento(cpf, nome, sobrenome, sexo, nasc) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o nascimento do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando nasc lidos do arquivo .qry sao invalidos (NULL).\n");
                    return -1;
                }
            }

            // 3.1.6: Comando rip - Pessoa falece.
            if(strcmp(comando, "rip") == 0){
                // Extrai o parâmetro do comando rip (CPF do habitante)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando rip foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    // Registra o falecimento do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarObito(cpf) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o óbito do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando rip lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 3.1.7: Comando mud - Morador identificado por cpf muda-se para novo endereço.
            if(strcmp(comando, "mud") == 0){
                // Extrai os parâmetros do comando mud (CPF do habitante, CEP, face, número e complemento do novo endereço)
                char* cpf        = strtok(NULL, " \n\r");
                char* cep        = strtok(NULL, " \n\r");
                char* bufferFace = strtok(NULL, " \n\r");
                int   num        = atoi(strtok(NULL, " \n\r"));
                char* cmpl       = strtok(NULL, " \n\r");

                // Converte o buffer de face para um caractere, se não for NULL
                char face = '\0';
                if(bufferFace != NULL) face = bufferFace[0];

                // Verifica se os parâmetros do comando mud foram lidos corretamente
                if(cpf != NULL && cep != NULL && face != '\0' && num != 0 && cmpl != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s | CEP = %s | Face = %c | Num = %d | Compl = %s\n", 
                        comando, cpf, cep, face, num, cmpl);
                    
                    // Registra a mudança de endereço do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarMudanca(cpf, cep, face, num, cmpl) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar a mudanca de endereco do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando mud lidos do arquivo .qry sao invalidos (NULL ou num == 0).\n");
                    return -1;
                }
            }

            // 3.1.8: Comando dspj - Morador identificado por cpf  é despejado.
            if(strcmp(comando, "dspj") == 0){
                // Extrai o parâmetro do comando dspj (CPF do habitante)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando dspj foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    // Registra o despejo do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarDespejo(cpf) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o despejo do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando dspj lido do arquivo .qry eh NULL.\n");
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
int removerQuadra(char* cpf){
    // *Implementar a lógica para remover uma quadra do sistema, de acordo com as instruções do arquivo .qry*

    printf("Funcao removerQuadra() chamada\n\n");
    return 0;
}
int calcMoradores(char* cpf){
    // *Implementar a lógica para calcular o número de moradores de uma quadra, de acordo com as instruções do arquivo .qry*

    printf("Funcao calcMoradores() chamada\n\n");
    return 0;
}
int produzirCenso(){
    // *Implementar a lógica para produzir várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry*

    printf("Funcao produzirCenso() chamada\n\n");
    return 0;
}
int obterDadosHabitante(char* cpf){
    // *Implementar a lógica para obter os dados sobre habitante identificado por cpf, de acordo com as instruções do arquivo .qry*

    printf("Funcao obterDadosHabitante() chamada\n\n");
    return 0;
}
int registrarNascimento(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc){
    // *Implementar a lógica para o nascimento de um habitante, de acordo com as instruções do arquivo .qry*

    printf("Funcao registrarNascimento() chamada\n\n");
    return 0;
}
int registrarObito(char* cpf){
    // *Implementar a lógica para o falecimento de um habitante, de acordo com as instruções do arquivo .qry*

    printf("Funcao registrarObito() chamada\n\n");
    return 0;
}
int registrarMudanca(char* cpf, char* cep, char face, int num, char* cmpl){
    // *Implementar a lógica para registrar a mudança de endereço de um habitante, de acordo com as instruções do arquivo .qry*

    printf("Funcao registrarMudanca() chamada\n\n");
    return 0;
}
int registrarDespejo(char* cpf){
    // *Implementar a lógica para registrar o despejo de um habitante, de acordo com as instruções do arquivo .qry*

    printf("Funcao registrarDespejo() chamada\n\n");
    return 0;
}
/*###############################################################################################*/