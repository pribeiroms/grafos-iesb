#ifndef BUSCA_PROFUNDIDADE_H
#define BUSCA_PROFUNDIDADE_H

#include "busca_largura.h"

/* Pilha (LIFO) para DFS iterativa. */
typedef struct {
    int *dados;
    int topo, capacidade;
} Pilha;

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int *tempo,
                   int *entrada, int *saida, int *pred);
void dfs_iterativa(GrafoLista *g, int origem, int *visitado);
int contar_componentes(GrafoLista *g);
int tem_ciclo(GrafoLista *g);

#endif
