#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

static void exibir(const char *nome, const int *ordem, int tamanho) {
    printf("%s:", nome);
    for (int i = 0; i < tamanho; i++) printf(" %d", ordem[i]);
    putchar('\n');
}

int main(void) {
    GrafoLista *g = criar_grafo_lista(7);
    if (g == NULL) return EXIT_FAILURE;
    const int arestas[][2] = {{5, 2}, {5, 0}, {4, 0}, {4, 1}, {2, 3}, {3, 1}};
    for (size_t i = 0; i < sizeof(arestas) / sizeof(arestas[0]); i++) {
        if (!inserir_aresta_lista(g, arestas[i][0], arestas[i][1])) {
            liberar_grafo_lista(g);
            return EXIT_FAILURE;
        }
    }
    int tamanho_kahn, tamanho_dfs;
    int *kahn = ordenacao_topologica_kahn(g, &tamanho_kahn);
    int *dfs = ordenacao_topologica_dfs(g, &tamanho_dfs);
    if (kahn == NULL || dfs == NULL || eh_dag(g) != 1) {
        fprintf(stderr, "Erro ao ordenar o DAG.\n");
        free(kahn); free(dfs); liberar_grafo_lista(g);
        return EXIT_FAILURE;
    }
    puts("DAG com 7 vertices (vertice 6 isolado):");
    exibir("Kahn", kahn, tamanho_kahn);
    exibir("DFS", dfs, tamanho_dfs);
    free(kahn); free(dfs);

    /* Fecha o ciclo dirigido 1 -> 5 -> 2 -> 3 -> 1. */
    if (!inserir_aresta_lista(g, 1, 5)) {
        liberar_grafo_lista(g);
        return EXIT_FAILURE;
    }
    kahn = ordenacao_topologica_kahn(g, &tamanho_kahn);
    dfs = ordenacao_topologica_dfs(g, &tamanho_dfs);
    int ciclo_detectado = kahn == NULL && dfs == NULL &&
                         tamanho_kahn == 0 && tamanho_dfs == 0 && eh_dag(g) == 0;
    puts(ciclo_detectado ? "Ciclo detectado: ordenacao impossivel nos dois algoritmos."
                        : "Erro na deteccao do ciclo.");
    free(kahn); free(dfs); liberar_grafo_lista(g);
    return ciclo_detectado ? EXIT_SUCCESS : EXIT_FAILURE;
}
