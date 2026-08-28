#ifndef GRAFO_LISTA_H
#define GRAFO_LISTA_H

// estrutura obrigatoria de cada no da lista
typedef struct No {
    int destino;
    struct No *prox;
} No;

// estrutura obrigatoria da lista de adjacencia
typedef struct {
    int n;
    No **adj;
} GrafoLista;

// operacoes disponiveis para a lista de adjacencia
GrafoLista *criar_grafo_lista(int n);
int inserir_aresta_lista(GrafoLista *grafo, int u, int v);
int remover_aresta_lista(GrafoLista *grafo, int u, int v);
int grau_lista(const GrafoLista *grafo, int vertice);
int sao_adjacentes_lista(const GrafoLista *grafo, int u, int v);
void exibir_grafo_lista(const GrafoLista *grafo);
void liberar_grafo_lista(GrafoLista *grafo);

#endif
