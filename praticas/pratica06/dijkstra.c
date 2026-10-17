#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

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
    if (g == NULL || u < 0 || v < 0 || u >= g->n || v >= g->n || peso < 0)
        return 0;
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

MinHeap *criar_min_heap(int capacidade) {
    if (capacidade < 0) return NULL;
    size_t tam = capacidade > 0 ? (size_t)capacidade : 1;
    MinHeap *h = malloc(sizeof(*h));
    if (h == NULL) return NULL;
    h->dados = malloc(tam * sizeof(*h->dados));
    h->posicao = malloc(tam * sizeof(*h->posicao));
    if (h->dados == NULL || h->posicao == NULL) {
        free(h->dados); free(h->posicao); free(h);
        return NULL;
    }
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int v = 0; v < capacidade; v++) h->posicao[v] = -1;
    return h;
}

void liberar_min_heap(MinHeap *h) {
    if (h == NULL) return;
    free(h->dados); free(h->posicao); free(h);
}

int heap_vazio(const MinHeap *h) {
    return h == NULL || h->tamanho == 0;
}

/* Troca dois nos e atualiza posicao[] dos dois vertices. */
static void trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].distancia <= h->dados[i].distancia) break;
        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    for (;;) {
        int menor = i, esq = 2 * i + 1, dir = 2 * i + 2;
        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia)
            menor = esq;
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia)
            menor = dir;
        if (menor == i) return;
        trocar(h, i, menor);
        i = menor;
    }
}

int inserir_heap(MinHeap *h, int vertice, int distancia) {
    if (h == NULL || vertice < 0 || vertice >= h->capacidade ||
        h->posicao[vertice] != -1)
        return 0;
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;
    subir(h, i);
    return 1;
}

int diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    if (h == NULL || vertice < 0 || vertice >= h->capacidade) return 0;
    int i = h->posicao[vertice];
    if (i == -1 || nova_distancia > h->dados[i].distancia) return 0;
    h->dados[i].distancia = nova_distancia;
    subir(h, i);
    return 1;
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap minimo = { -1, INFINITO };
    if (heap_vazio(h)) return minimo;
    minimo = h->dados[0];
    h->tamanho--;
    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
        descer(h, 0);
    }
    h->posicao[minimo.vertice] = -1;
    return minimo;
}

int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (g == NULL || dist == NULL || pred == NULL || origem < 0 ||
        origem >= g->n)
        return -1;
    MinHeap *h = criar_min_heap(g->n);
    if (h == NULL) return -1;
    for (int v = 0; v < g->n; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }
    dist[origem] = 0;
    inserir_heap(h, origem, 0);
    /* So entram no heap vertices ja alcancados; cada um sai uma unica vez. */
    while (!heap_vazio(h)) {
        int u = extrair_minimo(h).vertice;
        for (No *no = g->adj[u]; no != NULL; no = no->prox) {
            int v = no->destino;
            if (dist[v] != INFINITO && dist[u] + no->peso >= dist[v]) continue;
            int primeira_vez = dist[v] == INFINITO;
            dist[v] = dist[u] + no->peso;
            pred[v] = u;
            if (primeira_vez) inserir_heap(h, v, dist[v]);
            else diminuir_chave(h, v, dist[v]);
        }
    }
    liberar_min_heap(h);
    return 1;
}

static int imprimir_ate(int v, const int *pred) {
    int tamanho = 1;
    if (pred[v] != -1) {
        tamanho += imprimir_ate(pred[v], pred);
        printf(" -> ");
    }
    printf("%d", v);
    return tamanho;
}

int reconstruir_caminho(int origem, int destino, int *pred) {
    if (pred == NULL || origem < 0 || destino < 0) return -1;
    if (destino != origem && pred[destino] == -1) {
        printf("Sem caminho de %d para %d\n", origem, destino);
        return 0;
    }
    printf("Caminho de %d para %d: ", origem, destino);
    int tamanho = imprimir_ate(destino, pred);
    putchar('\n');
    return tamanho;
}
