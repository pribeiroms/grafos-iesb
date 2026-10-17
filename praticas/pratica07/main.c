#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

typedef struct { int u, v, peso; } Aresta;

static void imprimir_caminho(int v, const int *pred) {
    if (pred[v] != -1) {
        imprimir_caminho(pred[v], pred);
        printf(" -> ");
    }
    printf("%d", v);
}

static void exibir_distancias(int n, int origem, const int *dist, const int *pred) {
    for (int v = 0; v < n; v++) {
        printf("  %d -> %d: ", origem, v);
        if (dist[v] == INFINITO) {
            puts("inalcancavel");
            continue;
        }
        printf("dist = %d, caminho: ", dist[v]);
        imprimir_caminho(v, pred);
        putchar('\n');
    }
}

static int demonstrar(const char *nome, int n, const Aresta *arestas, int m,
                      int origem) {
    GrafoListaPonderado *g = criar_grafo_lista_ponderado(n);
    int *dist = malloc((size_t)n * sizeof(*dist));
    int *pred = malloc((size_t)n * sizeof(*pred));
    int ok = g != NULL && dist != NULL && pred != NULL;
    for (int i = 0; ok && i < m; i++)
        ok = inserir_aresta_ponderada(g, arestas[i].u, arestas[i].v, arestas[i].peso);

    int resultado = ok ? bellman_ford(g, origem, dist, pred) : -1;
    if (resultado >= 0) {
        printf("\n%s (origem %d)\n", nome, origem);
        if (resultado == 1) {
            exibir_distancias(n, origem, dist, pred);
        } else if (reconstruir_ciclo_negativo(g, pred) <= 0) {
            resultado = -1;
        }
    }
    free(dist); free(pred); liberar_grafo_lista_ponderado(g);
    return resultado >= 0;
}

int main(void) {
    /* Exemplo classico: pesos negativos sem ciclo negativo. */
    const Aresta sem_ciclo[] = {
        {0,1,6}, {0,3,7}, {1,2,5}, {1,3,8}, {1,4,-4},
        {2,1,-2}, {3,2,-3}, {3,4,9}, {4,0,2}, {4,2,7}
    };
    /* 1 -> 2 -> 3 -> 1 custa 1 - 4 + 2 = -1. */
    const Aresta com_ciclo[] = {
        {0,1,4}, {1,2,1}, {2,3,-4}, {3,1,2}, {3,4,3}
    };
    /* O ciclo 2 <-> 3 e negativo, mas nao e alcancavel a partir de 0. */
    const Aresta ciclo_inalcancavel[] = {
        {0,1,5}, {2,3,-3}, {3,2,1}, {3,1,1}
    };
    const Aresta laco_negativo[] = {
        {0,1,2}, {1,1,-1}, {1,2,3}
    };

    if (!demonstrar("Pesos negativos sem ciclo negativo", 5, sem_ciclo, 10, 0) ||
        !demonstrar("Ciclo negativo alcancavel", 5, com_ciclo, 5, 0) ||
        !demonstrar("Ciclo negativo inalcancavel", 4, ciclo_inalcancavel, 4, 0) ||
        !demonstrar("Laco negativo", 3, laco_negativo, 3, 0)) {
        fprintf(stderr, "Erro ao executar a demonstracao.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
