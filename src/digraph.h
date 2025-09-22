#ifndef _GRAPH_H_
#define _GRAPH_H_

/*
Um Grafo direcionado G e' constituido por um conjunto de vertices V
e um conjunto de arestas E, denotado por G = (V,E). 

Pode-se associar um dado a arestas e a vertices de G.
A cada vertice e' associado um nome.

Existem tambem operacoes que possibilitam percorrer o grafo,
invocando funcoes quando arestas do grafo 
sao "descobertas", podendo, inclusive, interromper o percurso.

Este modulo faz parte de um trabalho de Estrutura de Dados do 
curso de Ciencia da Computacao da Universidade Estadual de Londrina.
E' expressamente proibido o uso de ferramentas de IA para a sua implementacao.
A documentacao deste modulo deve ser melhorada.
*/

typedef struct graph Graph;
typedef struct node  Node;
typedef struct edge  Edge;
typedef struct info  Info;
typedef struct lista Lista;

/*
  Invocado quando uma aresta é "descoberta"/"percorrida"/"classificada". 
  Tambem informa os tempos de descoberta e finalizacao
 */
bool (*procEdge)(Graph*, Edge*, int td, int tf, void *extra); 

/*
  Invocado quando percurso e' recomecado
 */
bool (*dfsRestarted)(Graph*, void *extra);



/*
    Cria um grafo com, no maximo, "nVert" vertices.
 */
Graph* createGraph(int nVert);


/*
    Retorna numero maximo de vertices que grafo g pode conter.
 */
int getMaxNodes(Graph g);


/*
    Retorna numero total de vertices adicionados ao grafo gr.
 */
int getTotalNodes(Graph g);


/*
    Adiciona um novo vértice ao grafo "g" com o nome "nome".
 */
Node* addNode(Graph*, char* nome, info);


/*
    Retorna no' cujo de nome e' "nome". 
 */
Node* getNode(Graph*, char* nome);


/*
 */
Info* getNodeInfo(Graph*, node);


/*
 */
char* getNodeName(Graph*, node);


/*
 */
void setNodeInfo(Graph*, node, info);


/*
 */
Edge* addEdge(Graph*, Node* from, Node* to, info);


/*
 */
Edge* getEdge(Graph*, Node* from, Node* to);


/*
 */
Node* getFromNode(Graph*, Edge*);

  
/*
 */  
Node* getToNode(Graph*, Edge*);


/*
 */
Info getEdgeInfo(Graph*, Edge*);


/*
 */
void setEdgeInfo(Graph*, Edge*, info);


/*
 */
void removeEdge(Graph*,Edge*);


/*
 */
bool isAdjacent(Graph g, Node from, Node to);


/* 
   Adiciona 'a lista "nosAdjacentes" os nos adjacentes 'a "node".
 */
void adjacentNodes(Graph*, node, Lista* nosAdjacentes);


/*
   Adiciona 'a lista "arestaAdjacentes" as arestas (x,y), tal que,
   x == node.
 */
void adjacentEdges(Graph*, node, Lista* arestasAdjacentes);


/*
   Insere na lista "nomesNodes" os nomes atribuidos aos nos do grafo.
 */
void  getNodeNames(Graph*, Lista* nomesNodes);


/*
   Insere na lista "arestas", as arestas de g.
 */
void getEdges(Graph*, Lista* arestas);


/*
   Faz percurso em profundidade sobre  Graph*, a partir do no' em Node*, classificando 
   as arestas do grafo, invocando a respectiva funcao.
      A busca em profundidade, eventualmente, pode produzir uma floresta.
   newTree e' invocada sempre que o percurso for retomado.
 */  
bool dfs(Graph*, Node*, Edge* procEdge, Edge* treeEdge, Edge* forwardEdge, Edge* returnEdge,
	 Edge* crossEdge, newTree, void *extra);


/*
   Percorre o grafo g em largura, a partir do no' node. discoverNode e' usada
   para a aresta (x,y) usada para "descobrir" o y.
 */
bool bfs(Graph*, Node*, discoverNode, void *extra);


/*
   Destroi o grafo "g".
 */
void killDG(Graph*);

#endif