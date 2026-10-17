#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

GrafoListaPonderado *criar_grafo_lista_ponderado(int n) {
    if (n < 0) return NULL;
    GrafoListaPonderado *g = malloc(sizeof(*g));
    if (g == NULL) return NULL;
    g->n = n;
    g->adj = calloc(n > 0 ? (size_t)n : 1, sizeof(*g->adj));
    if (g->adj == NULL) { free(g); return NULL; }
    return g;
}

int inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso) {
    if (g == NULL || u < 0 || v < 0 || u >= g->n || v >= g->n) return 0;
    No *no = malloc(sizeof(*no));
    if (no == NULL) return 0;
    no->destino = v;
    no->peso = peso;
    no->prox = g->adj[u];
    g->adj[u] = no;
    return 1;
}

void liberar_grafo_lista_ponderado(GrafoListaPonderado *g) {
    if (g == NULL) return;
    for (int u = 0; u < g->n; u++) {
        No *no = g->adj[u];
        while (no != NULL) {
            No *proximo = no->prox;
            free(no); no = proximo;
        }
    }
    free(g->adj); free(g);
}

/* Uma passada por todas as arestas. Retorna 1 se alguma distancia diminuiu.
 * Com parar_na_primeira, relaxa so a primeira aresta encontrada.
 */
static int relaxar_arestas(GrafoListaPonderado *g, int *dist, int *pred,
                           int parar_na_primeira) {
    int mudou = 0;
    for (int u = 0; u < g->n; u++) {
        if (dist[u] == INFINITO) continue;
        for (No *no = g->adj[u]; no != NULL; no = no->prox) {
            int v = no->destino;
            if (dist[v] == INFINITO || dist[u] + no->peso < dist[v]) {
                dist[v] = dist[u] + no->peso;
                pred[v] = u;
                mudou = 1;
                if (parar_na_primeira) return 1;
            }
        }
    }
    return mudou;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (g == NULL || dist == NULL || pred == NULL || origem < 0 ||
        origem >= g->n)
        return -1;
    for (int v = 0; v < g->n; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }
    dist[origem] = 0;
    /* Caminho simples tem no maximo n - 1 arestas. */
    for (int i = 1; i < g->n; i++)
        if (!relaxar_arestas(g, dist, pred, 0)) return 1;
    /* n-esima iteracao: qualquer melhora indica ciclo negativo. A aresta e
     * relaxada para que a cadeia de pred[] do vertice alcance o ciclo.
     */
    return relaxar_arestas(g, dist, pred, 1) ? 0 : 1;
}

static int peso_minimo(GrafoListaPonderado *g, int u, int v) {
    int menor = INFINITO;
    for (No *no = g->adj[u]; no != NULL; no = no->prox)
        if (no->destino == v && no->peso < menor) menor = no->peso;
    return menor;
}

int reconstruir_ciclo_negativo(GrafoListaPonderado *g, const int *pred) {
    if (g == NULL || pred == NULL) return -1;
    for (int inicio = 0; inicio < g->n; inicio++) {
        /* Apos n passos uma cadeia sem -1 repetiu vertice: x esta no ciclo. */
        int x = inicio, i = 0;
        while (i < g->n && x != -1) { x = pred[x]; i++; }
        if (x == -1) continue;

        int tamanho = 0;
        int v = x;
        do { v = pred[v]; tamanho++; } while (v != x);

        /* pred[] aponta para tras; inverte para imprimir no sentido das arestas. */
        int *ciclo = malloc((size_t)tamanho * sizeof(*ciclo));
        if (ciclo == NULL) return -1;
        v = x;
        for (int k = tamanho - 1; k >= 0; k--) { ciclo[k] = v; v = pred[v]; }

        long long custo = 0;
        printf("Ciclo negativo:");
        for (int k = 0; k < tamanho; k++) {
            int u = ciclo[k], w = ciclo[(k + 1) % tamanho];
            custo += peso_minimo(g, u, w);
            printf(" %d ->", u);
        }
        printf(" %d (custo %lld)\n", ciclo[0], custo);
        free(ciclo);
        return tamanho;
    }
    return 0;
}
