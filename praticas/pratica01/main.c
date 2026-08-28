#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"
#include "grafo_matriz.h"

int main(void) {
    // cria as duas representacoes com cinco vertices
    GrafoMatriz *matriz = criar_grafo_matriz(5);
    GrafoLista *lista = criar_grafo_lista(5);

    if (matriz == NULL || lista == NULL) {
        fprintf(stderr, "erro ao criar os grafos.\n");
        liberar_grafo_matriz(matriz);
        liberar_grafo_lista(lista);
        return EXIT_FAILURE;
    }

    // define as arestas usadas nos testes
    int arestas[][2] = {{0, 1}, {0, 2}, {1, 3}, {2, 4}};
    int quantidade = (int)(sizeof(arestas) / sizeof(arestas[0]));
    for (int i = 0; i < quantidade; i++) {
        inserir_aresta_matriz(matriz, arestas[i][0], arestas[i][1]);
        inserir_aresta_lista(lista, arestas[i][0], arestas[i][1]);
    }

    // exibe as duas representacoes
    printf("matriz de adjacencia:\n");
    exibir_grafo_matriz(matriz);
    printf("\nlista de adjacencia:\n");
    exibir_grafo_lista(lista);

    // testa grau e adjacencia
    printf("\ngrau do vertice 0: matriz=%d, lista=%d\n",
           grau_matriz(matriz, 0), grau_lista(lista, 0));
    printf("vertices 0 e 2 sao adjacentes? matriz=%s, lista=%s\n",
           sao_adjacentes_matriz(matriz, 0, 2) ? "sim" : "nao",
           sao_adjacentes_lista(lista, 0, 2) ? "sim" : "nao");

    // testa a remocao de uma aresta
    remover_aresta_matriz(matriz, 0, 2);
    remover_aresta_lista(lista, 0, 2);
    printf("depois da remocao, 0 e 2 sao adjacentes? matriz=%s, lista=%s\n",
           sao_adjacentes_matriz(matriz, 0, 2) ? "sim" : "nao",
           sao_adjacentes_lista(lista, 0, 2) ? "sim" : "nao");

    // libera toda a memoria alocada
    liberar_grafo_matriz(matriz);
    liberar_grafo_lista(lista);
    return EXIT_SUCCESS;
}
