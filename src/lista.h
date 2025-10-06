#ifndef _Lista_h_
#define _Lista_h_

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct lista Lista;
typedef struct cel Celula;

Lista*  criaLista     (                );
Celula* buscarElemento(Lista*,      int);
int     inserirFim    (Lista*,      int);
int     removerMeio   (Lista*,      int);
void    imprimirLista (Lista*          );
int     getValor      (Celula*         );
void    limparLista   (Lista*          );

#endif