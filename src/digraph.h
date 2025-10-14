#ifndef _GRAPH_H_
#define _GRAPH_H_

/**
 * Um Grafo direcionado G é constituido por um conjunto de vértices V e um conjunto de arestas E, denotado por G = (V,E).
 * Pode-se associar um dado a arestas e a vértices de G. A cada vértice é associado um nome.
 * Existem também operações que possibilitam percorrer o grafo, invocando funções quando arestas do grafo são 
 * "descobertas", podendo, inclusive, interromper o percurso. 
 * Este módulo faz parte de um trabalho de Estrutura de Dados do curso de Ciência da Computação da Universidade Estadual 
 * de Londrina. É expressamente proibido o uso de ferramentas de IA para a sua implementação. 
 * A documentação deste módulo deve ser melhorada.
 */

typedef struct graph Graph;
typedef struct node  Node;
typedef struct edge  Edge;
typedef struct info  Info;
typedef struct lista Lista;

#include <stdbool.h>
#include "lista.h"

/**
 * Invocado quando uma aresta é "descoberta"/"percorrida"/"classificada".
 * @param g O grafo em que a aresta está contida.
 * @param edge A aresta que está sendo processada.
 * @param td O tempo de descoberta da aresta.
 * @param tf O tempo de finalização da aresta.
 * @param extra Dados adicionais a serem passados para a função.
 * @return true se o percurso deve continuar. false caso contrário.
 * @note Também informa os tempos de descoberta e finalização.
 */
bool (*procEdge)(Graph* g, Edge* edge, int td, int tf, void *extra); 
/**
 * Invocado quando o percurso é recomeçado
 * @param g O grafo em que o percurso está sendo recomeçado
 * @param extra Dados adicionais a serem passados para a função
 * @return true se o percurso deve continuar. false caso contrário
 */
bool (*dfsRestarted)(Graph* g, void* extra);
/**
 * Cria um grafo com, no máximo, "nVert" vértices
 * @param nVert O número máximo de vértices que o grafo pode conter
 * @return Ponteiro para o grafo criado. NULL se não conseguiu criar o grafo
 */
Graph* createGraph(int nVert);
/**
 * Retorna o número máximo de vértices que o grafo pode conter
 * @param g O grafo
 * @return O número máximo de vértices que o grafo pode conter
 */
int getMaxNodes(Graph* g);
/**
 * Retorna o número total de vértices adicionados ao grafo
 * @param g O grafo
 * @return O número total de vértices adicionados ao grafo
 */
int getTotalNodes(Graph* g);
/**
 * Adiciona um novo vértice ao grafo "g" com o nome "nome" e associado a informação "info"
 * @param g O grafo
 * @param nome O nome do vértice
 * @param info A informação a ser associada ao vértice
 * @return O novo vértice criado. NULL em caso de falha
 */
Node* addNode(Graph* g, char* nome, Info* info);
/**
 * Retorna o nó cujo nome é "nome"
 * @param g O grafo
 * @param nome O nome do vértice
 * @return O nó correspondente ao nome. NULL se não encontrado
 */
Node* getNode(Graph* g, char* nome);
/**
 * Retorna a informação associada ao nó "node"
 * @param g O grafo
 * @param node O nó
 * @return A informação associada ao nó. NULL se não encontrado
 */
Info* getNodeInfo(Graph* g, Node* node);
/**
 * Retorna o nome associado ao nó "node"
 * @param g O grafo
 * @param node O nó
 * @return O nome associado ao nó. NULL se não encontrado
 */
char* getNodeName(Graph* g, Node* node);
/**
 * Atribui a informação "info" ao nó "node"
 * @param g O grafo
 * @param node O nó
 * @param info A informação a ser atribuída
 * @return Void
 */
void setNodeInfo(Graph* g, Node* node, Info* info);
/**
 * Adiciona uma nova aresta ao grafo "g" entre os nós "from" e "to" com a informação "info"
 * @param g O grafo
 * @param from O nó de origem
 * @param to O nó de destino
 * @param info A informação a ser associada à aresta
 * @return A nova aresta criada. NULL em caso de falha
 */
Edge* addEdge(Graph* g, Node* from, Node* to, Info* info);
/**
 * Retorna a aresta entre os nós "from" e "to"
 * @param g O grafo
 * @param from O nó de origem
 * @param to O nó de destino
 * @return A aresta entre os nós "from" e "to". NULL se não encontrada
 */
