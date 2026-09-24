#include <stdlib.h>
#include "busca_largura.h"

static int vertice_valido(const GrafoLista *g, int v) {
    return g != NULL && v >= 0 && v < g->n;
}

GrafoLista *criar_grafo_lista(int n) {
    if (n <= 0) return NULL;
    GrafoLista *g = malloc(sizeof(*g));
    if (g == NULL) return NULL;
    g->n = n;
    g->adj = calloc((size_t)n, sizeof(*g->adj));
    if (g->adj == NULL) {
        free(g);
        return NULL;
    }
    return g;
}

int inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (!vertice_valido(g, u) || !vertice_valido(g, v) || u == v) return 0;
    No *atual = g->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) return 1;
        atual = atual->prox;
    }

    No *uv = malloc(sizeof(*uv));
    No *vu = malloc(sizeof(*vu));
    if (uv == NULL || vu == NULL) {
        free(uv);
        free(vu);
        return 0;
    }
    uv->destino = v;
    uv->prox = g->adj[u];
    vu->destino = u;
    vu->prox = g->adj[v];
    g->adj[u] = uv;
    g->adj[v] = vu;
    return 1;
}

void liberar_grafo_lista(GrafoLista *g) {
    if (g == NULL) return;
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(g->adj);
    free(g);
}

static int iniciar_fila(Fila *f, int capacidade) {
    f->dados = malloc((size_t)capacidade * sizeof(*f->dados));
    f->capacidade = capacidade;
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    return f->dados != NULL;
}

static void enfileirar(Fila *f, int valor) {
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

static int desenfileirar(Fila *f) {
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

void bfs(GrafoLista *g, int origem, int *dist, int *pred) {
    if (!vertice_valido(g, origem) || dist == NULL || pred == NULL) return;
    for (int i = 0; i < g->n; i++) {
        dist[i] = -1;
        pred[i] = -1;
    }

    Fila fila;
    if (!iniciar_fila(&fila, g->n)) return;
    dist[origem] = 0;
    enfileirar(&fila, origem);

    while (fila.tamanho > 0) {
        int u = desenfileirar(&fila);
        for (No *no = g->adj[u]; no != NULL; no = no->prox) {
            int v = no->destino;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                pred[v] = u;
                enfileirar(&fila, v);
            }
        }
    }
    free(fila.dados);
}

int eh_bipartido(GrafoLista *g) {
    if (g == NULL) return 0;
    int *cor = malloc((size_t)g->n * sizeof(*cor));
    Fila fila;
    if (cor == NULL || !iniciar_fila(&fila, g->n)) {
        free(cor);
        return 0;
    }
    for (int i = 0; i < g->n; i++) cor[i] = -1;

    for (int inicio = 0; inicio < g->n; inicio++) {
        if (cor[inicio] != -1) continue;
        cor[inicio] = 0;
        enfileirar(&fila, inicio);
        while (fila.tamanho > 0) {
            int u = desenfileirar(&fila);
            for (No *no = g->adj[u]; no != NULL; no = no->prox) {
                int v = no->destino;
                if (cor[v] == -1) {
                    cor[v] = 1 - cor[u];
                    enfileirar(&fila, v);
                } else if (cor[v] == cor[u]) {
                    free(cor);
                    free(fila.dados);
                    return 0;
                }
            }
        }
    }
    free(cor);
    free(fila.dados);
    return 1;
}
