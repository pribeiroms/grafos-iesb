#ifndef DAG_H
#define DAG_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

/* Digrafo: inserir u -> v nao insere v -> u. Aceita lacos. */
GrafoLista *criar_grafo_lista(int n);
int inserir_aresta_lista(GrafoLista *g, int u, int v);
void liberar_grafo_lista(GrafoLista *g);

/* O chamador libera o array retornado com free().
 * Em ciclo, argumento invalido ou falha de memoria: NULL e tamanho = 0.
 * Grafo vazio: array liberavel nao nulo e tamanho = 0.
 */
int *ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);
int *ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);
/* Retorna 1 para DAG, 0 para ciclo e -1 para argumento/falha de memoria. */
int eh_dag(GrafoLista *g);

#endif
