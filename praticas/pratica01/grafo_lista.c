#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

// verifica se o vertice existe no grafo
static int vertice_valido(const GrafoLista *grafo, int vertice) {
    return grafo != NULL && vertice >= 0 && vertice < grafo->n;
}

// cria um novo no para uma lista encadeada
static No *criar_no(int destino, No *proximo) {
    No *novo = malloc(sizeof(No));
    if (novo != NULL) {
        novo->destino = destino;
        novo->prox = proximo;
    }
    return novo;
}

// cria o grafo e aloca o vetor de listas
GrafoLista *criar_grafo_lista(int n) {
    if (n <= 0) return NULL;
    GrafoLista *grafo = malloc(sizeof(GrafoLista));
    if (grafo == NULL) return NULL;
    grafo->n = n;
    grafo->adj = calloc(n, sizeof(No *));
    if (grafo->adj == NULL) {
        free(grafo);
        return NULL;
    }
    return grafo;
}

// procura um destino na lista do vertice de origem
int sao_adjacentes_lista(const GrafoLista *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v)) return 0;
    for (No *atual = grafo->adj[u]; atual != NULL; atual = atual->prox) {
        if (atual->destino == v) return 1;
    }
    return 0;
}

// insere a ligacao nas listas dos dois vertices
int inserir_aresta_lista(GrafoLista *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v) || u == v) return 0;
    if (sao_adjacentes_lista(grafo, u, v)) return 1;

    No *novo_u = criar_no(v, grafo->adj[u]);
    if (novo_u == NULL) return 0;
    No *novo_v = criar_no(u, grafo->adj[v]);
    if (novo_v == NULL) {
        free(novo_u);
        return 0;
    }
    grafo->adj[u] = novo_u;
    grafo->adj[v] = novo_v;
    return 1;
}

// remove um no de uma das listas encadeadas
static int remover_da_lista(No **inicio, int destino) {
    No **atual = inicio;
    while (*atual != NULL) {
        if ((*atual)->destino == destino) {
            No *removido = *atual;
            *atual = removido->prox;
            free(removido);
            return 1;
        }
        atual = &(*atual)->prox;
    }
    return 0;
}

// remove a ligacao das listas dos dois vertices
int remover_aresta_lista(GrafoLista *grafo, int u, int v) {
    if (!vertice_valido(grafo, u) || !vertice_valido(grafo, v) || u == v) return 0;
    int removeu = remover_da_lista(&grafo->adj[u], v);
    remover_da_lista(&grafo->adj[v], u);
    return removeu;
}

// conta os elementos da lista do vertice
int grau_lista(const GrafoLista *grafo, int vertice) {
    if (!vertice_valido(grafo, vertice)) return -1;
    int grau = 0;
    for (No *atual = grafo->adj[vertice]; atual != NULL; atual = atual->prox) grau++;
    return grau;
}

// mostra cada vertice e os seus vizinhos
void exibir_grafo_lista(const GrafoLista *grafo) {
    if (grafo == NULL) return;
    for (int i = 0; i < grafo->n; i++) {
        printf("%d:", i);
        for (No *atual = grafo->adj[i]; atual != NULL; atual = atual->prox) {
            printf(" %d", atual->destino);
        }
        printf("\n");
    }
}

// libera todos os nos, o vetor de listas e o grafo
void liberar_grafo_lista(GrafoLista *grafo) {
    if (grafo == NULL) return;
    for (int i = 0; i < grafo->n; i++) {
        No *atual = grafo->adj[i];
        while (atual != NULL) {
            No *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(grafo->adj);
    free(grafo);
}
