#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

#define INFINITO INT_MAX

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

/* Digrafo ponderado: inserir u -> v nao insere v -> u.
 * Pesos negativos sao recusados, pois Dijkstra exige pesos >= 0.
 */
typedef struct {
    int n;
    No **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_lista_ponderado(int n);
int inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo_lista_ponderado(GrafoListaPonderado *g);

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vértice -> índice no heap
} MinHeap;

/* Vertices validos: 0..capacidade-1. posicao[v] = -1 se v nao esta no heap. */
MinHeap *criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *h);
int heap_vazio(const MinHeap *h);
/* Retornam 1 em sucesso e 0 para vertice invalido, repetido ou ausente. */
int inserir_heap(MinHeap *h, int vertice, int distancia);
int diminuir_chave(MinHeap *h, int vertice, int nova_distancia);
/* Heap vazio retorna vertice = -1 e distancia = INFINITO. */
NoHeap extrair_minimo(MinHeap *h);

/* dist[] e pred[] devem ter n posicoes. Vertices inalcancaveis ficam com
 * dist = INFINITO e pred = -1; a origem fica com pred = -1.
 * Retorna 1 em sucesso e -1 para argumento invalido ou falha de memoria.
 * Supoe que as distancias cabem em int.
 */
int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Imprime o caminho origem -> ... -> destino usando pred[] do dijkstra().
 * destino deve ser um vertice valido do grafo usado no dijkstra().
 * Retorna o numero de vertices do caminho, 0 se destino for inalcancavel
 * e -1 para argumento invalido.
 */
int reconstruir_caminho(int origem, int destino, int *pred);

#endif
