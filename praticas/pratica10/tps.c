#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tps.h"

/* ------------------------------------------------------------------ */
/* Operações básicas                                                   */
/* ------------------------------------------------------------------ */

void tsp_inicializar(TSP *t, int n) {
    t->n = n;
    memset(t->dist, 0, sizeof(t->dist));
}

void tsp_aleatorio(TSP *t, int n, int peso_max) {
    tsp_inicializar(t, n);
    for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++) {
            int w = 1 + rand() % peso_max;
            t->dist[u][v] = w;
            t->dist[v][u] = w;
        }
}

int tsp_custo_rota(const TSP *t, const int *rota) {
    int custo = 0;
    for (int i = 0; i < t->n; i++)
        custo += t->dist[rota[i]][rota[(i + 1) % t->n]];
    return custo;
}

void tsp_imprimir_rota(const int *rota, int n) {
    for (int i = 0; i < n; i++)
        printf(i == 0 ? "%d" : " -> %d", rota[i]);
    if (n > 0) printf(" -> %d", rota[0]);
}

/* ------------------------------------------------------------------ */
/* Força bruta                                                         */
/* ------------------------------------------------------------------ */

typedef struct {
    const TSP *t;
    int atual[MAX_VERTICES];
    int usado[MAX_VERTICES];
    int melhor_custo;
    int *melhor_rota;
    long permutacoes;
} EstadoBF;

static void bf_permutar(EstadoBF *e, int pos, int custo) {
    int n = e->t->n;

    if (pos == n) {
        int total = custo + e->t->dist[e->atual[n - 1]][e->atual[0]];
        e->permutacoes++;
        if (total < e->melhor_custo) {
            e->melhor_custo = total;
            memcpy(e->melhor_rota, e->atual, (size_t)n * sizeof(int));
        }
        return;
    }

    for (int v = 1; v < n; v++) {
        if (e->usado[v]) continue;
        e->usado[v] = 1;
        e->atual[pos] = v;
        bf_permutar(e, pos + 1, custo + e->t->dist[e->atual[pos - 1]][v]);
        e->usado[v] = 0;
    }
}

int tsp_forca_bruta(const TSP *t, int *rota, long *permutacoes) {
    EstadoBF e;

    if (t->n <= 0) {
        if (permutacoes) *permutacoes = 0;
        return 0;
    }

    e.t = t;
    memset(e.usado, 0, sizeof(e.usado));
    e.melhor_custo = INT_MAX;
    e.melhor_rota = rota;
    e.permutacoes = 0;

    /* Origem fixa no vértice 0 */
    e.atual[0] = 0;
    e.usado[0] = 1;
    bf_permutar(&e, 1, 0);

    if (permutacoes) *permutacoes = e.permutacoes;
    return e.melhor_custo;
}

/* ------------------------------------------------------------------ */
/* Heurística: Vizinho Mais Próximo                                    */
/* ------------------------------------------------------------------ */

int tsp_vizinho_mais_proximo(const TSP *t, int origem, int *rota) {
    int visitado[MAX_VERTICES] = {0};

    if (t->n <= 0) return 0;

    rota[0] = origem;
    visitado[origem] = 1;

    for (int i = 1; i < t->n; i++) {
        int atual = rota[i - 1];
        int prox = -1;
        for (int v = 0; v < t->n; v++) {
            if (visitado[v]) continue;
            if (prox == -1 || t->dist[atual][v] < t->dist[atual][prox])
                prox = v;
        }
        rota[i] = prox;
        visitado[prox] = 1;
    }
    return tsp_custo_rota(t, rota);
}

/* ------------------------------------------------------------------ */
/* Heurística: Inserção do Mais Próximo                                */
/* ------------------------------------------------------------------ */

int tsp_insercao(const TSP *t, int *rota) {
    int na_rota[MAX_VERTICES] = {0};
    int dist_rota[MAX_VERTICES]; /* menor distância de v até a rota */
    int n = t->n;
    int k;

    if (n <= 0) return 0;

    rota[0] = 0;
    na_rota[0] = 1;
    k = 1;
    for (int v = 0; v < n; v++)
        dist_rota[v] = t->dist[0][v];

    while (k < n) {
        /* 1) Seleção: cidade fora da rota mais próxima da rota */
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (na_rota[v]) continue;
            if (u == -1 || dist_rota[v] < dist_rota[u]) u = v;
        }

        /* 2) Inserção: posição que menos aumenta o custo */
        int melhor_pos = k;
        if (k >= 2) {
            int melhor_aumento = INT_MAX;
            for (int i = 0; i < k; i++) {
                int a = rota[i], b = rota[(i + 1) % k];
                int aumento = t->dist[a][u] + t->dist[u][b] - t->dist[a][b];
                if (aumento < melhor_aumento) {
                    melhor_aumento = aumento;
                    melhor_pos = i + 1;
                }
            }
        }

        for (int i = k; i > melhor_pos; i--)
            rota[i] = rota[i - 1];
        rota[melhor_pos] = u;
        na_rota[u] = 1;
        k++;

        for (int v = 0; v < n; v++)
            if (!na_rota[v] && t->dist[u][v] < dist_rota[v])
                dist_rota[v] = t->dist[u][v];
    }
    return tsp_custo_rota(t, rota);
}

/* ------------------------------------------------------------------ */
/* Redução Ciclo Hamiltoniano -> TSP                                   */
/* ------------------------------------------------------------------ */

void grafo_para_tsp(const Grafo *g, TSP *t) {
    tsp_inicializar(t, g->n);
    for (int u = 0; u < g->n; u++)
        for (int v = 0; v < g->n; v++)
            if (u != v)
                t->dist[u][v] = g->adj[u][v] ? 1 : 2;
}
