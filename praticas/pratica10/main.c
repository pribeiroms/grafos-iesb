#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

#define REP_HEURISTICA 10000

static double ms_desde(clock_t inicio, clock_t fim) {
    return 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* ------------------------------------------------------------------ */
/* Parte 1: Caminho e Ciclo Hamiltoniano                               */
/* ------------------------------------------------------------------ */

static void testar_hamiltoniano(const char *titulo, const Grafo *g) {
    int caminho[MAX_VERTICES];
    long nos;

    printf("--- %s ---\n", titulo);
    grafo_imprimir(g);

    if (encontrar_caminho_hamiltoniano_bt(g, 0, caminho, &nos)) {
        printf("Caminho Hamiltoniano: ");
        caminho_imprimir(caminho, g->n, 0);
        printf("  valido=%s  nos=%ld\n",
               eh_caminho_hamiltoniano(g, caminho, 0) ? "sim" : "nao", nos);
    } else {
        printf("Caminho Hamiltoniano: nao existe  nos=%ld\n", nos);
    }

    if (encontrar_ciclo_hamiltoniano_bt(g, caminho, &nos)) {
        printf("Ciclo Hamiltoniano:   ");
        caminho_imprimir(caminho, g->n, 1);
        printf("  valido=%s  nos=%ld\n",
               eh_caminho_hamiltoniano(g, caminho, 1) ? "sim" : "nao", nos);
    } else {
        printf("Ciclo Hamiltoniano:   nao existe  nos=%ld\n", nos);
    }
    printf("\n");
}

static void exemplos_hamiltoniano(void) {
    Grafo g;

    printf("=== Parte 1: Caminho e Ciclo Hamiltoniano (backtracking) ===\n\n");

    /* Casa: quadrado 0-1-2-3 com telhado 4 sobre 0 e 1 -> tem ciclo */
    grafo_inicializar(&g, 5);
    grafo_adicionar_aresta(&g, 0, 1);
    grafo_adicionar_aresta(&g, 1, 2);
    grafo_adicionar_aresta(&g, 2, 3);
    grafo_adicionar_aresta(&g, 3, 0);
    grafo_adicionar_aresta(&g, 0, 4);
    grafo_adicionar_aresta(&g, 1, 4);
    testar_hamiltoniano("Grafo 'casa' (tem ciclo)", &g);

    /* Estrela com uma aresta extra: tem caminho mas não tem ciclo */
    grafo_inicializar(&g, 4);
    grafo_adicionar_aresta(&g, 0, 1);
    grafo_adicionar_aresta(&g, 0, 2);
    grafo_adicionar_aresta(&g, 0, 3);
    grafo_adicionar_aresta(&g, 1, 2);
    testar_hamiltoniano("Grafo com caminho e sem ciclo", &g);

    /* Estrela K1,3: não tem caminho nem ciclo */
    grafo_inicializar(&g, 4);
    grafo_adicionar_aresta(&g, 0, 1);
    grafo_adicionar_aresta(&g, 0, 2);
    grafo_adicionar_aresta(&g, 0, 3);
    testar_hamiltoniano("Estrela K1,3 (sem caminho e sem ciclo)", &g);
}

/* ------------------------------------------------------------------ */
/* Parte 2: Redução Ciclo Hamiltoniano -> TSP                          */
/* ------------------------------------------------------------------ */

static void exemplo_reducao(void) {
    const int tamanhos[] = {5, 6, 7, 8};
    const int qtd = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));

    printf("=== Parte 2: Reducao Ciclo Hamiltoniano -> TSP (peso 1 / 2) ===\n");
    printf("Tem ciclo hamiltoniano <=> custo otimo do TSP == n\n\n");
    printf("%4s %6s | %10s | %10s %12s | %s\n",
           "n", "m", "BT ciclo?", "TSP custo", "custo == n?", "confere");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        Grafo g;
        TSP t;
        int caminho[MAX_VERTICES], rota[MAX_VERTICES];
        long nos, perms;

        grafo_aleatorio(&g, tamanhos[i], 0.4);
        grafo_para_tsp(&g, &t);

        int tem_ciclo = encontrar_ciclo_hamiltoniano_bt(&g, caminho, &nos);
        int custo = tsp_forca_bruta(&t, rota, &perms);
        int pela_reducao = (custo == g.n);

        printf("%4d %6d | %10s | %10d %12s | %s\n",
               g.n, grafo_num_arestas(&g), tem_ciclo ? "sim" : "nao",
               custo, pela_reducao ? "sim" : "nao",
               tem_ciclo == pela_reducao ? "OK" : "ERRO");
    }
    printf("\n");
}

