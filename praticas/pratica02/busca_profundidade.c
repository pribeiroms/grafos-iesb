#include <stdlib.h>
#include "busca_profundidade.h"

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo,
                   int *entrada, int *saida, int *pred) {
    if (g == NULL || u < 0 || u >= g->n || visitado == NULL ||
        tempo == NULL || entrada == NULL || saida == NULL) return;

    visitado[u] = 1;
    entrada[u] = ++(*tempo);
    for (No *no = g->adj[u]; no != NULL; no = no->prox) {
        int v = no->destino;
        if (!visitado[v]) {
            if (pred != NULL) pred[v] = u;
            dfs_recursiva(g, v, visitado, tempo, entrada, saida, pred);
        }
    }
    saida[u] = ++(*tempo);
}

void dfs_iterativa(GrafoLista *g, int origem, int *visitado) {
    if (g == NULL || origem < 0 || origem >= g->n || visitado == NULL) return;
    Pilha pilha;
    pilha.dados = malloc((size_t)g->n * sizeof(*pilha.dados));
    pilha.topo = -1;
    pilha.capacidade = g->n;
    if (pilha.dados == NULL) return;

    visitado[origem] = 1;
    pilha.dados[++pilha.topo] = origem;
    while (pilha.topo >= 0) {
        int u = pilha.dados[pilha.topo--];
        for (No *no = g->adj[u]; no != NULL; no = no->prox) {
            if (!visitado[no->destino]) {
                visitado[no->destino] = 1;
                pilha.dados[++pilha.topo] = no->destino;
            }
        }
    }
    free(pilha.dados);
}

static void marcar_componente(GrafoLista *g, int u, int *visitado) {
    visitado[u] = 1;
    for (No *no = g->adj[u]; no != NULL; no = no->prox)
        if (!visitado[no->destino]) marcar_componente(g, no->destino, visitado);
}

int contar_componentes(GrafoLista *g) {
    if (g == NULL) return 0;
    int *visitado = calloc((size_t)g->n, sizeof(*visitado));
    if (visitado == NULL) return -1;
    int componentes = 0;
    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            componentes++;
            marcar_componente(g, i, visitado);
        }
    }
    free(visitado);
    return componentes;
}

static int ciclo_dfs(GrafoLista *g, int u, int pai, int *visitado) {
    visitado[u] = 1;
    for (No *no = g->adj[u]; no != NULL; no = no->prox) {
        int v = no->destino;
        if (!visitado[v]) {
            if (ciclo_dfs(g, v, u, visitado)) return 1;
        } else if (v != pai) {
            return 1;
        }
    }
    return 0;
}

int tem_ciclo(GrafoLista *g) {
    if (g == NULL) return 0;
    int *visitado = calloc((size_t)g->n, sizeof(*visitado));
    if (visitado == NULL) return 0;
    for (int i = 0; i < g->n; i++) {
        if (!visitado[i] && ciclo_dfs(g, i, -1, visitado)) {
            free(visitado);
            return 1;
        }
    }
    free(visitado);
    return 0;
}
