#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"
#include "planaridade.h"

static int demonstrar(const char *nome, int n, const int arestas[][2], int m) {
    GrafoLista *g = criar_grafo_lista(n);
    if (g == NULL) return 0;
    for (int i = 0; i < m; i++) {
        if (!inserir_aresta_lista(g, arestas[i][0], arestas[i][1])) {
            liberar_grafo_lista(g);
            return 0;
        }
    }
    int *articulacoes = calloc(n > 0 ? (size_t)n : 1, sizeof(*articulacoes));
    int quantidade;
    Ponte *pontes = detectar_pontes(g, &quantidade);
    if (articulacoes == NULL || pontes == NULL || dfs_articulacoes(g, articulacoes) < 0) {
        free(articulacoes); free(pontes); liberar_grafo_lista(g);
        return 0;
    }
    printf("\n%s\nArticulacoes:", nome);
    for (int u = 0; u < n; u++) if (articulacoes[u]) printf(" %d", u);
    printf("\nPontes (%d):", quantidade);
    for (int i = 0; i < quantidade; i++) printf(" (%d,%d)", pontes[i].u, pontes[i].v);
    printf("\nLimite de Euler: %s\n", eh_planar_euler(g) ? "satisfeito" : "violado");
    ResultadoPlanaridade resultado = verificar_planaridade(g);
    const char *descricao = resultado == PLANAR ? "planar" :
                            resultado == NAO_PLANAR ? "nao planar" :
                            resultado == PLANARIDADE_INCONCLUSIVA ?
                            "inconclusivo (mais de 10 vertices)" : "erro";
    printf("Planaridade: %s\n", descricao);
    free(articulacoes); free(pontes); liberar_grafo_lista(g);
    return resultado != PLANARIDADE_ERRO;
}

int main(void) {
    const int conectado[][2] = {{0,1},{1,2},{2,0},{1,3},{3,4}};
    const int k5[][2] = {{0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}};
    const int k33[][2] = {{0,3},{0,4},{0,5},{1,3},{1,4},{1,5},{2,3},{2,4},{2,5}};
    /* A aresta 0--3 foi substituida por 0--6--3. */
    const int subdividido[][2] = {{0,6},{6,3},{0,4},{0,5},{1,3},{1,4},{1,5},{2,3},{2,4},{2,5}};
    if (!demonstrar("Triangulo com cauda e vertice isolado", 6, conectado, 5) ||
        !demonstrar("K5", 5, k5, 10) ||
        !demonstrar("K3,3 (passa em Euler, mas nao e planar)", 6, k33, 9) ||
        !demonstrar("Subdivisao de K3,3", 7, subdividido, 10) ||
        !demonstrar("Grafo com 11 vertices: limite do teste exato", 11, conectado, 5)) {
        fprintf(stderr, "Erro ao executar a demonstracao.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
