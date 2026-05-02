#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
#include "svg.h"
#include "params.h"
#include "hashPM.h"
#include "hashTable.h"
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
    // 1: Montar o caminho completo do arquivo .qry a partir do diretório de entrada e do nome do arquivo .qry fornecidos como argumentos na linha de comando, e armazenados na estrutura de parâmetros
    char* dirEntrada = getDirEntradaCompleto(param); // Obtém o diretório de entrada completo a partir da estrutura de parâmetros
    char* nomeQry    = getNomeQry           (param); // Obtém o nome do arquivo .qry a partir da estrutura de parâmetros

    // 2: Imprime o nome do arquivo .qry original para depuração
    printf("Arquivo .qry fornecido: \t\t\t%s\n", nomeQry);
    
    // 3: Concatena o diretório de entrada completo com o nome do arquivo .qry
    strcpy(caminhoQry, dirEntrada); // Copia o diretório de entrada completo
    strcat(caminhoQry, nomeQry);    // Concatena o nome do arquivo .qry ao caminho completo do diretório de entrada

    // 4: Imprime o caminho completo do arquivo .qry para depuração
    printf("Caminho completo do arquivo .qry: \t\t%s\n", caminhoQry);

    return 0;
}

int readFileQry(FILE* arquivoQry, hashPM* htp, TabelaHash* h, Param* param){
    // 1: Prepara os arquivos de saída (SVG e TXT) para o processamento do arquivo .qry
    // 1.1: Busca diretório de saída e nomes originais
    char* dirSaida = getDirSaidaCompleto(param);    // Ex: "./saida/"
    char nomeGeoOriginal[256];                      // Ex: "a.geo" (sem o caminho, apenas o nome do arquivo)
    char nomeQryOriginal[256];                      // Ex: "b.qry" (sem o caminho, apenas o nome do arquivo)
    
    // 1.2: Faz cópias locais dos nomes originais para manipulação (remoção de extensão, concatenação, etc.)
    strcpy(nomeGeoOriginal, getNomeGeo(param)); 
    strcpy(nomeQryOriginal, getNomeQry(param));

    // 1.3: Remove extensões das cópias locais
    char* p;
    p = strrchr(nomeGeoOriginal, '.');
    if(p) *p = '\0';
    p = strrchr(nomeQryOriginal, '.');
    if(p) *p = '\0';

    // 1.4: Monta Caminho do SVG BASE (O que já existe)
    // dirSaida + nomeGeo + .svg
    char caminhoSvgBase[512];
    strcpy(caminhoSvgBase, dirSaida);
    strcat(caminhoSvgBase, nomeGeoOriginal);
    strcat(caminhoSvgBase, ".svg");

    // 1.5: Montar Caminho do SVG QRY (O clone novo)
    // dirSaida + nomeGeo + "-" + nomeQry + .svg
    char caminhoSvgQry[512];
    strcpy(caminhoSvgQry, dirSaida);
    strcat(caminhoSvgQry, nomeGeoOriginal);
    strcat(caminhoSvgQry, "-");
    strcat(caminhoSvgQry, nomeQryOriginal);
    strcat(caminhoSvgQry, ".svg");

    // 1.6: Montar Caminho do TXT (O relatório)
    // dirSaida + nomeGeo + "-" + nomeQry + .txt
    char caminhoTxtQry[512];
    strcpy(caminhoTxtQry, dirSaida);
    strcat(caminhoTxtQry, nomeGeoOriginal);
    strcat(caminhoTxtQry, "-");
    strcat(caminhoTxtQry, nomeQryOriginal);
    strcat(caminhoTxtQry, ".txt");

    // 1.7: Clona o SVG BASE para o SVG QRY
    FILE* qrySVG = clonarSvgBase(caminhoSvgBase, caminhoSvgQry);    
    if(qrySVG == NULL){
        printf("ERRO: Nao foi possivel abrir/clonar o SVG: %s\n", caminhoSvgBase);
        return -1;
    }

    // 1.8: Abre o arquivo de saída .txt para escrita dos resultados das consultas do arquivo .qry
    FILE* qryTXT = fopen(caminhoTxtQry, "w");
    if(qryTXT == NULL){
        printf("ERRO: Nao foi possivel criar o arquivo de texto para os resultados do .qry: %s\n", caminhoTxtQry);
        return -1;
    }
    
    // Inicializa o buffer para leitura das linhas do arquivo .qry e abre o arquivo de saída .txt para escrita dos resultados das consultas do arquivo .qry
    char linha[256];    
    // 2: Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoQry) != NULL){

        // 2.1: Limpeza da linha
        linha[strcspn(linha, "\n")] = '\0'; // Remove o ENTER do final da linha, se existir
        linha[strcspn(linha, "\r")] = '\0'; // Previne bugs de quebra de linha do Windows
        if(strlen(linha) == 0) continue;    // Ignora linhas em branco

        // printf("\nLendo linha do .qry: %s\n", linha);

        // Precedido de um [*] para facilitar a identificação das linhas do arquivo .qry no meio dos outros prints de depuração
        fprintf(qryTXT, "\n[*] %s\n", linha);

        // 2.2: Extrai apenas o comando (string) para sabermos o que fazer
        char* comando = strtok(linha, " \n\r");

        // 2.3: Processa o comando lido do arquivo .qry
        if(comando != NULL){

            // 2.3.1: Comando rq - Remove quadra cujo cep é cep. Moradores da quadra passam a ser sem-tetos.
            if(strcmp(comando, "rq") == 0){
                // Extrai o parâmetro do comando rq (CEP da quadra a ser removida)
                char* cep = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando rq foi lido corretamente
                if(cep != NULL){
                    // printf(" => COMANDO LIDO [%s]: CEP = %s\n", comando, cep);

                    // Remove a quadra do sistema, de acordo com as instruções do arquivo .qry
                    if(removerQuadraQRY(htp, cep, qryTXT, h, qrySVG) != 0){
                        fprintf(stderr, "ERRO: Falha ao remover a quadra do sistema.\n");
                        return -1;
                    }

                }else{
                    printf("ERRO: Parametro do comando rq lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 2.3.2: Comando pq - Calcula o número de moradores da quadra (por face e total).
            if(strcmp(comando, "pq") == 0){
                // Extrai o parâmetro do comando pq (CEP da quadra a ser removida)
                char* cep = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando pq foi lido corretamente
                if(cep != NULL){
                    // printf(" => COMANDO LIDO [%s]: CEP = %s\n", comando, cep);
                    
                    // Calcula o número de moradores da quadra, de acordo com as instruções do arquivo .qry
                    if(calcMoradores(htp, h, cep, qryTXT, qrySVG) != 0){
                        fprintf(stderr, "ERRO: Falha ao calcular o numero de moradores da quadra.\n");
                        return -1;
                    }

                }else{
                    printf("ERRO: Parametro do comando pq lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 2.3.3: Comando censo - Produz várias estatísticas sobre habitantes de Bitnópolis.
            if(strcmp(comando, "censo") == 0){
                // printf(" => COMANDO LIDO [%s]\n", comando);

                // Produz várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry
                if(produzirCenso(htp, qryTXT) != 0){
                    fprintf(stderr, "ERRO: Falha ao produzir as estatisticas do censo.\n");
                    return -1;
                }
            }

            // 2.3.4: Comando h? - Dados sobre habitante identificado por cpf.
            if(strcmp(comando, "h?") == 0){
                // Extrai o parâmetro do comando h? (CPF do habitante)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando h? foi lido corretamente
                if(cpf != NULL){
                    // printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    if(obterDadosHabitante(htp, cpf, qryTXT) != 0){
                        fprintf(stderr, "ERRO: Falha ao obter os dados do habitante identificado pelo CPF.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando \"h?\" lido do arquivo .qry \"é\" NULL.\n");
                    return -1;
                }
            }

            // 2.3.5: Comando nasc - Pessoa nasce.
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
                    // printf(" => COMANDO LIDO [%s]: CPF = %s | Nome = %s | Sobrenome = %s | Sexo = %c | Nascimento = %s\n", 
                    //     comando, cpf, nome, sobrenome, sexo, nasc);
                    
                    // Registra o nascimento do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarNascimento(htp, cpf, nome, sobrenome, sexo, nasc, qryTXT) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o nascimento do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando nasc lidos do arquivo .qry sao invalidos (NULL).\n");
                    return -1;
                }
            }

            // 2.3.6: Comando rip - Pessoa falece.
            if(strcmp(comando, "rip") == 0){
                // Extrai o parâmetro do comando rip (CPF do habitante)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando rip foi lido corretamente
                if(cpf != NULL){
                    // printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    // Registra o falecimento do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarObito(htp, h, cpf, qryTXT, qrySVG) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o óbito do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando rip lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 2.3.7: Comando mud - Morador identificado por cpf muda-se para novo endereço.
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
                    // printf(" => COMANDO LIDO [%s]: CPF = %s | CEP = %s | Face = %c | Num = %d | Compl = %s\n", 
                    //     comando, cpf, cep, face, num, cmpl);
                    
                    // Registra a mudança de endereço do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarMudanca(htp, h, cpf, cep, face, num, cmpl, qryTXT, qrySVG) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar a mudanca de endereco do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando mud lidos do arquivo .qry sao invalidos (NULL ou num == 0).\n");
                    return -1;
                }
            }

            // 2.3.8: Comando dspj - Morador identificado por cpf  é despejado.
            if(strcmp(comando, "dspj") == 0){
                // Extrai os parâmetros do comando dspj (CPF do habitante, CEP, face, número e complemento do endereço de despejo)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando dspj foi lido corretamente
                if(cpf != NULL){
                    // printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    // Registra o despejo do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarDespejo(htp, h, cpf, qryTXT, qrySVG) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o despejo do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando dspj lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

        }

        // 2.4: Se o comando lido do arquivo .qry for NULL, imprime uma mensagem de erro e continua para a próxima linha
        else{
            printf("ERRO: Comando lido do arquivo .qry \"é\" NULL.\n");
            return -1;
        }
    }

    // 3: Fecha os arquivos de saída após o processamento do arquivo .qry
    fclose(qryTXT);
    if(fecharSvg(qrySVG) != 0){
        fprintf(stderr, "ERRO: Fechar o arquivo .svg apos a geracao do conteudo.\n");
        return -1;
    }printf("Arquivo .svg fechado com sucesso apos a geracao do conteudo.\n");
    return 0;
}

FILE* clonarSvgBase(char* caminhoSvgBase, char* caminhoSvgQry){
    // 1: Abre o arquivo .svg base para leitura e o arquivo .svg do .qry para escrita (será criado a partir do clone do .svg base)
    FILE* arqBase = fopen(caminhoSvgBase, "r"); // Abre o arquivo .svg base para leitura
    FILE* arqQry = fopen(caminhoSvgQry, "w");   // Abre o arquivo .svg do .qry para escrita (será criado a partir do clone do .svg base)
    if(arqBase == NULL || arqQry == NULL){
        printf("ERRO: Nao foi possivel clonar o SVG base.\n");
        return NULL;
    }

    // 2: Lê o arquivo .svg base linha por linha e escreve no arquivo .svg do .qry, até encontrar a tag de fechamento </svg>
    char linha[1024];
    while(fgets(linha, sizeof(linha), arqBase)){
        // Se encontrar a tag de fechamento, NÃO copia ela. 
        // Vamos deixar o arquivo "aberto" para novos desenhos.
        if(strstr(linha, "</svg>") != NULL) continue;
        
        fputs(linha, arqQry);
    }

    // 3: Fecha o arquivo .svg base, mas deixa o arquivo .svg do .qry aberto para receber os desenhos do arquivo .qry
    fclose(arqBase);
    
    // 4: Retorna o ponteiro do novo arquivo pronto para receber os desenhos do .qry
    return arqQry; 
}

void calcularCoordenadaEndereco(Quadras* q, char face, int num, double* out_x, double* out_y){
    // 1: Obtém as coordenadas (x, y) e dimensões (w, h) da quadra para calcular a posição do endereço com base na face e número
    double x = getQuadraX(q);
    double y = getQuadraY(q);
    double w = getQuadraW(q);
    double h = getQuadraH(q);

    // 2: Calcula as coordenadas (x, y) do endereço com base na face e número, usando as coordenadas e dimensões da quadra
    if      (face == 'N') { *out_x = (x + num); *out_y = (y);       }
    else if (face == 'S') { *out_x = (x + num); *out_y = (y + h);   }
    else if (face == 'L') { *out_x = (x + w)  ; *out_y = (y + num); }
    else if (face == 'O') { *out_x = (x)      ; *out_y = (y + num); }
    else                  { *out_x = (x + w/2); *out_y = (y + h/2); }
    /**
     * Calcula as coordenadas (x, y) do endereço com base na face e número, usando as coordenadas e dimensões da quadra:
     *  - Face 'N' (Norte): O endereço está na parte superior da quadra, então x varia com o número e y é constante (y da quadra)
     *  - Face 'S' (Sul): O endereço está na parte inferior da quadra, então x varia com o número e y é constante (y + altura da quadra)
     *  - Face 'L' (Leste): O endereço está na parte direita da quadra, então x é constante (x + largura da quadra) e y varia com o número
     *  - Face 'O' (Oeste): O endereço está na parte esquerda da quadra, então x é constante (x da quadra) e y varia com o número
     *  - Se a face for inválida, podemos definir um comportamento padrão (por exemplo, colocar o endereço no centro da quadra)
     */
}
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
int processarQry(Param* param, hashPM* htp, TabelaHash* h){
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
    if(readFileQry(arquivoQry, htp, h, param) != 0){ 
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
    // 1: Aloca memória para o objeto Qry
    Qry* qry = (Qry*)malloc(sizeof(Qry));
    if(qry == NULL){
        fprintf(stderr, "ERRO: Falha na alocacao de memoria para o objeto Qry\n");
        return NULL;
    }

    // 2: Inicializa os campos do objeto Qry com valores padrão (NULL para strings, '\0' para char, 0 para int)
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

    // 3: Retorna o ponteiro para o objeto Qry criado
    return qry;
}

int freeQry(Qry* qry){
    // 1: Verifica se o ponteiro do objeto Qry é NULL antes de tentar liberar a memória
    if(qry == NULL) return 0;

    // 2: Libera a memória alocada para as strings dentro do objeto Qry, se não forem NULL
    free(qry->comando);
    free(qry->cep);
    free(qry->cpf);
    free(qry->nome);
    free(qry->sobrenome);
    free(qry->sexo);
    free(qry->nasc);
    free(qry->cmpl);

    // 3: Libera a memória alocada para o objeto Qry em si
    free(qry);
    return 0;
}

int removerQuadraQRY(hashPM* htp, char* cep, FILE* txt, TabelaHash* h, FILE* qrySVG){
    // 1: Variável local para receber os dados da quadra a ser removida
    Quadras* q = criarQuadra(); 

    // 2: Buscar a quadra em TabelaHash
    if(buscarQuadra(h, cep, q) == 1){
        // 2.1: SVG - Desenha um pequeno 'X' vermelho na ÂNCORA da quadra a ser removida
        // A âncora da quadra é definida como o ponto (x, y) da própria quadra, que é o canto superior esquerdo. 
        double x = getQuadraX(q);
        double y = getQuadraY(q);
        // O 'X' vermelho será desenhado com linhas diagonais cruzadas centradas nesse ponto.
        fprintf(qrySVG, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x - 5, y - 5, x + 5, y + 5);
        fprintf(qrySVG, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x - 5, y + 5, x + 5, y - 5);


        // 2.2: Despeja os moradores da quadra a ser removida, ou seja, remove os dados de moradia (CEP, face, num, compl) dos registros das pessoas que moram na quadra e salva os buckets atualizados no disco
        despejarMoradoresQuadra(htp, cep, txt);

        // 2.3: Remove a quadra do sistema, de acordo com as instruções do arquivo .qry
        fprintf(txt, "---------------- REMOCAO DE QUADRA ---------------\n");
        removerQuadra(h, cep);
        fprintf(txt, "[rq] Quadra %s removida com sucesso\n", cep);
        fprintf(txt, "------------------------------------------------\n\n");

        // 2.4: Libera a memória alocada para a variável local da quadra
        freeQuadra(q);
        return 0;
    }

    // 3: Se a quadra não for encontrada, reportar o erro no arquivo de saída .txt
    fprintf(txt, "[rq] ERRO: Quadra %s nao encontrada\n", cep);
    fprintf(txt, "------------------------------------------------\n\n");

    // 4: Libera a memória alocada para a variável local da quadra
    freeQuadra(q);
    return 0;
}

int calcMoradores(hashPM* htp, TabelaHash* htq, char* cep, FILE* txt, FILE* qrySVG){
    // 1: Variáveis locais para receber os resultados
    int total = 0, morN = 0, morS = 0, morL = 0, morO = 0;

    fprintf(txt, "--------- CALCULO DE MORADORES DA QUADRA ---------\n");

    // 2: Chama o banco de dados passando os endereços para ele preencher
    calcularMoradoresQuadra(htp, cep, &total, &morN, &morS, &morL, &morO);

    
    fprintf(txt, "[pq] Quadra %s: Total = %d (N = %d | S = %d | L = %d | O = %d)\n", 
                            cep, total, morN, morS, morL, morO);

    fprintf(txt, "--------------------------------------------------\n\n");

    // 3: SVG - Colocar número de moradores de cada face (próximo ao limite da face) e, 
    // no centro da quadra, o número total de moradores da quadra
    Quadras* q = criarQuadra();
    if (buscarQuadra(htq, cep, q) == 1) {
        double x = getQuadraX(q);
        double y = getQuadraY(q);
        double w = getQuadraW(q);
        double h = getQuadraH(q);

        // 3.1: Centro (Total)
        fprintf(qrySVG, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"15\" font-weight=\"bold\" fill=\"black\">%d</text>\n", x + w/2 - 5, y + h/2 + 5, total);
        
        // 3.2: Faces (Deslocados levemente para dentro da quadra)
        fprintf(qrySVG, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"10\" font-weight=\"bold\" fill=\"red\">%d</text>\n", x + w/2   , y + 12   , morN); // Face N
        fprintf(qrySVG, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"10\" font-weight=\"bold\" fill=\"red\">%d</text>\n", x + w/2   , y + h - 5, morS); // Face S
        fprintf(qrySVG, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"10\" font-weight=\"bold\" fill=\"red\">%d</text>\n", x + w - 15, y + h/2  , morL); // Face L
        fprintf(qrySVG, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"10\" font-weight=\"bold\" fill=\"red\">%d</text>\n", x + 5     , y + h/2  , morO); // Face O
    }

    // 4: Libera a memória alocada para a variável local da quadra
    freeQuadra(q);
    return 0;
}

int produzirCenso(hashPM* htp, FILE* txt) {
    // 1: Variáveis locais para receber os resultados
    int totHab, totMor, totSemTeto;
    int masc, fem, morMasc, morFem, semTetoMasc, semTetoFem;

    // 2: Chama o banco de dados passando os endereços para ele preencher
    calcularEstatisticas(htp, &totHab, &totMor, &totSemTeto, 
                           &masc, &fem, &morMasc, &morFem, 
                           &semTetoMasc, &semTetoFem);

    // 3: Imprime o relatório
    fprintf(txt, "--- DADOS ---\n");
    fprintf(txt, "Total de Habitantes: %d\n", totHab);
    fprintf(txt, "Total de Moradores: %d\n", totMor);
    fprintf(txt, "Total de Sem-Teto: %d\n", totSemTeto);
    
    // 4: Calcula e imprime as proporções, verificando se os denominadores são maiores que zero para evitar divisão por zero
    if(totHab > 0){
        fprintf(txt, "\n--- PROPORCOES ---\n");
        fprintf(txt, "Proporcao Moradores/Habitantes: %.2f%%\n", (totMor / (float)totHab) * 100);
        fprintf(txt, "Homens: %d (%.2f%%) | Mulheres: %d (%.2f%%)\n", masc, (masc / (float)totHab) * 100, fem, (fem / (float)totHab) * 100);
    }

    // 5: Calcula e imprime as proporções de moradores por sexo, verificando se os denominadores são maiores que zero para evitar divisão por zero
    if(totMor > 0){
        fprintf(txt, "\n--- MORADORES ---\n");
        fprintf(txt, "Moradores Homens: %d (%.2f%%) | Moradores Mulheres: %d (%.2f%%)\n", morMasc, (morMasc / (float)totMor) * 100, morFem, (morFem / (float)totMor) * 100);
    }

    // 6: Calcula e imprime as proporções de sem-teto por sexo, verificando se os denominadores são maiores que zero para evitar divisão por zero
    if(totSemTeto > 0){
        fprintf(txt, "\n--- SEM-TETOS ---\n");
        fprintf(txt, "Sem-teto Homens: %d (%.2f%%) | Sem-teto Mulheres: %d (%.2f%%)\n\n", semTetoMasc, (semTetoMasc / (float)totSemTeto) * 100, semTetoFem, (semTetoFem / (float)totSemTeto) * 100);
    }

    return 0;
}

int obterDadosHabitante(hashPM* htp, char* cpf, FILE* txt){
    // 1: Criar uma estrutura de Pessoas para armazenar os dados do habitante a ser buscado
    Pessoas* p = criarPessoa();
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha ao criar a estrutura de Pessoas para obter os dados do habitante.\n");
        return -1;
    }

    fprintf(txt, "-------------- CONSULTA DE HABITANTE -------------\n");
    
    // 2: Tenta buscar no disco
    if(buscarPessoa(htp, cpf, p) == 1){
        // 1.1: Encontrou - Imprime os dados do habitante no arquivo de saída .txt
        fprintf(txt, "[h?] Dados do Habitante %s:\n", cpf);
        fprintf(txt, "Nome: %s %s | Sexo: %s | Nasc: %s\n", 
            getPessoaNome(p), getPessoaSobrenome(p), getPessoaSexo(p), getPessoaNasc(p));
        
        // 1.2: Se for morador, reportar também o endereço. Se não for morador, reportar que é sem-teto
        if(strlen(getPessoaCep(p)) > 0) 
            fprintf(txt, "Endereco: CEP %s, Face %s, Num %s, Compl %s\n", 
                getPessoaCep(p), getPessoaFace(p), getPessoaNum(p), getPessoaCompl(p));
        
        // 1.3: Se a pessoa não tiver um endereço registrado, ou seja, se o campo de CEP for vazio, consideramos que ela é sem-teto
        else 
            fprintf(txt, "Situacao: Sem-teto\n");
    }

    // 3: Não encontrou - Imprime mensagem de erro no arquivo de saída .txt
    else fprintf(txt, "[h?] Habitante com CPF %s nao encontrado.\n", cpf);

    fprintf(txt, "--------------------------------------------------\n\n");

    // 4: Libera a estrutura de Pessoas utilizada para armazenar os dados do habitante durante a operação de consulta
    freePessoa(p);
    return 0;
}

int registrarNascimento(hashPM* htp, char* cpf, char* nome, char* sobrenome, char sexo, char* nasc, FILE* txt){
    // 1: Converte o char de sexo para uma string de 2 caracteres (1 para o sexo e 1 para o terminador nulo) 
    char strSexo[2] = {sexo, '\0'}; // sexo é char mas a struct e o inserirRegPM esperam string

    fprintf(txt, "-------------- REGISTRO DE NASCIMENTO ------------\n");

    // 2: Registra o nascimento do habitante, de acordo com as instruções do arquivo .qry
    if(inserirRegPM(htp, cpf, nome, sobrenome, strSexo, nasc) == 0)
        fprintf(txt, "[nasc] Habitante %s %s (CPF: %s) nasceu com sucesso.\n", nome, sobrenome, cpf);
    
    // 3: Se o registro de nascimento falhar (por exemplo, se já existir um habitante com o mesmo CPF), reportar o erro no arquivo de saída .txt
    else
        fprintf(txt, "[nasc] Falha ao registrar nascimento do CPF: %s\n", cpf);

    fprintf(txt, "--------------------------------------------------\n\n");
    return 0;
}

int registrarObito(hashPM* htp, TabelaHash* htq, char* cpf, FILE* txt, FILE* qrySVG){
    // 1: Criar uma estrutura de Pessoas para armazenar os dados do habitante a ser buscado
    Pessoas* p = criarPessoa();
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha ao criar a estrutura de Pessoas para obter os dados do habitante.\n");
        return -1;
    }

    fprintf(txt, "--------------- REGISTRO DE OBITO ----------------\n");
    
    // 2: Busca a pessoa
    if(buscarPessoa(htp, cpf, p) == 1){
        // 2.1: SVG
        Quadras* q = criarQuadra();
        if(buscarQuadra(htq, getPessoaCep(p), q) == 1){
            double px, py;
            calcularCoordenadaEndereco(q, getPessoaFace(p)[0], atoi(getPessoaNum(p)), &px, &py);
            
            // Coloca uma pequena cruz vermelha no local do endereço (se morador)
            // Duas linhas levemente deslocadas para formar um '+' e centralizadas na coordenada do endereço
            fprintf(qrySVG, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", px - 5, py, px + 5, py);
            fprintf(qrySVG, "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", px, py - 5, px, py + 5);
        }
        // 2.2: Libera a memória alocada para a variável local da quadra
        freeQuadra(q);

        // 2.3: Encontrou - Imprime os dados do habitante no arquivo de saída .txt
        fprintf(txt, "[rip] Dados do Habitante %s:\n", cpf);
        fprintf(txt, "Morador %s faleceu.\n", cpf);
        fprintf(txt, "Nome: %s %s | Sexo: %s | Nasc: %s\n", 
            getPessoaNome(p), getPessoaSobrenome(p), getPessoaSexo(p), getPessoaNasc(p));
        
        // 2.4: Se for morador, reportar também o endereço. Se não for morador, reportar que é sem-teto
        if(strlen(getPessoaCep(p)) > 0) 
            fprintf(txt, "Endereco: CEP %s, Face %s, Num %s, Compl %s\n", 
                getPessoaCep(p), getPessoaFace(p), getPessoaNum(p), getPessoaCompl(p));
        
        // 2.5: Se a pessoa não tiver um endereço registrado, ou seja, se o campo de CEP for vazio, consideramos que ela é sem-teto
        else 
            fprintf(txt, "Situacao: Sem-teto\n");

        // 2.6: Remove a pessoa do sistema, de acordo com as instruções do arquivo .qry
        removerPessoa(htp, cpf);
    }

    // 3: Não encontrou - Imprime mensagem de erro no arquivo de saída .txt
    else fprintf(txt, "[h?] Habitante com CPF %s nao encontrado.\n", cpf);
    fprintf(txt, "--------------------------------------------------\n\n");

    // 4: Libera a estrutura de Pessoas utilizada para armazenar os dados do habitante durante a operação de registro de óbito
    freePessoa(p);
    return 0;
}

int registrarMudanca(hashPM* htp, TabelaHash* htq, char* cpf, char* novo_cep, char face, int num, char* compl, FILE* txt, FILE* qrySVG){    
    // 1: Criar uma estrutura de Pessoas para armazenar os dados do habitante que terá a mudança de endereço registrada
    Pessoas* p = criarPessoa();
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha ao criar a estrutura de Pessoas para registrar a mudanca de endereco do habitante.\n");
        return -1;
    }

    fprintf(txt, "-------------- REGISTRO DE MUDANCA ---------------\n");
    
    // 2: Busca a pessoa
    if(buscarPessoa(htp, cpf, p) == 1){
        // 2.1: Verifica se a pessoa é sem-teto, ou seja, se o campo de CEP estiver vazio. 
        // Se for sem-teto, reporta que a pessoa não pode ser registrada para mudança de endereço e encerra a função.
        if(strlen(getPessoaCep(p)) < 2){
            fprintf(txt, "[dspj] Erro: Habitante CPF %s ja e sem-teto.\n", cpf);
            freePessoa(p);
            return 0;
        }

        // 2.2: Converte chars e ints para strings
        char strFace[2] = {face, '\0'};
        char strNum[10];
        sprintf(strNum, "%d", num);

        // 2.3: SVG
        Quadras* q = criarQuadra();
        if (buscarQuadra(htq, novo_cep, q) == 1) {
            double px, py;
            calcularCoordenadaEndereco(q, face, num, &px, &py);
            
            // Marcar o endereço de destino com um pequeno quadrado vermelho no local de destino
            fprintf(qrySVG, "\t<rect x=\"%lf\" y=\"%lf\" width=\"8\" height=\"8\" fill=\"red\" />\n", px - 4, py - 4);
            // Colocar o cpf dentro do quadrado (usar fonte minúscula)
            fprintf(qrySVG, "\t<text x=\"%lf\" y=\"%lf\" font-size=\"6\" font-weight=\"bold\" fill=\"white\" text-anchor=\"middle\" alignment-baseline=\"central\">%s</text>\n", px, py + 1, cpf);
        }
        // 2.4: Libera a memória alocada para a variável local da quadra
        freeQuadra(q);

        // 2.5: Atualiza na RAM (usando o setters)
        setPessoaEndereco(p, novo_cep, strFace, strNum, compl);

        // 2.6: Salva no Disco
        if(atualizarPessoa(htp, p) == 1)
            fprintf(txt, "[mud] CPF %s mudou-se para CEP %s, Face %c, Num %d\n", cpf, novo_cep, face, num);
    }
    
    // 3: Se não encontrar, imprime mensagem de erro no arquivo de saída .txt
    else
        fprintf(txt, "[mud] Erro: Habitante CPF %s nao encontrado para mudanca.\n", cpf);
    fprintf(txt, "--------------------------------------------------\n\n");

    // 4: Libera a estrutura de Pessoas utilizada para armazenar os dados do habitante durante a operação de mudança de endereço
    freePessoa(p);
    return 0;
}

int registrarDespejo(hashPM* htp, TabelaHash* htq, char* cpf, FILE* txt, FILE* qrySVG){
    // 1: Criar uma estrutura de Pessoas para armazenar os dados do habitante que será despejado
    Pessoas* p = criarPessoa();
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha ao criar a estrutura de Pessoas para registrar o despejo do habitante.\n");
        return -1;
    }

    fprintf(txt, "--------------- REGISTRO DE DESPEJO --------------\n");
    
    // 2: Busca a pessoa
    if(buscarPessoa(htp, cpf, p) == 1){
        // 2.1: Verifica se a pessoa é sem-teto, ou seja, se o campo de CEP estiver vazio.
        // Se for sem-teto, reporta que a pessoa já é sem-teto e não pode ser despejada, e encerra a função.
        if(strlen(getPessoaCep(p)) < 2){
            fprintf(txt, "[dspj] Erro: Habitante CPF %s ja e sem-teto.\n", cpf);
            freePessoa(p);
            return 0;
        }

        // 2.2: Converte chars e ints para strings
        char* strFace = getPessoaFace(p);
        strcat(strFace, "\0");
        // getPessoaFace retorna um ponteiro para char, mas a struct e o setPessoaEndereco esperam string
        char strNum[10];
        sprintf(strNum, "%d", atoi(getPessoaNum(p))); 
        // atoi converte a string num para int, e sprintf converte de volta para string para usar no setPessoaEndereco

        // 2.3: Reportar os dados do habitante e endereço onde ocorreu o despejo
        fprintf(txt, "[dspj] CPF %s despejado do CEP %s, Face %s, Num %d\n", 
            cpf, getPessoaCep(p), getPessoaFace(p), atoi(getPessoaNum(p)));

        // 2.4: SVG - Busca a quadra para pegar a coordenada exata antes de despejar
        char cep[25];
        strcpy(cep, getPessoaCep(p));
        char face = getPessoaFace(p)[0];
        int num = atoi(getPessoaNum(p));

        // 2.5: SVG - Marcar o local do despejo com um pequeno círculo preto, indicando que a pessoa foi despejada daquele endereço
        Quadras* q = criarQuadra();
        if(buscarQuadra(htq, cep, q) == 1){
            double px, py;
            calcularCoordenadaEndereco(q, face, num, &px, &py);
            
            // Desenha um pequeno círculo preto no local da casa
            fprintf(qrySVG, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"3\" fill=\"black\" />\n", px, py);
        }
        // 2.6: Libera a memória alocada para a variável local da quadra
        freeQuadra(q);

        // 2.7: Atualiza na RAM (usando o setters)
        setPessoaEndereco(p, "", strFace, strNum, getPessoaCompl(p));

        // 2.8: Salva no Disco
        if(atualizarPessoa(htp, p) == 1)
            fprintf(txt, "CPF %s agora e sem-teto.\n", cpf);
    }
    
    // 3: Se não encontrar, imprime mensagem de erro no arquivo de saída .txt
    else
        fprintf(txt, "[dspj] Erro: Habitante CPF %s nao encontrado para despejo.\n", cpf);
    fprintf(txt, "--------------------------------------------------\n\n");

    // 4: Libera a estrutura de Pessoas utilizada para armazenar os dados do habitante durante a operação de mudança de endereço
    freePessoa(p);
    return 0;
}
/*###############################################################################################*/