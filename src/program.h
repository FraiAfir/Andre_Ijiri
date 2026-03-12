#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "geo.h"
#include "qry.h"
#include "params.h"
#include "lista.h"


/*                                       FUNÇÕES AUXILIARES                                      */
/**
 * Função de comparação entre as informações dos nós da árvore. Deve ser implementada de acordo com o tipo de informação armazenada nos nós da árvore
 * @param info1 Informação a ser inserida
 * @param info2 Informação do nó atual da árvore
 * @return      true se info1 for menor que info2, false caso contrário
 */
bool cmpFuncGeo(void* info1, void* info2);
/*###############################################################################################*/



/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Função para inicializar os objetos de Parametro e da ED para armazenar os dados do arquivo .geo
 * @param param Ponteiro para o objeto criado de Parametro
 * @param l     Ponteiro para o objeto criado da ED
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int bootProgram(Param** param, Lista** l, Geo** geo, Qry** qry);
/**
 * Função para liberar a memória alocada para os objetos de Parametro e da ED
 * @param param Ponteiro para o objeto de Parametro
 * @param l     Ponteiro para o objeto da ED
 * @return      0 em caso de sucesso. -1 em caso de erro
 */
int shutProgram(Param** param, Lista** l, Geo** geo, Qry** qry);

/*###############################################################################################*/

#endif