#ifndef AGM_H
#define AGM_H

#include <limits.h>

#define INFINITO INT_MAX

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

/* Grafo nao direcionado ponderado: inserir u - v insere u -> v e v -> u.
 * Aceita pesos negativos, lacos e arestas paralelas.
 */
typedef struct {
    int n;
    No **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_lista_ponderado(int n);
int inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo_lista_ponderado(GrafoListaPonderado *g);

/* Union-Find: cada vertice comeca em um conjunto proprio. */
DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);
/* Retorna o representante de x, com compressao de caminho; -1 se invalido. */
int find(DSU *d, int x);
/* Uniao por rank. Retorna 1 se uniu, 0 se ja estavam juntos ou invalido. */
int unir(DSU *d, int x, int y);

typedef struct {
    int vertice;
    int chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vértice -> índice no heap
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *h);
int heap_vazio(const MinHeap *h);
/* Retornam 1 em sucesso e 0 para vertice invalido, repetido ou ausente. */
int inserir_heap(MinHeap *h, int vertice, int chave);
int diminuir_chave(MinHeap *h, int vertice, int nova_chave);
/* Heap vazio retorna vertice = -1 e chave = INFINITO. */
NoHeap extrair_minimo(MinHeap *h);

/* agm[] deve ter espaco para n - 1 arestas e *custo recebe a soma dos pesos.
 * Retornam o numero de arestas escolhidas: n - 1 em grafo conexo e menos
 * em grafo desconexo (floresta geradora minima). -1 indica argumento
 * invalido ou falha de memoria.
 * Kruskal: ordena uma copia das arestas, O(E log E). Arestas de entrada
 * devem ter vertices entre 0 e n - 1.
 * Prim: heap com atualizacao de chaves, O((V + E) log V), recomecando em
 * cada componente ainda nao visitado.
 */
int kruskal(int n, const Aresta *arestas, int m, Aresta *agm, long long *custo);
int prim(GrafoListaPonderado *g, Aresta *agm, long long *custo);

#endif
