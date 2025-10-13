#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geo.h"

/*                          ESTRUTURAS DE DADOS A SEREM IMPLEMENTADAS                          */
// Estrutura para armazenar os dados do arquivo .geo
typedef struct geo{
    char* cep;      // Identificador único de cada quadra
    double x, y;    // Coordenadas das quadras
    double w, h;    // Largura e altura das quadras

    double sw;      // Espessura da borda das quadras
    char*  cfill;   // Cor de preenchimento das quadras
    char*  cstrk;   // Cor da borda das quadras
}Geo;
/*#############################################################################################*/

/*                                      FUNÇÕES AUXILIARES                                     */
/*#############################################################################################*/

/*                                      FUNÇÕES PRINCIPAIS                                     */
/*#############################################################################################*/
