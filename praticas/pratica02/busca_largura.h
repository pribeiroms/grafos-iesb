#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

/* Fila (FIFO) para BFS. */
typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

GrafoLista *criar_grafo_lista(int n);
int inserir_aresta_lista(GrafoLista *g, int u, int v);
void liberar_grafo_lista(GrafoLista *g);

void bfs(GrafoLista *g, int origem, int *dist, int *pred);
int eh_bipartido(GrafoLista *g);

#endif
