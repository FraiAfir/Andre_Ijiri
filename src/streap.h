#ifndef _STREAP_H_
#define _STREAP_H_

// DEFINIÇÕES E DECLARAÇÕES RELACIONADAS A ÁRVORE TREAP ESPACIAL (STreap)
/**
 * Uma Treap espacial (STreap) é uma árvore Treap que associa uma coordenada no plano (âncora) a uma dada informação.
 * A chave de busca é a coordenada da âncora.
 *
 * A ordem da chave de busca é definida como a seguir. Sejam ch1 e ch2 duas chaves de busca. Define-se que ch1 < ch2, se:
 *  * ch1.x < ch2.x
 *  OU
 *  * ch1.x == ch2.x  AND ch1.y < ch2.y
 * 
 * caso contrário, ch1 == ch2
 *
 * Associa-se uma precisão epsilon à árvore. Esta precisão é usada para determinar quando dois números reais 
 * (da chave de busca) devem ser considerados como iguais. Assim v == w, se |v - w| <= epsilon.
 * 
 * Algumas operações de busca retornam o nó da árvore onde uma determinada informação está armazenada. Os dados referentes
 * a este nó podem ser obtidos por outras operações, desde que estes nós continuem válidos. Qualquer remoção posterior 
 * faz com que nós (Node) retornados previamente devam ser considerados inválidos.
 */

typedef struct streap STreap; 
typedef struct info   Info;
typedef struct node   Node;
typedef struct lista  Lista;

/*                                       FUNÇÕES PRINCIPAIS                                      */
/**
 * Processa a informação i associada a um nó da árvore, cuja âncora é o ponto (x,y), 
 * bounding box (mbbX1,mbbY1) -- (mbbX2,mbbY2)
 * 
 * O parâmetro aux aponta para conjunto de dados (provavelmente um registro) que são compartilhados entre as sucessivas
 * invocações a esta função
 */
typedef void (*FvisitaNo) (Info* i, double x, double y, double mbbX1, double mbbY1, double mbbX2, double mbbY2, void *aux);
/**
 * Cria uma árvore treap espacial vazia
 * @param epsilon Precisão para comparar números reais
 * @return Árvore vazia, com precisão epsilon. NULL caso não tenha conseguido criar a árvore
 */
STreap* createSTrp(double epsilon);
/**
 * Insere a informacao info na árvore t, associada a coordenada (x,y)
 * @param t A árvore onde a informação será inserida
 * @param x Coordenada x da âncora
 * @param y Coordenada y da âncora
 * @param info Informação a ser armazenada
 * @return Nó onde foi inserida a informação. NULL, caso já exista informação com a mesma chave (veja acima) inserida
 */
Node* insertSTrp(STreap* t, double x, double y,  Info* info);
/**
 * Insere na lista resultado os nós da árvore (Node) cuja âncora estejam dentro da região retangular de âncora (x,y), 
 * largura w e altura h
 * @param t A árvore onde a informação será buscada
 * @param x Coordenada x da âncora do retângulo de busca
 * @param y Coordenada y da âncora do retângulo de busca
 * @param w Largura do retângulo de busca
 * @param h Altura do retângulo de busca
 * @param resultado Lista onde os nós encontrados serão armazenados
 * @return A lista resultado é alterada para conter os nós encontrados
 */
void getNodeRegiaoSTrp(STreap* t, double x, double y, double w, double h, Lista* resultado);
/**
 * Retorna a informação associada ao nó n, sua âncora (xa,ya) e o retângulo envolvente (mbbX1,mbbY1) -- (mbbX2,mbbY2)
 * @param t A árvore onde a informação será buscada
 * @param n O nó onde a informação está armazenada
 * @return A informação associada ao nó n
 * @note Este nó deve ser um nó válido (veja acima)
 */
Info* getInfoSTrp(STreap* t, Node* n);
/**
 * Retorna o nó da árvore cuja âncora é o ponto (xa,ya)
 * @param t A árvore onde a informação será buscada
 * @param xa Coordenada x da âncora
 * @param ya Coordenada y da âncora
 * @return Nó da árvore associado a âncora (xa,ya) e o retângulo envolvente. NULL, se tal âncora não existir
 */
Node* getNodeSTrp(STreap* t, double xa, double ya);
/**
 * Altera a informação associada ao nó n que deve ser existente e válido
 * @param t A árvore onde a informação será alterada
 * @param n O nó onde a informação será alterada
 * @param i A nova informação a ser associada ao nó n
 * @return Void
 * @note A âncora deste nó não são alterados
 */
void updateInfoSTrp(STreap* t, Node* n, Info* i);
/**
 * Remove o nó n da árvore, portanto, o nó n deve ser considerado inválido
 * @param t A árvore onde o nó será removido
 * @param n O nó a ser removido
 * @return A informação que estava associada a tal nó
 */
Info* deleteNodeSTrp(STreap* t, Node* n);
/**
 * Remove o nó da árvore cuja chave é a coordenada (xa,ya)
 * @param t A árvore onde o nó será removido
 * @param xa Coordenada x da âncora do nó a ser removido
 * @param ya Coordenada y da âncora do nó a ser removido
 * @return A informação que estava associada a tal nó (ou NULL, se não encontrado)
 * @note Qualquer nó da árvore retornado por operações anteriores deve ser considerado inválido
 */
Info* removeSTrp(STreap* t,double xa, double ya);
/**
 * "Desenha" (no formato dot) a arvore no arquivo nomeArq. Veja: https://graphviz.org/
 * @param t A árvore a ser impressa
 * @param nomeArq Nome do arquivo onde a árvore será impressa
 * @return Void
 */
void printSTrp(STreap* t, char *nomeArq);

/*          As próximas operações percorrem a árvore em largura/profundidade/simétrico           */
/*              Invoca a função fVisita (veja descrição acima) em cada nó visitado               */

/**
 * Percorre a árvore em largura, invocando a função fVisita (veja descrição acima) em cada nó visitado
 * @param t A árvore a ser percorrida
 * @param fVisita Função a ser invocada em cada nó visitado
 * @param aux Parâmetro adicional a ser passado para a função fVisita
 * @return Void
 */
void percursoLargura(STreap* t, FvisitaNo fVisita, void *aux);
/**
 * Percorre a árvore em simétrico, invocando a função fVisita (veja descrição acima) em cada nó visitado
 * @param t A árvore a ser percorrida
 * @param fVisita Função a ser invocada em cada nó visitado
 * @param aux Parâmetro adicional a ser passado para a função fVisita
 * @return Void
 */
void percursoSimetrico(STreap* t, FvisitaNo fVisita, void *aux);
/**
 * Percorre a árvore em profundidade, invocando a função fVisita (veja descrição acima) em cada nó visitado
 * @param t A árvore a ser percorrida
 * @param fVisita Função a ser invocada em cada nó visitado
 * @param aux Parâmetro adicional a ser passado para a função fVisita
 * @return Void
 */
void percursoProfundidade(STreap* t, FvisitaNo fVisita, void *aux);
/**
 * Desaloca todos os recursos usados pela árvore t
 * @param t A árvore a ser desalocada
 * @return Void
 */
void killSTrp(STreap* t);
/*###############################################################################################*/

#endif