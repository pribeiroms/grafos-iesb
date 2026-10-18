#include <stdlib.h>
#include <string.h>
#include "agm.h"

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
    No *uv = malloc(sizeof(*uv)), *vu = malloc(sizeof(*vu));
    if (uv == NULL || vu == NULL) { free(uv); free(vu); return 0; }
    uv->destino = v; uv->peso = peso; uv->prox = g->adj[u];
    g->adj[u] = uv;
    vu->destino = u; vu->peso = peso; vu->prox = g->adj[v];
    g->adj[v] = vu;
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

DSU *criar_dsu(int n) {
    if (n < 0) return NULL;
    size_t tam = n > 0 ? (size_t)n : 1;
    DSU *d = malloc(sizeof(*d));
    if (d == NULL) return NULL;
    d->pai = malloc(tam * sizeof(*d->pai));
    d->rank = calloc(tam, sizeof(*d->rank));
    if (d->pai == NULL || d->rank == NULL) {
        free(d->pai); free(d->rank); free(d);
        return NULL;
    }
    d->n = n;
    for (int x = 0; x < n; x++) d->pai[x] = x;
    return d;
}

void liberar_dsu(DSU *d) {
    if (d == NULL) return;
    free(d->pai); free(d->rank); free(d);
}

int find(DSU *d, int x) {
    if (d == NULL || x < 0 || x >= d->n) return -1;
    /* Com uniao por rank a altura e O(log n), entao a recursao e rasa. */
    if (d->pai[x] != x) d->pai[x] = find(d, d->pai[x]);
    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int rx = find(d, x), ry = find(d, y);
    if (rx < 0 || ry < 0 || rx == ry) return 0;
    if (d->rank[rx] < d->rank[ry]) { int t = rx; rx = ry; ry = t; }
    d->pai[ry] = rx;
    if (d->rank[rx] == d->rank[ry]) d->rank[rx]++;
    return 1;
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
        if (h->dados[pai].chave <= h->dados[i].chave) break;
        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    for (;;) {
        int menor = i, esq = 2 * i + 1, dir = 2 * i + 2;
        if (esq < h->tamanho && h->dados[esq].chave < h->dados[menor].chave)
            menor = esq;
        if (dir < h->tamanho && h->dados[dir].chave < h->dados[menor].chave)
            menor = dir;
        if (menor == i) return;
        trocar(h, i, menor);
        i = menor;
    }
}

int inserir_heap(MinHeap *h, int vertice, int chave) {
    if (h == NULL || vertice < 0 || vertice >= h->capacidade ||
        h->posicao[vertice] != -1)
        return 0;
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].chave = chave;
    h->posicao[vertice] = i;
    subir(h, i);
    return 1;
}

int diminuir_chave(MinHeap *h, int vertice, int nova_chave) {
    if (h == NULL || vertice < 0 || vertice >= h->capacidade) return 0;
    int i = h->posicao[vertice];
    if (i == -1 || nova_chave > h->dados[i].chave) return 0;
    h->dados[i].chave = nova_chave;
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

static int comparar_peso(const void *a, const void *b) {
    const Aresta *x = a, *y = b;
    return (x->peso > y->peso) - (x->peso < y->peso);
}

int kruskal(int n, const Aresta *arestas, int m, Aresta *agm, long long *custo) {
    if (n < 0 || m < 0 || (m > 0 && arestas == NULL) || agm == NULL ||
        custo == NULL)
        return -1;
    for (int i = 0; i < m; i++)
        if (arestas[i].u < 0 || arestas[i].v < 0 ||
            arestas[i].u >= n || arestas[i].v >= n)
            return -1;
    Aresta *ordenadas = malloc((m > 0 ? (size_t)m : 1) * sizeof(*ordenadas));
    DSU *d = criar_dsu(n);
    if (ordenadas == NULL || d == NULL) {
        free(ordenadas); liberar_dsu(d);
        return -1;
    }
    if (m > 0) memcpy(ordenadas, arestas, (size_t)m * sizeof(*ordenadas));
    qsort(ordenadas, (size_t)m, sizeof(*ordenadas), comparar_peso);

    int k = 0;
    *custo = 0;
    /* Aresta entre componentes diferentes nunca fecha ciclo. */
    for (int i = 0; i < m && k < n - 1; i++) {
        if (unir(d, ordenadas[i].u, ordenadas[i].v)) {
            agm[k++] = ordenadas[i];
            *custo += ordenadas[i].peso;
        }
    }
    free(ordenadas); liberar_dsu(d);
    return k;
}

int prim(GrafoListaPonderado *g, Aresta *agm, long long *custo) {
    if (g == NULL || agm == NULL || custo == NULL) return -1;
    size_t tam = g->n > 0 ? (size_t)g->n : 1;
    int *chave = malloc(tam * sizeof(*chave));
    int *pai = malloc(tam * sizeof(*pai));
    int *na_arvore = calloc(tam, sizeof(*na_arvore));
    MinHeap *h = criar_min_heap(g->n);
    if (chave == NULL || pai == NULL || na_arvore == NULL || h == NULL) {
        free(chave); free(pai); free(na_arvore); liberar_min_heap(h);
        return -1;
    }
    for (int v = 0; v < g->n; v++) { chave[v] = INFINITO; pai[v] = -1; }

    int k = 0;
    *custo = 0;
    for (int raiz = 0; raiz < g->n; raiz++) {
        if (na_arvore[raiz]) continue;
        /* Novo componente: a raiz entra sem aresta. */
        chave[raiz] = 0;
        inserir_heap(h, raiz, 0);
        while (!heap_vazio(h)) {
            int u = extrair_minimo(h).vertice;
            na_arvore[u] = 1;
            if (pai[u] != -1) {
                agm[k].u = pai[u]; agm[k].v = u; agm[k].peso = chave[u];
                *custo += chave[u];
                k++;
            }
            for (No *no = g->adj[u]; no != NULL; no = no->prox) {
                int v = no->destino;
                int no_heap = h->posicao[v] != -1;
                if (na_arvore[v] || (no_heap && no->peso >= chave[v])) continue;
                chave[v] = no->peso;
                pai[v] = u;
                if (!no_heap) inserir_heap(h, v, chave[v]);
                else diminuir_chave(h, v, chave[v]);
            }
        }
    }
    free(chave); free(pai); free(na_arvore); liberar_min_heap(h);
    return k;
}
