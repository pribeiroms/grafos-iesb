#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cobertura.h"

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

void cobertura_imprimir(const int *cobertura, int n) {
    printf("{");
    for (int v = 0; v < n; v++)
        if (cobertura[v]) printf(" %d", v);
    printf(" }");
}

/* ------------------------------------------------------------------ */
/* Verificação                                                         */
/* ------------------------------------------------------------------ */

int eh_cobertura(const Grafo *g, const int *cobertura) {
    for (int u = 0; u < g->n; u++)
        for (int v = u + 1; v < g->n; v++)
            if (g->adj[u][v] && !cobertura[u] && !cobertura[v])
                return 0;
    return 1;
}

/* ------------------------------------------------------------------ */
/* Força bruta com backtracking                                        */
/* ------------------------------------------------------------------ */

/*
 * Estratégia: encontra uma aresta (u, v) ainda não coberta.
 * Toda cobertura precisa conter u ou v, então ramifica em duas
 * possibilidades: incluir u ou incluir v.
 *
 * Poda por limite: se o tamanho atual + 1 já for >= à melhor
 * solução encontrada, não adianta continuar nesse ramo
 * (ainda há ao menos uma aresta descoberta, logo pelo menos
 * mais um vértice será necessário).
 */
static void bt(const Grafo *g, int *atual, int tam,
               int *melhor, int *melhor_tam, long *nos) {
    (*nos)++;

    /* procura uma aresta descoberta */
    int eu = -1, ev = -1;
    for (int u = 0; u < g->n && eu < 0; u++) {
        if (atual[u]) continue;
        for (int v = u + 1; v < g->n; v++) {
            if (g->adj[u][v] && !atual[v]) {
                eu = u;
                ev = v;
                break;
            }
        }
    }

    /* todas as arestas cobertas: solução válida */
    if (eu < 0) {
        if (tam < *melhor_tam) {
            *melhor_tam = tam;
            memcpy(melhor, atual, sizeof(int) * g->n);
        }
        return;
    }

    /* poda por limite */
    if (tam + 1 >= *melhor_tam) return;

    /* ramo 1: inclui u */
    atual[eu] = 1;
    bt(g, atual, tam + 1, melhor, melhor_tam, nos);
    atual[eu] = 0;

    /* ramo 2: inclui v */
    atual[ev] = 1;
    bt(g, atual, tam + 1, melhor, melhor_tam, nos);
    atual[ev] = 0;
}

int cobertura_minima_bt(const Grafo *g, int *cobertura, long *nos) {
    int atual[MAX_VERTICES] = {0};
    long contador = 0;

    /* limite inicial: todos os vértices (sempre é cobertura) */
    int melhor_tam = g->n;
    for (int v = 0; v < g->n; v++) cobertura[v] = 1;

    bt(g, atual, 0, cobertura, &melhor_tam, &contador);

    if (nos) *nos = contador;
    return melhor_tam;
}

/* ------------------------------------------------------------------ */
/* Heurística gulosa (maior grau primeiro)                             */
/* ------------------------------------------------------------------ */

int cobertura_gulosa(const Grafo *g, int *cobertura) {
    int restante[MAX_VERTICES][MAX_VERTICES];
    memcpy(restante, g->adj, sizeof(restante));

    for (int v = 0; v < g->n; v++) cobertura[v] = 0;
    int tam = 0;

    for (;;) {
        /* encontra o vértice de maior grau restante */
        int escolhido = -1, maior_grau = 0;
        for (int u = 0; u < g->n; u++) {
            int grau = 0;
            for (int v = 0; v < g->n; v++) grau += restante[u][v];
            if (grau > maior_grau) {
                maior_grau = grau;
                escolhido = u;
            }
        }

        if (escolhido < 0) break; /* nenhuma aresta restante */

        /* adiciona à cobertura e remove suas arestas */
        cobertura[escolhido] = 1;
        tam++;
        for (int v = 0; v < g->n; v++) {
            restante[escolhido][v] = 0;
            restante[v][escolhido] = 0;
        }
    }

    return tam;
}
