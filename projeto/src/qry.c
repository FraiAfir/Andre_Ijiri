#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qry.h"
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

int readFileQry(FILE* arquivoQry, hashPM* htp, TabelaHash* h){
    // 1: Inicializa o buffer para leitura das linhas do arquivo .qry e abre o arquivo de saída .txt para escrita dos resultados das consultas do arquivo .qry
    char linha[256];
    FILE* txt = fopen("qry.txt", "w");
    if(txt == NULL){
        fprintf(stderr, "ERRO: Nao foi possivel criar o arquivo de saida.txt para escrita.\n");
        return -1;
    }

    // 2: Lê o arquivo linha por linha
    while(fgets(linha, sizeof(linha), arquivoQry) != NULL){

        // 3: Limpeza da linha
        linha[strcspn(linha, "\n")] = '\0'; // Remove o ENTER do final da linha, se existir
        linha[strcspn(linha, "\r")] = '\0'; // Previne bugs de quebra de linha do Windows
        if(strlen(linha) == 0) continue;    // Ignora linhas em branco

        printf("\nLendo linha do .qry: %s\n", linha);

        // 4: Extrai apenas o comando (string) para sabermos o que fazer
        char* comando = strtok(linha, " \n\r");

        // 5: Processa o comando lido do arquivo .qry
        if(comando != NULL){

            // 5.1: Comando rq - Remove quadra cujo cep é cep. Moradores da quadra passam a ser sem-tetos.
            if(strcmp(comando, "rq") == 0){
                // Extrai o parâmetro do comando rq (CEP da quadra a ser removida)
                char* cep = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando rq foi lido corretamente
                if(cep != NULL){
                    printf(" => COMANDO LIDO [%s]: CEP = %s\n", comando, cep);

                    // Remove a quadra do sistema, de acordo com as instruções do arquivo .qry
                    if(removerQuadraQRY(htp, cep, txt, h) != 0){
                        fprintf(stderr, "ERRO: Falha ao remover a quadra do sistema.\n");
                        return -1;
                    }

                }else{
                    printf("ERRO: Parametro do comando rq lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 5.2: Comando pq - Calcula o número de moradores da quadra (por face e total).
            if(strcmp(comando, "pq") == 0){
                // Extrai o parâmetro do comando pq (CEP da quadra a ser removida)
                char* cep = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando pq foi lido corretamente
                if(cep != NULL){
                    printf(" => COMANDO LIDO [%s]: CEP = %s\n", comando, cep);
                    
                    // Calcula o número de moradores da quadra, de acordo com as instruções do arquivo .qry
                    if(calcMoradores(htp, cep, txt) != 0){
                        fprintf(stderr, "ERRO: Falha ao calcular o numero de moradores da quadra.\n");
                        return -1;
                    }

                }else{
                    printf("ERRO: Parametro do comando pq lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 5.3: Comando censo - Produz várias estatísticas sobre habitantes de Bitnópolis.
            if(strcmp(comando, "censo") == 0){
                printf(" => COMANDO LIDO [%s]\n", comando);

                // Produz várias estatísticas sobre habitantes de Bitnópolis, de acordo com as instruções do arquivo .qry
                if(produzirCenso(htp, txt) != 0){
                    fprintf(stderr, "ERRO: Falha ao produzir as estatisticas do censo.\n");
                    return -1;
                }
            }

            // 5.4: Comando h? - Dados sobre habitante identificado por cpf.
            if(strcmp(comando, "h?") == 0){
                // 5.4.1: Extrai o parâmetro do comando h? (CPF do habitante)
                char* cpf = strtok(NULL, " \n\r");

                // 5.4.2: Verifica se o parâmetro do comando h? foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);
                    if(obterDadosHabitante(htp, cpf, txt) != 0){
                        fprintf(stderr, "ERRO: Falha ao obter os dados do habitante identificado pelo CPF.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando \"h?\" lido do arquivo .qry \"é\" NULL.\n");
                    return -1;
                }
            }

            // 5.5: Comando nasc - Pessoa nasce.
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
                    if(registrarNascimento(htp, cpf, nome, sobrenome, sexo, nasc, txt) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o nascimento do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando nasc lidos do arquivo .qry sao invalidos (NULL).\n");
                    return -1;
                }
            }

            // 5.6: Comando rip - Pessoa falece.
            if(strcmp(comando, "rip") == 0){
                // Extrai o parâmetro do comando rip (CPF do habitante)
                char* cpf = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando rip foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    // Registra o falecimento do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarObito(htp, cpf, txt) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o óbito do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando rip lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

            // 5.7: Comando mud - Morador identificado por cpf muda-se para novo endereço.
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
                    if(registrarMudanca(htp, cpf, cep, face, num, cmpl, txt) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar a mudanca de endereco do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametros do comando mud lidos do arquivo .qry sao invalidos (NULL ou num == 0).\n");
                    return -1;
                }
            }

            // 5.8: Comando dspj - Morador identificado por cpf  é despejado.
            if(strcmp(comando, "dspj") == 0){
                // Extrai os parâmetros do comando dspj (CPF do habitante, CEP, face, número e complemento do endereço de despejo)
                char* cpf        = strtok(NULL, " \n\r");

                // Verifica se o parâmetro do comando dspj foi lido corretamente
                if(cpf != NULL){
                    printf(" => COMANDO LIDO [%s]: CPF = %s\n", comando, cpf);

                    // Registra o despejo do habitante, de acordo com as instruções do arquivo .qry
                    if(registrarDespejo(htp, cpf, txt) != 0){
                        fprintf(stderr, "ERRO: Falha ao registrar o despejo do habitante.\n");
                        return -1;
                    }
                }else{
                    printf("ERRO: Parametro do comando dspj lido do arquivo .qry eh NULL.\n");
                    return -1;
                }
            }

        }else{
            printf("ERRO: Comando lido do arquivo .qry \"é\" NULL.\n");
            return -1;
        }
    }

    return 0;
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
    if(readFileQry(arquivoQry, htp, h) != 0){ 
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
    if(qry == NULL) return 0;

    free(qry->comando);
    free(qry->cep);
    free(qry->cpf);
    free(qry->nome);
    free(qry->sobrenome);
    free(qry->sexo);
    free(qry->nasc);
    free(qry->cmpl);

    free(qry);
    return 0;
}

int removerQuadraQRY(hashPM* htp, char* cep, FILE* txt, TabelaHash* h){
    // 1: Buscar a quadra em TabelaHash
    // 2: Guardar as coordenadas x e y da âncora da quadra para usar no SVG

    // 3: Despeja os moradores da quadra a ser removida, ou seja, remove os dados de moradia (CEP, face, num, compl) dos registros das pessoas que moram na quadra e salva os buckets atualizados no disco
    despejarMoradoresQuadra(htp, cep, txt);

    fprintf(txt, "---------------- REMOCAO DE QUADRA ---------------\n");
    
    // 4: Remove a quadra do sistema, de acordo com as instruções do arquivo .qry
    if(removerQuadra(h, cep) == 1)
    fprintf(txt, "[rq] Quadra %s removida fisicamente do sistema.\n", cep);
    else
    fprintf(txt, "[rq] ERRO: Quadra %s nao encontrada.\n", cep);
    
    
    // 5: TO-DO: SVG - colocar um pequeno X vermelho no local da âncora da quadra removida.
    
    fprintf(txt, "------------------------------------------------\n\n");
    return 0;
}

int calcMoradores(hashPM* htp, char* cep, FILE* txt){
    // 1: Variáveis locais para receber os resultados
    int morN = 0, morS = 0, morL = 0, morO = 0;

    fprintf(txt, "--------- CALCULO DE MORADORES DA QUADRA ---------\n");

    // 2: Chama o banco de dados passando os endereços para ele preencher
    calcularMoradoresQuadra(htp, cep, &morN, &morS, &morL, &morO);

    
    fprintf(txt, "[pq] Quadra %s - Moradores por face: N=%d | S=%d | L=%d | O=%d | Total=%d\n", 
        cep, morN, morS, morL, morO, morN + morS + morL + morO);

    // TO-DO: SVG - colocar número de moradores de cada face (próximo ao limite da face) e, 
    // no centro da quadra, o número total de moradores da quadra

    fprintf(txt, "--------------------------------------------------\n\n");
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
    fprintf(txt, "\n\n\n\n===== CENSO DE BITNOPOLIS =====\n");
    fprintf(txt, "Total de Habitantes: %d\n", totHab);
    fprintf(txt, "Total de Moradores: %d\n", totMor);
    fprintf(txt, "Total de Sem-Teto: %d\n", totSemTeto);
    
    // 4: Evita divisão por zero no cálculo das porcentagens
    if(totHab > 0){
        fprintf(txt, "\n--- PROPORCOES ---\n");
        fprintf(txt, "Proporcao Moradores/Habitantes: %.2f%%\n", (totMor / (float)totHab) * 100);
        fprintf(txt, "Homens: %d (%.2f%%) | Mulheres: %d (%.2f%%)\n", masc, (masc / (float)totHab) * 100, fem, (fem / (float)totHab) * 100);
    }
    if(totMor > 0){
        fprintf(txt, "\n--- MORADORES ---\n");
        fprintf(txt, "Moradores Homens: %d (%.2f%%) | Moradores Mulheres: %d (%.2f%%)\n", morMasc, (morMasc / (float)totMor) * 100, morFem, (morFem / (float)totMor) * 100);
    }
    if(totSemTeto > 0){
        fprintf(txt, "\n--- SEM-TETOS ---\n");
        fprintf(txt, "Sem-teto Homens: %d (%.2f%%) | Sem-teto Mulheres: %d (%.2f%%)\n", semTetoMasc, (semTetoMasc / (float)totSemTeto) * 100, semTetoFem, (semTetoFem / (float)totSemTeto) * 100);
    }
    fprintf(txt, "===============================\n\n\n\n\n\n");

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
        
        else 
            fprintf(txt, "Situacao: Sem-teto\n");
    }

    // 3: Não encontrou - Imprime mensagem de erro no arquivo de saída .txt
    else fprintf(txt, "[h?] Habitante com CPF %s nao encontrado.\n", cpf);

    fprintf(txt, "--------------------------------------------------\n\n");
    return 0;
}

int registrarNascimento(hashPM* htp, char* cpf, char* nome, char* sobrenome, char sexo, char* nasc, FILE* txt){
    // 1: Converte o char de sexo para uma string de 2 caracteres (1 para o sexo e 1 para o terminador nulo) 
    char strSexo[2] = {sexo, '\0'}; // sexo é char mas a struct e o inserirRegPM esperam string

    fprintf(txt, "-------------- REGISTRO DE NASCIMENTO ------------\n");

    // 2: Registra o nascimento do habitante, de acordo com as instruções do arquivo .qry
    if(inserirRegPM(htp, cpf, nome, sobrenome, strSexo, nasc) == 0)
        fprintf(txt, "[nasc] Habitante %s %s (CPF: %s) nasceu com sucesso.\n", nome, sobrenome, cpf);
    else
        fprintf(txt, "[nasc] Falha ao registrar nascimento do CPF: %s\n", cpf);

    fprintf(txt, "--------------------------------------------------\n\n");
    return 0;
}

int registrarObito(hashPM* htp, char* cpf, FILE* txt){
    // 1: Criar uma estrutura de Pessoas para armazenar os dados do habitante a ser buscado
    Pessoas* p = criarPessoa();
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha ao criar a estrutura de Pessoas para obter os dados do habitante.\n");
        return -1;
    }

    fprintf(txt, "--------------- REGISTRO DE OBITO ----------------\n");
    
    // 2: Busca a pessoa
    if(buscarPessoa(htp, cpf, p) == 1){
        // 1.1: Encontrou - Imprime os dados do habitante no arquivo de saída .txt
        fprintf(txt, "[rip] Dados do Habitante %s:\n", cpf);
        fprintf(txt, "Morador %s faleceu.\n", cpf);
        fprintf(txt, "Nome: %s %s | Sexo: %s | Nasc: %s\n", 
            getPessoaNome(p), getPessoaSobrenome(p), getPessoaSexo(p), getPessoaNasc(p));
        
        // 1.2: Se for morador, reportar também o endereço. Se não for morador, reportar que é sem-teto
        if(strlen(getPessoaCep(p)) > 0) 
            fprintf(txt, "Endereco: CEP %s, Face %s, Num %s, Compl %s\n", 
                getPessoaCep(p), getPessoaFace(p), getPessoaNum(p), getPessoaCompl(p));
        
        else 
            fprintf(txt, "Situacao: Sem-teto\n");

        // 1.3: Remove a pessoa do sistema, de acordo com as instruções do arquivo .qry
        removerPessoa(htp, cpf);

        // TO-DO: SVG - Colocar uma pequena cruz vermelha no local do endereço (se morador).
    }

    // 3: Não encontrou - Imprime mensagem de erro no arquivo de saída .txt
    else fprintf(txt, "[h?] Habitante com CPF %s nao encontrado.\n", cpf);

    // 4: Libera a estrutura de Pessoas utilizada para armazenar os dados do habitante durante a operação de registro de óbito
    freePessoa(p);

    fprintf(txt, "--------------------------------------------------\n\n");
    return 0;
}

int registrarMudanca(hashPM* htp, char* cpf, char* cep, char face, int num, char* cmpl, FILE* txt){
    // 1: Criar uma estrutura de Pessoas para armazenar os dados do habitante que terá a mudança de endereço registrada
    Pessoas* p = criarPessoa();
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha ao criar a estrutura de Pessoas para registrar a mudanca de endereco do habitante.\n");
        return -1;
    }

    fprintf(txt, "-------------- REGISTRO DE MUDANCA ---------------\n");
    
    // 2: Busca a pessoa
    if(buscarPessoa(htp, cpf, p) == 1){
        // 2.1: Converte chars e ints para strings
        char strFace[2] = {face, '\0'};
        char strNum[10];
        sprintf(strNum, "%d", num);

        // 2.2: Atualiza na RAM (usando o setters)
        setPessoaEndereco(p, cep, strFace, strNum, cmpl);

        // 2.3: Salva no Disco
        if(atualizarPessoa(htp, p) == 1)
            fprintf(txt, "[mud] CPF %s mudou-se para CEP %s, Face %c, Num %d\n", cpf, cep, face, num);

        // TO-DO: SVG - marcar o endereço de destino com um pequeno quadrado vermelho no local de destino. Colocar o cpf dentro do quadrado (usar fonte minúscula)
    }
    
    // 3: Se não encontrar, imprime mensagem de erro no arquivo de saída .txt
    else
        fprintf(txt, "[mud] Erro: Habitante CPF %s nao encontrado para mudanca.\n", cpf);

    // 4: Libera a estrutura de Pessoas utilizada para armazenar os dados do habitante durante a operação de mudança de endereço
    freePessoa(p);

    fprintf(txt, "--------------------------------------------------\n\n");
    return 0;
}

int registrarDespejo(hashPM* htp, char* cpf, FILE* txt){
    // 1: Criar uma estrutura de Pessoas para armazenar os dados do habitante que será despejado
    Pessoas* p = criarPessoa();
    if(p == NULL){
        fprintf(stderr, "ERRO: Falha ao criar a estrutura de Pessoas para registrar o despejo do habitante.\n");
        return -1;
    }

    fprintf(txt, "--------------- REGISTRO DE DESPEJO --------------\n");
    
    // 2: Busca a pessoa
    if(buscarPessoa(htp, cpf, p) == 1){
        // 2.1: Converte chars e ints para strings
        char* strFace = getPessoaFace(p);
        strcat(strFace, "\0");
        // getPessoaFace retorna um ponteiro para char, mas a struct e o setPessoaEndereco esperam string

        char strNum[10];
        sprintf(strNum, "%d", atoi(getPessoaNum(p))); 
        // atoi converte a string num para int, e sprintf converte de volta para string para usar no setPessoaEndereco

        // 2.2: Reportar os dados do habitante e endereço onde ocorreu o despejo
        fprintf(txt, "[dspj] CPF %s despejado do CEP %s, Face %s, Num %d\n", 
            cpf, getPessoaCep(p), getPessoaFace(p), atoi(getPessoaNum(p)));

        // 2.3: Atualiza na RAM (usando o setters)
        setPessoaEndereco(p, "", strFace, strNum, getPessoaCompl(p));

        // 2.4: Salva no Disco
        if(atualizarPessoa(htp, p) == 1)
            fprintf(txt, "CPF %s agora e sem-teto.\n", cpf);

        // TO-DO: SVG - colocar um pequeno círculo preto no local do despejo
    }
    
    // 3: Se não encontrar, imprime mensagem de erro no arquivo de saída .txt
    else
        fprintf(txt, "[dspj] Erro: Habitante CPF %s nao encontrado para despejo.\n", cpf);

    // 4: Libera a estrutura de Pessoas utilizada para armazenar os dados do habitante durante a operação de mudança de endereço
    freePessoa(p);

    fprintf(txt, "--------------------------------------------------\n\n");
    return 0;
}
/*###############################################################################################*/