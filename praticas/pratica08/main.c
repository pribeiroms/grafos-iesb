#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

static void exibir(const char *nome, const Aresta *agm, int k, long long custo) {
    printf("%s (%d arestas, custo %lld):", nome, k, custo);
    for (int i = 0; i < k; i++)
        printf(" %d-%d(%d)", agm[i].u, agm[i].v, agm[i].peso);
    putchar('\n');
}

static int demonstrar(const char *nome, int n, const Aresta *arestas, int m) {
    GrafoListaPonderado *g = criar_grafo_lista_ponderado(n);
    Aresta *agm = malloc((n > 1 ? (size_t)n - 1 : 1) * sizeof(*agm));
    int ok = g != NULL && agm != NULL;
    for (int i = 0; ok && i < m; i++)
        ok = inserir_aresta_ponderada(g, arestas[i].u, arestas[i].v, arestas[i].peso);

    if (ok) {
        long long ck, cp;
        printf("\n%s\n", nome);
        int kk = kruskal(n, arestas, m, agm, &ck);
        if (kk >= 0) exibir("Kruskal", agm, kk, ck);
        int kp = prim(g, agm, &cp);
        if (kp >= 0) exibir("Prim", agm, kp, cp);
        ok = kk >= 0 && kp >= 0;
        if (ok && kk < n - 1)
            puts("Grafo desconexo: resultado e uma floresta geradora minima.");
    }
    free(agm); liberar_grafo_lista_ponderado(g);
    return ok;
}

int main(void) {
    /* Exemplo classico com 9 vertices: custo minimo 37. */
    const Aresta classico[] = {
        {0,1,4}, {0,7,8}, {1,2,8}, {1,7,11}, {2,3,7}, {2,5,4}, {2,8,2},
        {3,4,9}, {3,5,14}, {4,5,10}, {5,6,2}, {6,7,1}, {6,8,6}, {7,8,7}
    };
    /* Peso negativo, aresta paralela e laco (que nunca entra na arvore). */
    const Aresta especiais[] = {
        {0,1,3}, {0,1,1}, {1,2,-2}, {2,3,4}, {0,3,2}, {3,3,-5}
    };
    /* Dois componentes: {0, 1, 2} e {3, 4}. */
    const Aresta desconexo[] = {
        {0,1,5}, {1,2,3}, {0,2,1}, {3,4,7}
    };

    if (!demonstrar("Grafo classico", 9, classico, 14) ||
        !demonstrar("Peso negativo, aresta paralela e laco", 4, especiais, 6) ||
        !demonstrar("Grafo desconexo", 5, desconexo, 4)) {
        fprintf(stderr, "Erro ao executar a demonstracao.\n");
        return EXIT_FAILURE;
    }

    DSU *d = criar_dsu(5);
    if (d == NULL) return EXIT_FAILURE;
    unir(d, 0, 1); unir(d, 2, 3); unir(d, 1, 3);
    printf("\nUnion-Find: find(0) = %d, find(3) = %d, find(4) = %d\n",
           find(d, 0), find(d, 3), find(d, 4));
    liberar_dsu(d);
    return EXIT_SUCCESS;
}
