#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

typedef struct { int u, v, peso; } Aresta;

static int demonstrar(const char *nome, int n, const Aresta *arestas, int m,
                      int origem) {
    GrafoListaPonderado *g = criar_grafo_lista_ponderado(n);
    int *dist = malloc((size_t)n * sizeof(*dist));
    int *pred = malloc((size_t)n * sizeof(*pred));
    int ok = g != NULL && dist != NULL && pred != NULL;
    for (int i = 0; ok && i < m; i++)
        ok = inserir_aresta_ponderada(g, arestas[i].u, arestas[i].v, arestas[i].peso);

    if (ok) ok = dijkstra(g, origem, dist, pred) == 1;
    if (ok) {
        printf("\n%s (origem %d)\n", nome, origem);
        for (int v = 0; v < n; v++) {
            if (dist[v] == INFINITO) printf("  dist[%d] = infinito | ", v);
            else printf("  dist[%d] = %d | ", v, dist[v]);
            if (reconstruir_caminho(origem, v, pred) < 0) ok = 0;
        }
    }
    free(dist); free(pred); liberar_grafo_lista_ponderado(g);
    return ok;
}

int main(void) {
    /* Exemplo classico com ciclos e mais de um caminho ate cada vertice. */
    const Aresta classico[] = {
        {0,1,10}, {0,3,5}, {1,2,1}, {1,3,2}, {3,1,3},
        {3,2,9}, {3,4,2}, {2,4,4}, {4,2,6}, {4,0,7}
    };
    /* O vertice 4 nao e alcancavel a partir de 0; 0 -> 2 tem peso zero. */
    const Aresta com_inalcancavel[] = {
        {0,1,4}, {0,2,0}, {2,1,1}, {1,3,2}, {4,0,1}
    };

    if (!demonstrar("Grafo com ciclos", 5, classico, 10, 0) ||
        !demonstrar("Vertice inalcancavel e aresta de peso zero", 5,
                    com_inalcancavel, 5, 0)) {
        fprintf(stderr, "Erro ao executar a demonstracao.\n");
        return EXIT_FAILURE;
    }

    GrafoListaPonderado *g = criar_grafo_lista_ponderado(2);
    if (g == NULL) return EXIT_FAILURE;
    printf("\nInserir aresta com peso -1: %s\n",
           inserir_aresta_ponderada(g, 0, 1, -1) ? "aceita" : "recusada");
    liberar_grafo_lista_ponderado(g);
    return EXIT_SUCCESS;
}
