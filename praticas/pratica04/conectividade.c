#include <stdlib.h>
#include "conectividade.h"

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
    if (g == NULL || u < 0 || v < 0 || u >= g->n || v >= g->n || u == v)
        return 0;
    for (No *no = g->adj[u]; no != NULL; no = no->prox)
        if (no->destino == v) return 1;
    No *uv = malloc(sizeof(*uv)), *vu = malloc(sizeof(*vu));
    if (uv == NULL || vu == NULL) { free(uv); free(vu); return 0; }
    uv->destino = v; uv->prox = g->adj[u];
    vu->destino = u; vu->prox = g->adj[v];
    g->adj[u] = uv; g->adj[v] = vu;
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
    free(g->adj); free(g);
}

static void tarjan(GrafoLista *g, int u, int pai, int *tempo,
                   int *descoberta, int *low, int *articulacao,
                   Ponte *pontes, int *quantidade) {
    descoberta[u] = low[u] = ++(*tempo);
    int filhos = 0;
    for (No *no = g->adj[u]; no != NULL; no = no->prox) {
        int v = no->destino;
        if (descoberta[v] == 0) {
            filhos++;
            tarjan(g, v, u, tempo, descoberta, low, articulacao, pontes, quantidade);
            if (low[v] < low[u]) low[u] = low[v];
            if (articulacao != NULL && pai != -1 && low[v] >= descoberta[u])
                articulacao[u] = 1;
            if (pontes != NULL && low[v] > descoberta[u]) {
                pontes[*quantidade].u = u;
                pontes[*quantidade].v = v;
                (*quantidade)++;
            }
        } else if (v != pai && descoberta[v] < low[u]) {
            low[u] = descoberta[v];
        }
    }
    if (articulacao != NULL && pai == -1 && filhos > 1) articulacao[u] = 1;
}

static int executar_tarjan(GrafoLista *g, int *articulacao, Ponte *pontes,
                          int *quantidade) {
    size_t capacidade = g->n > 0 ? (size_t)g->n : 1;
    int *descoberta = calloc(capacidade, sizeof(*descoberta));
    int *low = calloc(capacidade, sizeof(*low));
    if (descoberta == NULL || low == NULL) {
        free(descoberta); free(low);
        return 0;
    }
    int tempo = 0;
    for (int u = 0; u < g->n; u++)
        if (descoberta[u] == 0)
            tarjan(g, u, -1, &tempo, descoberta, low, articulacao, pontes, quantidade);
    free(descoberta); free(low);
    return 1;
}

int dfs_articulacoes(GrafoLista *g, int *articulacao) {
    if (g == NULL || (g->n > 0 && articulacao == NULL)) return -1;
    for (int u = 0; u < g->n; u++) articulacao[u] = 0;
    if (!executar_tarjan(g, articulacao, NULL, NULL)) return -1;
    int quantidade = 0;
    for (int u = 0; u < g->n; u++) quantidade += articulacao[u];
    return quantidade;
}

Ponte *detectar_pontes(GrafoLista *g, int *quantidade) {
    if (quantidade == NULL) return NULL;
    *quantidade = -1;
    if (g == NULL) return NULL;
    /* As pontes formam uma floresta, portanto sao no maximo n-1. */
    Ponte *pontes = malloc((g->n > 0 ? (size_t)g->n : 1) * sizeof(*pontes));
    if (pontes == NULL) return NULL;
    int total = 0;
    if (!executar_tarjan(g, NULL, pontes, &total)) { free(pontes); return NULL; }
    *quantidade = total;
    return pontes;
}
