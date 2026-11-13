#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

/* Instância do Problema do Caixeiro Viajante (TSP) simétrico */
typedef struct {
    int n;
    int dist[MAX_VERTICES][MAX_VERTICES];
} TSP;

/* Operações básicas */
void tsp_inicializar(TSP *t, int n);
void tsp_aleatorio(TSP *t, int n, int peso_max);
int  tsp_custo_rota(const TSP *t, const int *rota);
void tsp_imprimir_rota(const int *rota, int n);

/*
 * Força bruta: fixa a origem no vértice 0 e gera as (n-1)!
 * permutações dos demais vértices, guardando a de menor custo.
 * Preenche 'rota' e retorna o custo ótimo. Se 'permutacoes'
 * não for NULL, recebe o número de rotas completas avaliadas.
 */
int tsp_forca_bruta(const TSP *t, int *rota, long *permutacoes);

/*
 * Heurística do Vizinho Mais Próximo: a partir da origem,
 * vai sempre para a cidade não visitada mais próxima.
 * Preenche 'rota' e retorna o custo.
 */
int tsp_vizinho_mais_proximo(const TSP *t, int origem, int *rota);

/*
 * Heurística de Inserção do Mais Próximo: começa com o vértice 0
 * e seu vizinho mais próximo; a cada passo escolhe a cidade fora
 * da rota mais próxima de alguma cidade da rota e a insere na
 * posição que menos aumenta o custo.
 * Preenche 'rota' e retorna o custo.
 */
int tsp_insercao(const TSP *t, int *rota);

/*
 * Redução Ciclo Hamiltoniano -> TSP: peso 1 para arestas que
 * existem no grafo e peso 2 para as que não existem.
 * O grafo tem ciclo hamiltoniano se e somente se o custo ótimo
 * do TSP resultante for exatamente n.
 */
void grafo_para_tsp(const Grafo *g, TSP *t);

#endif
