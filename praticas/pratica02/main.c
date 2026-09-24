#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"
#include "busca_profundidade.h"

int main(void) {
    GrafoLista *g = criar_grafo_lista(7);
    if (g == NULL) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return EXIT_FAILURE;
    }

    const int arestas[][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {4, 5}};
    const int quantidade = (int)(sizeof(arestas) / sizeof(arestas[0]));
    for (int i = 0; i < quantidade; i++) {
        if (!inserir_aresta_lista(g, arestas[i][0], arestas[i][1])) {
            fprintf(stderr, "Erro ao inserir uma aresta.\n");
            liberar_grafo_lista(g);
            return EXIT_FAILURE;
        }
    }

    int *dist = malloc((size_t)g->n * sizeof(*dist));
    int *pred = malloc((size_t)g->n * sizeof(*pred));
    int *visitado = calloc((size_t)g->n, sizeof(*visitado));
    int *entrada = calloc((size_t)g->n, sizeof(*entrada));
    int *saida = calloc((size_t)g->n, sizeof(*saida));
    if (dist == NULL || pred == NULL || visitado == NULL ||
        entrada == NULL || saida == NULL) {
        fprintf(stderr, "Erro de memoria.\n");
        free(dist); free(pred); free(visitado); free(entrada); free(saida);
        liberar_grafo_lista(g);
        return EXIT_FAILURE;
    }

    bfs(g, 0, dist, pred);
    printf("BFS a partir do vertice 0:\n");
    for (int i = 0; i < g->n; i++)
        printf("vertice %d: distancia=%d, predecessor=%d\n", i, dist[i], pred[i]);

    int tempo = 0;
    for (int i = 0; i < g->n; i++) pred[i] = -1;
    for (int i = 0; i < g->n; i++)
        if (!visitado[i]) dfs_recursiva(g, i, visitado, &tempo, entrada, saida, pred);

    printf("\nDFS recursiva:\n");
    for (int i = 0; i < g->n; i++)
        printf("vertice %d: entrada=%d, saida=%d, predecessor=%d\n",
               i, entrada[i], saida[i], pred[i]);

    printf("\nComponentes conexos: %d\n", contar_componentes(g));
    printf("Tem ciclo: %s\n", tem_ciclo(g) ? "sim" : "nao");
    printf("Eh bipartido: %s\n", eh_bipartido(g) ? "sim" : "nao");

    free(dist); free(pred); free(visitado); free(entrada); free(saida);
    liberar_grafo_lista(g);
    return EXIT_SUCCESS;
}