Edge* getEdge(Graph* g, Node* from, Node* to);
/**
 * Retorna o nó de origem da aresta "edge"
 * @param g O grafo
 * @param edge A aresta
 * @return O nó de origem da aresta. NULL se não encontrado
 */
Node* getFromNode(Graph* g, Edge* edge);
/**
 * Retorna o nó de destino da aresta "edge"
 * @param g O grafo
 * @param edge A aresta
 * @return O nó de destino da aresta. NULL se não encontrado
 */
Node* getToNode(Graph* g, Edge* edge);
/**
 * Retorna a informação associada à aresta "edge"
 * @param g O grafo
 * @param edge A aresta
 * @return A informação associada à aresta. NULL se não encontrado
 */
Info* getEdgeInfo(Graph* g, Edge* edge);
/**
 * Atribui a informação "info" à aresta "edge"
 * @param g O grafo
 * @param edge A aresta
 * @param info A informação a ser atribuída
 * @return Void
 */
void setEdgeInfo(Graph* g, Edge* edge, Info* info);
/**
 * Remove a aresta "edge" do grafo "g"
 * @param g O grafo
 * @param edge A aresta a ser removida
 * @return Void
 */
void removeEdge(Graph* g, Edge* edge);
/**
 * Verifica se existe uma aresta entre os nós "from" e "to"
 * @param g O grafo
 * @param from O nó de origem
 * @param to O nó de destino
 * @return true se existe uma aresta. false caso contrário
 */
bool isAdjacent(Graph* g, Node* from, Node* to);
/**
 * Adiciona à lista "nosAdjacentes" os nós adjacentes a "node"
 * @param g O grafo
 * @param node O nó
 * @param nosAdjacentes A lista de nós adjacentes
 * @return Void
 */
void adjacentNodes(Graph* g, Node* node, Lista* nosAdjacentes);
/**
 * Adiciona à lista "arestasAdjacentes" as arestas (x,y) tal que x == node
 * @param g O grafo
 * @param node O nó
 * @param arestasAdjacentes A lista de arestas adjacentes
 * @return Void
 */
void adjacentEdges(Graph* g, Node* node, Lista* arestasAdjacentes);
/**
 * Insere na lista "nomesNodes" os nomes atribuidos aos nós do grafo
 * @param g O grafo
 * @param nomesNodes A lista de nomes dos nós
 * @return Void
 */
void  getNodeNames(Graph* g, Lista* nomesNodes);
/**
 * Insere na lista "arestas", as arestas de g
 * @param g O grafo
 * @param arestas A lista de arestas
 * @return Void
 */
void getEdges(Graph* g, Lista* arestas);
/**
 * Faz percurso em profundidade sobre Graph* g, a partir do nó em Node* node, 
 * classificando as arestas do grafo, invocando a respectiva função.
 * @param g O grafo
 * @param node O nó a partir do qual o percurso deve começar
 * @param procEdge Aresta que está sendo processada
 * @param treeEdge Aresta classificada como aresta de árvore
 * @param forwardEdge Aresta classificada como aresta de avanço
 * @param returnEdge Aresta classificada como aresta de retorno
 * @param crossEdge Aresta classificada como aresta cruzada
 * @param newTree Função invocada sempre que o percurso for retomado
 * @param extra Dados adicionais a serem passados para as funções
 * @return true se o percurso foi concluído. false caso contrário
 * @note A busca em profundidade pode produzir uma floresta.
 * @note STreap* newTree é invocada sempre que o percurso for retomado.
 */
bool dfs(Graph* g, Node* node, Edge* procEdge, Edge* treeEdge, Edge* forwardEdge, Edge* returnEdge,
	      Edge* crossEdge, STreap* newTree, void *extra);
/**
 * Percorre Graph* g em largura, a partir de Node* node. discoverNode é usada para a aresta (x,y) usada para "descobrir" o y.
 * @param g O grafo
 * @param node O nó a partir do qual o percurso deve começar
 * @param discoverNode Aresta usada para "descobrir" o nó y
 * @param extra Dados adicionais a serem passados para a função
 * @return true se o percurso foi concluído. false caso contrário
 */
bool bfs(Graph* g, Node* node, Edge* discoverNode, void *extra);
/**
 * Destroi o grafo "g", liberando todos os recursos alocados por ele
 * @param g O grafo a ser destruído
 * @return Void
 */
void killDG(Graph* g);

#endif