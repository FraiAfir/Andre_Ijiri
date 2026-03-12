#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "geo.h"
#include "qry.h"
#include "params.h"
#include "lista.h"


/*                                       FUNÇÕES AUXILIARES                                      */
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