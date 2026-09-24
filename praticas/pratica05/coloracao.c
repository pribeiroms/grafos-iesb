#include <stdlib.h>
#include "coloracao.h"

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
            free(no); no = proximo;
        }
    }
    free(g->adj); free(g);
}

typedef struct { int vertice, grau; } VerticeOrdenado;

static int comparar_grau(const void *a, const void *b) {
    const VerticeOrdenado *x = a, *y = b;
    if (x->grau != y->grau) return x->grau > y->grau ? -1 : 1;
    return (x->vertice > y->vertice) - (x->vertice < y->vertice);
}

static int *colorir(GrafoLista *g, int *num_cores, int ordenar) {
    if (num_cores == NULL) return NULL;
    *num_cores = -1;
    if (g == NULL) return NULL;
    size_t capacidade = g->n > 0 ? (size_t)g->n : 1;
    int *cor = malloc(capacidade * sizeof(*cor));
    int *proibida = malloc(capacidade * sizeof(*proibida));
    VerticeOrdenado *ordem = malloc(capacidade * sizeof(*ordem));
    if (cor == NULL || proibida == NULL || ordem == NULL) {
        free(cor); free(proibida); free(ordem);
        return NULL;
    }
    for (int u = 0; u < g->n; u++) {
        cor[u] = proibida[u] = -1;
        ordem[u].vertice = u;
        ordem[u].grau = 0;
        if (ordenar)
            for (No *no = g->adj[u]; no != NULL; no = no->prox) ordem[u].grau++;
    }
    if (ordenar) qsort(ordem, (size_t)g->n, sizeof(*ordem), comparar_grau);
    *num_cores = 0;
    for (int i = 0; i < g->n; i++) {
        int u = ordem[i].vertice;
        /* A marca i evita limpar todo o array a cada vertice. */
        for (No *no = g->adj[u]; no != NULL; no = no->prox)
            if (cor[no->destino] >= 0) proibida[cor[no->destino]] = i;
        int c = 0;
        while (c < g->n && proibida[c] == i) c++;
        cor[u] = c;
        if (c + 1 > *num_cores) *num_cores = c + 1;
    }
    free(proibida); free(ordem);
    return cor;
}

int *coloracao_gulosa(GrafoLista *g, int *num_cores) {
    return colorir(g, num_cores, 0);
}

int *coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    return colorir(g, num_cores, 1);
}

int eh_bipartido(GrafoLista *g) {
    if (g == NULL) return -1;
    size_t capacidade = g->n > 0 ? (size_t)g->n : 1;
    int *cor = malloc(capacidade * sizeof(*cor));
    int *fila = malloc(capacidade * sizeof(*fila));
    if (cor == NULL || fila == NULL) { free(cor); free(fila); return -1; }
    for (int u = 0; u < g->n; u++) cor[u] = -1;
    int inicio = 0, fim = 0;
    for (int raiz = 0; raiz < g->n; raiz++) {
        if (cor[raiz] != -1) continue;
        cor[raiz] = 0;
        fila[fim++] = raiz;
        while (inicio < fim) {
            int u = fila[inicio++];
            for (No *no = g->adj[u]; no != NULL; no = no->prox) {
                int v = no->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    fila[fim++] = v;
                } else if (cor[v] == cor[u]) {
                    free(cor); free(fila);
                    return 0;
                }
            }
        }
    }
    free(cor); free(fila);
    return 1;
}
