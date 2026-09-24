#ifndef COLORACAO_H
#define COLORACAO_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

/* Grafo simples nao direcionado, sem lacos ou arestas paralelas. */
typedef struct {
    int n;
    No **adj;
} GrafoLista;

GrafoLista *criar_grafo_lista(int n);
int inserir_aresta_lista(GrafoLista *g, int u, int v);
void liberar_grafo_lista(GrafoLista *g);

/* Retornam cor[n], com cores numeradas a partir de zero; liberar com free().
 * NULL e *num_cores = -1 indicam erro. Grafo vazio retorna array liberavel
 * e zero cores. As heuristicas nao garantem o numero cromatico minimo.
 * Gulosa: ordem 0..n-1. Welsh-Powell: grau decrescente, desempate por indice.
 */
int *coloracao_gulosa(GrafoLista *g, int *num_cores);
int *coloracao_welsh_powell(GrafoLista *g, int *num_cores);

/* BFS em todos os componentes: 1 bipartido, 0 nao bipartido, -1 erro.
 * Grafos sem arestas tambem sao bipartidos (usam no maximo duas cores).
 */
int eh_bipartido(GrafoLista *g);

#endif
