#include <stdlib.h>
#include "dag.h"

GrafoLista *criar_grafo_lista(int n) {
    if (n < 0) return NULL;
    GrafoLista *g = malloc(sizeof(*g));
    if (g == NULL) return NULL;
    g->n = n;
    g->adj = calloc(n > 0 ? (size_t)n : 1, sizeof(*g->adj));
    if (g->adj == NULL) { free(g); return NULL; }
    return g;
}

int inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (g == NULL || u < 0 || v < 0 || u >= g->n || v >= g->n) return 0;
    for (No *no = g->adj[u]; no != NULL; no = no->prox)
        if (no->destino == v) return 1;
    No *no = malloc(sizeof(*no));
    if (no == NULL) return 0;
    no->destino = v;
    no->prox = g->adj[u];
    g->adj[u] = no;
    return 1;
}

void liberar_grafo_lista(GrafoLista *g) {
    if (g == NULL) return;
    for (int u = 0; u < g->n; u++) {
        No *no = g->adj[u];
        while (no != NULL) {
            No *proximo = no->prox;
            free(no);
            no = proximo;
        }
    }
    free(g->adj);
    free(g);
}

int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    if (tamanho == NULL) return NULL;
    *tamanho = 0;
    if (g == NULL) return NULL;
    size_t capacidade = g->n > 0 ? (size_t)g->n : 1;
    int *grau = calloc(capacidade, sizeof(*grau));
    int *ordem = malloc(capacidade * sizeof(*ordem));
    if (grau == NULL || ordem == NULL) {
        free(grau); free(ordem);
        return NULL;
    }
    for (int u = 0; u < g->n; u++)
        for (No *no = g->adj[u]; no != NULL; no = no->prox)
            grau[no->destino]++;

    /* O array de saida tambem funciona como fila FIFO: cada vertice
     * entra uma unica vez, quando seu grau de entrada chega a zero. */
    int inicio = 0, fim = 0;
    for (int u = 0; u < g->n; u++)
        if (grau[u] == 0) ordem[fim++] = u;
    while (inicio < fim) {
        int u = ordem[inicio++];
        for (No *no = g->adj[u]; no != NULL; no = no->prox)
            if (--grau[no->destino] == 0) ordem[fim++] = no->destino;
    }
    free(grau);
    if (fim != g->n) { free(ordem); return NULL; }
    *tamanho = fim;
    return ordem;
}

/* 0: nao visitado; 1: na pilha de recursao; 2: finalizado. */
static int visitar(GrafoLista *g, int u, int *cor, int *pilha, int *topo) {
    cor[u] = 1;
    for (No *no = g->adj[u]; no != NULL; no = no->prox) {
        int v = no->destino;
        if (cor[v] == 1) return 0;
        if (cor[v] == 0 && !visitar(g, v, cor, pilha, topo)) return 0;
    }
    cor[u] = 2;
    if (pilha != NULL) pilha[(*topo)++] = u;
    return 1;
}

int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    if (tamanho == NULL) return NULL;
    *tamanho = 0;
    if (g == NULL) return NULL;
    size_t capacidade = g->n > 0 ? (size_t)g->n : 1;
    int *cor = calloc(capacidade, sizeof(*cor));
    int *ordem = malloc(capacidade * sizeof(*ordem));
    if (cor == NULL || ordem == NULL) {
        free(cor); free(ordem);
        return NULL;
    }
    int topo = 0;
    for (int u = 0; u < g->n; u++) {
        if (cor[u] == 0 && !visitar(g, u, cor, ordem, &topo)) {
            free(cor); free(ordem);
            return NULL;
        }
    }
    free(cor);
    /* Desempilhar equivale a inverter a ordem de finalizacao. */
    for (int i = 0; i < topo / 2; i++) {
        int auxiliar = ordem[i];
        ordem[i] = ordem[topo - 1 - i];
        ordem[topo - 1 - i] = auxiliar;
    }
    *tamanho = topo;
    return ordem;
}

int eh_dag(GrafoLista *g) {
    if (g == NULL) return -1;
    int *cor = calloc(g->n > 0 ? (size_t)g->n : 1, sizeof(*cor));
    if (cor == NULL) return -1;
    for (int u = 0; u < g->n; u++) {
        if (cor[u] == 0 && !visitar(g, u, cor, NULL, NULL)) {
            free(cor);
            return 0;
        }
    }
    free(cor);
    return 1;
}
