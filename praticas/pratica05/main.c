#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

static void exibir(const char *nome, const int *cor, int n, int num_cores) {
    printf("%s (%d cores):", nome, num_cores);
    for (int u = 0; u < n; u++) printf(" v%d=%d", u, cor[u]);
    putchar('\n');
}

static int demonstrar(const char *nome, int n, const int arestas[][2], int m) {
    GrafoLista *g = criar_grafo_lista(n);
    if (g == NULL) return 0;
    for (int i = 0; i < m; i++) {
        if (!inserir_aresta_lista(g, arestas[i][0], arestas[i][1])) {
            liberar_grafo_lista(g);
            return 0;
        }
    }
    int ng, nw;
    int *gulosa = coloracao_gulosa(g, &ng);
    int *wp = coloracao_welsh_powell(g, &nw);
    int bipartido = eh_bipartido(g);
    if (gulosa == NULL || wp == NULL || bipartido < 0) {
        free(gulosa); free(wp); liberar_grafo_lista(g);
        return 0;
    }
    printf("\n%s\n", nome);
    exibir("Gulosa", gulosa, n, ng);
    exibir("Welsh-Powell", wp, n, nw);
    printf("Bipartido: %s\n", bipartido ? "sim" : "nao");
    free(gulosa); free(wp); liberar_grafo_lista(g);
    return 1;
}

int main(void) {
    const int caminho[][2] = {{0,2},{2,3},{3,1}};
    const int par[][2] = {{0,1},{1,2},{2,3},{3,0}};
    const int impar[][2] = {{0,1},{1,2},{2,3},{3,4},{4,0}};
    const int k4[][2] = {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}};
    const int desconexo[][2] = {{0,1},{2,3},{3,4},{4,2}};
    puts("Cores numeradas a partir de 0. Heuristicas nao garantem o minimo.");
    if (!demonstrar("Caminho: efeito da ordem dos vertices", 4, caminho, 3) ||
        !demonstrar("Ciclo par", 4, par, 4) ||
        !demonstrar("Ciclo impar", 5, impar, 5) ||
        !demonstrar("Completo K4", 4, k4, 6) ||
        !demonstrar("Desconexo com triangulo e vertice isolado", 6, desconexo, 4) ||
        !demonstrar("Vertices isolados", 3, NULL, 0) ||
        !demonstrar("Grafo vazio", 0, NULL, 0)) {
        fprintf(stderr, "Erro ao executar a demonstracao.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
