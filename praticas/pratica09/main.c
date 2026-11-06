#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

#define REPETICOES 1000

static double ms_desde(clock_t inicio, clock_t fim) {
    return 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
}

static void exemplo_fixo(void) {
    Grafo g;
    int cob_bt[MAX_VERTICES], cob_gu[MAX_VERTICES];
    long nos;

    /*
     * Exemplo onde a gulosa não é ótima:
     *   o vértice 0 tem o maior grau, mas a cobertura mínima
     *   é {1, 2, 3} (sem o vértice 0).
     */
    grafo_inicializar(&g, 7);
    grafo_adicionar_aresta(&g, 0, 1);
    grafo_adicionar_aresta(&g, 0, 2);
    grafo_adicionar_aresta(&g, 0, 3);
    grafo_adicionar_aresta(&g, 1, 4);
    grafo_adicionar_aresta(&g, 1, 5);
    grafo_adicionar_aresta(&g, 2, 5);
    grafo_adicionar_aresta(&g, 2, 6);
    grafo_adicionar_aresta(&g, 3, 4);
    grafo_adicionar_aresta(&g, 3, 6);

    printf("=== Exemplo fixo ===\n");
    grafo_imprimir(&g);

    int t_bt = cobertura_minima_bt(&g, cob_bt, &nos);
    int t_gu = cobertura_gulosa(&g, cob_gu);

    printf("Backtracking: tamanho %d ", t_bt);
    cobertura_imprimir(cob_bt, g.n);
    printf("  valida=%s  nos=%ld\n", eh_cobertura(&g, cob_bt) ? "sim" : "nao", nos);

    printf("Gulosa:       tamanho %d ", t_gu);
    cobertura_imprimir(cob_gu, g.n);
    printf("  valida=%s\n\n", eh_cobertura(&g, cob_gu) ? "sim" : "nao");
}

static void experimento(void) {
    const int tamanhos[] = {5, 8, 10, 12, 15};
    const int qtd = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));
    const double prob = 0.5;

    printf("=== Experimento (grafos aleatorios, p = %.2f, media de %d execucoes) ===\n",
           prob, REPETICOES);
    printf("%4s %6s | %8s %12s %12s | %8s %12s | %6s\n",
           "n", "m", "BT tam", "BT nos", "BT tempo(ms)",
           "Gul tam", "Gul tempo(ms)", "razao");
    printf("------------------------------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        int n = tamanhos[i];
        Grafo g;
        int cob_bt[MAX_VERTICES], cob_gu[MAX_VERTICES];
        long nos = 0;
        int t_bt = 0, t_gu = 0;

        grafo_aleatorio(&g, n, prob);

        clock_t ini = clock();
        for (int r = 0; r < REPETICOES; r++)
            t_bt = cobertura_minima_bt(&g, cob_bt, &nos);
        double tempo_bt = ms_desde(ini, clock()) / REPETICOES;

        ini = clock();
        for (int r = 0; r < REPETICOES; r++)
            t_gu = cobertura_gulosa(&g, cob_gu);
        double tempo_gu = ms_desde(ini, clock()) / REPETICOES;

        if (!eh_cobertura(&g, cob_bt) || !eh_cobertura(&g, cob_gu)) {
            printf("ERRO: solucao invalida para n = %d\n", n);
            continue;
        }

        double razao = t_bt > 0 ? (double)t_gu / t_bt : 1.0;

        printf("%4d %6d | %8d %12ld %12.4f | %8d %12.4f | %6.2f\n",
               n, grafo_num_arestas(&g), t_bt, nos, tempo_bt,
               t_gu, tempo_gu, razao);
    }

    printf("\nrazao = tamanho guloso / tamanho otimo (1.00 = guloso encontrou o otimo)\n");
}

int main(void) {
    srand(42);
    exemplo_fixo();
    experimento();
    return 0;
}