/* ------------------------------------------------------------------ */
/* Parte 3: Experimento TSP                                            */
/* ------------------------------------------------------------------ */

static void experimento_tsp(void) {
    const int tamanhos[] = {5, 8, 10, 12};
    const int qtd = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));
    const int peso_max = 100;

    printf("=== Parte 3: Experimento TSP (pesos aleatorios 1..%d) ===\n", peso_max);
    printf("%4s | %6s %10s %12s | %6s %10s %7s | %6s %10s %7s\n",
           "n", "BF", "perms", "tempo(ms)",
           "VMP", "tempo(ms)", "gap%",
           "Ins", "tempo(ms)", "gap%");
    printf("------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < qtd; i++) {
        int n = tamanhos[i];
        TSP t;
        int r_bf[MAX_VERTICES], r_vmp[MAX_VERTICES], r_ins[MAX_VERTICES];
        long perms = 0;
        int c_bf = 0, c_vmp = 0, c_ins = 0;

        tsp_aleatorio(&t, n, peso_max);

        /* Força bruta: repete mais vezes nas instâncias pequenas */
        int rep_bf = n <= 8 ? 100 : 1;
        clock_t ini = clock();
        for (int r = 0; r < rep_bf; r++)
            c_bf = tsp_forca_bruta(&t, r_bf, &perms);
        double tempo_bf = ms_desde(ini, clock()) / rep_bf;

        ini = clock();
        for (int r = 0; r < REP_HEURISTICA; r++)
            c_vmp = tsp_vizinho_mais_proximo(&t, 0, r_vmp);
        double tempo_vmp = ms_desde(ini, clock()) / REP_HEURISTICA;

        ini = clock();
        for (int r = 0; r < REP_HEURISTICA; r++)
            c_ins = tsp_insercao(&t, r_ins);
        double tempo_ins = ms_desde(ini, clock()) / REP_HEURISTICA;

        if (tsp_custo_rota(&t, r_bf) != c_bf ||
            tsp_custo_rota(&t, r_vmp) != c_vmp ||
            tsp_custo_rota(&t, r_ins) != c_ins) {
            printf("ERRO: custo inconsistente para n = %d\n", n);
            continue;
        }

        double gap_vmp = 100.0 * (c_vmp - c_bf) / c_bf;
        double gap_ins = 100.0 * (c_ins - c_bf) / c_bf;

        printf("%4d | %6d %10ld %12.4f | %6d %10.5f %7.1f | %6d %10.5f %7.1f\n",
               n, c_bf, perms, tempo_bf,
               c_vmp, tempo_vmp, gap_vmp,
               c_ins, tempo_ins, gap_ins);

        if (n == tamanhos[qtd - 1]) {
            printf("\nRotas para n = %d:\n", n);
            printf("  BF:  "); tsp_imprimir_rota(r_bf, n);  printf("  custo %d\n", c_bf);
            printf("  VMP: "); tsp_imprimir_rota(r_vmp, n); printf("  custo %d\n", c_vmp);
            printf("  Ins: "); tsp_imprimir_rota(r_ins, n); printf("  custo %d\n", c_ins);
        }
    }

    printf("\nBF  = forca bruta, (n-1)! permutacoes com origem fixa (otimo)\n");
    printf("VMP = vizinho mais proximo | Ins = insercao do mais proximo\n");
    printf("gap%% = (custo heuristica - otimo) / otimo * 100\n");
}

int main(void) {
    srand(42);
    exemplos_hamiltoniano();
    exemplo_reducao();
    experimento_tsp();
    return 0;
}
