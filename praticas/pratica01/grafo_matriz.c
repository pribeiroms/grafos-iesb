#include <stdio.h>
#include <stdlib.h>
#include "grafo_matriz.h"

// verifica se o vertice existe no grafo
static int vertice_valido(const GrafoMatriz *grafo, int vertice) {
    return grafo != NULL && vertice >= 0 && vertice < grafo->n;
}

// cria o grafo e aloca a matriz dinamicamente
GrafoMatriz *criar_grafo_matriz(int n) {
    if (n <= 0) return NULL;

    GrafoMatriz *grafo = malloc(sizeof(GrafoMatriz));
    if (grafo == NULL) return NULL;

    grafo->n = n;
    grafo->adj = calloc(n, sizeof(int *));
    if (grafo->adj == NULL) {
        free(grafo);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        grafo->adj[i] = calloc(n, sizeof(int));
        if (grafo->adj[i] == NULL) {
            for (int j = 0; j < i; j++) free(grafo->adj[j]);
            free(grafo->adj);
            free(grafo);
            return NULL;
        }
    }
    return grafo;
}

// insere uma aresta nos dois sentidos
int inserir_aresta_matriz(GrafoMatriz *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v) || u == v) return 0;
    grafo->adj[u][v] = 1;
    grafo->adj[v][u] = 1;
    return 1;
}

// remove uma aresta nos dois sentidos
int remover_aresta_matriz(GrafoMatriz *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v) || u == v) return 0;
    grafo->adj[u][v] = 0;
    grafo->adj[v][u] = 0;
    return 1;
}

// conta quantas arestas estao ligadas ao vertice
int grau_matriz(const GrafoMatriz *grafo, int vertice) {
    if (!vertice_valido(grafo, vertice)) return -1;
    int grau = 0;
    for (int i = 0; i < grafo->n; i++) grau += grafo->adj[vertice][i];
    return grau;
}

// verifica se existe uma aresta entre dois vertices
int sao_adjacentes_matriz(const GrafoMatriz *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v)) return 0;
    return grafo->adj[u][v] == 1;
}

// mostra a matriz de adjacencia
void exibir_grafo_matriz(const GrafoMatriz *grafo) {
    if (grafo == NULL) return;
    for (int i = 0; i < grafo->n; i++) {
        for (int j = 0; j < grafo->n; j++) printf("%3d", grafo->adj[i][j]);
        printf("\n");
    }
}

// libera as linhas, o vetor de linhas e o grafo
void liberar_grafo_matriz(GrafoMatriz *grafo) {
    if (grafo == NULL) return;
    for (int i = 0; i < grafo->n; i++) free(grafo->adj[i]);
    free(grafo->adj);
    free(grafo);
}
