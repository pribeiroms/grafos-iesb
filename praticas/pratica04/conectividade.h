#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

/* Grafo simples nao direcionado: sem lacos ou arestas paralelas. */
typedef struct {
    int n;
    No **adj;
} GrafoLista;

typedef struct { int u, v; } Ponte;

GrafoLista *criar_grafo_lista(int n);
int inserir_aresta_lista(GrafoLista *g, int u, int v);
void liberar_grafo_lista(GrafoLista *g);

/* Preenche articulacao[n] com 0/1. Retorna a quantidade ou -1 em erro.
 * Percorre todos os componentes usando descoberta[] e low[] de Tarjan.
 */
int dfs_articulacoes(GrafoLista *g, int *articulacao);
/* Retorna array liberavel com free(), inclusive quando nao ha pontes.
 * Em erro retorna NULL e define *quantidade = -1.
 */
Ponte *detectar_pontes(GrafoLista *g, int *quantidade);

#endif
