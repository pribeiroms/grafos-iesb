#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hamiltoniano.h"

/* ------------------------------------------------------------------ */
/* Operações básicas                                                   */
/* ------------------------------------------------------------------ */

void grafo_inicializar(Grafo *g, int n) {
    g->n = n;
    memset(g->adj, 0, sizeof(g->adj));
}

void grafo_adicionar_aresta(Grafo *g, int u, int v) {
    if (u == v || u < 0 || v < 0 || u >= g->n || v >= g->n) return;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

int grafo_num_arestas(const Grafo *g) {
    int m = 0;
    for (int u = 0; u < g->n; u++)
        for (int v = u + 1; v < g->n; v++)
            m += g->adj[u][v];
    return m;
}

void grafo_aleatorio(Grafo *g, int n, double prob) {
    grafo_inicializar(g, n);
    for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
            if ((double)rand() / RAND_MAX < prob)
                grafo_adicionar_aresta(g, u, v);
}

void grafo_imprimir(const Grafo *g) {
    printf("Grafo com %d vertices e %d arestas:\n", g->n, grafo_num_arestas(g));
    for (int u = 0; u < g->n; u++) {
        printf("  %2d:", u);
        for (int v = 0; v < g->n; v++)
            if (g->adj[u][v]) printf(" %d", v);
        printf("\n");
    }
}

void caminho_imprimir(const int *caminho, int n, int ciclo) {
    for (int i = 0; i < n; i++)
        printf(i == 0 ? "%d" : " -> %d", caminho[i]);
    if (ciclo && n > 0) printf(" -> %d", caminho[0]);
}

/* ------------------------------------------------------------------ */
/* Verificação                                                         */
/* ------------------------------------------------------------------ */

int eh_caminho_hamiltoniano(const Grafo *g, const int *caminho, int ciclo) {
    int usado[MAX_VERTICES] = {0};

    if (g->n == 0) return 0;
    if (ciclo && g->n < 3) return 0;

    for (int i = 0; i < g->n; i++) {
        int v = caminho[i];
        if (v < 0 || v >= g->n || usado[v]) return 0;
        usado[v] = 1;
        if (i > 0 && !g->adj[caminho[i - 1]][v]) return 0;
    }
    if (ciclo && !g->adj[caminho[g->n - 1]][caminho[0]]) return 0;
    return 1;
}

/* ------------------------------------------------------------------ */
/* Backtracking                                                        */
/* ------------------------------------------------------------------ */

/*
 * Tenta preencher a posição 'pos' da permutação. As posições
 * 0..pos-1 já formam um caminho válido no grafo.
 */
static int bt_hamiltoniano(const Grafo *g, int ciclo, int *caminho,
                           int *usado, int pos, long *nos) {
    (*nos)++;

    if (pos == g->n)
        return !ciclo || g->adj[caminho[pos - 1]][caminho[0]];

    for (int v = 0; v < g->n; v++) {
        if (usado[v] || !g->adj[caminho[pos - 1]][v]) continue;

        usado[v] = 1;
        caminho[pos] = v;
        if (bt_hamiltoniano(g, ciclo, caminho, usado, pos + 1, nos))
            return 1;
        usado[v] = 0;
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int ciclo, int *caminho, long *nos) {
    int usado[MAX_VERTICES];
    long contador = 0;
    int achou = 0;

    if (g->n == 0 || (ciclo && g->n < 3)) {
        if (nos) *nos = 0;
        return 0;
    }

    /* Num ciclo qualquer vértice pode ser a origem; basta fixar o 0. */
    int origens = ciclo ? 1 : g->n;

    for (int s = 0; s < origens && !achou; s++) {
        memset(usado, 0, sizeof(usado));
        usado[s] = 1;
        caminho[0] = s;
        achou = bt_hamiltoniano(g, ciclo, caminho, usado, 1, &contador);
    }

    if (nos) *nos = contador;
    return achou;
}

int encontrar_ciclo_hamiltoniano_bt(const Grafo *g, int *ciclo, long *nos) {
    return encontrar_caminho_hamiltoniano_bt(g, 1, ciclo, nos);
}
