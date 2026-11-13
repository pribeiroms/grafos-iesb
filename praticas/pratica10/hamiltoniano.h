#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

#define MAX_VERTICES 16

/* Grafo não orientado representado por matriz de adjacência */
typedef struct {
    int n;
    int adj[MAX_VERTICES][MAX_VERTICES];
} Grafo;

/* Operações básicas sobre o grafo */
void grafo_inicializar(Grafo *g, int n);
void grafo_adicionar_aresta(Grafo *g, int u, int v);
int  grafo_num_arestas(const Grafo *g);
void grafo_aleatorio(Grafo *g, int n, double prob);
void grafo_imprimir(const Grafo *g);

/*
 * Força bruta com backtracking: monta permutações dos vértices
 * posição a posição, só avançando quando o próximo vértice é
 * adjacente ao anterior (poda dos ramos inviáveis).
 *
 * Se 'ciclo' for 0, procura um Caminho Hamiltoniano (testa todas
 * as origens). Se 'ciclo' for 1, procura um Ciclo Hamiltoniano
 * (o último vértice precisa ser adjacente ao primeiro).
 *
 * Retorna 1 se encontrou e preenche 'caminho' (n vértices),
 * ou 0 caso não exista. Se 'nos' não for NULL, recebe o número
 * de nós explorados na árvore de busca.
 */
int encontrar_caminho_hamiltoniano_bt(const Grafo *g, int ciclo, int *caminho, long *nos);

/* Atalho para encontrar_caminho_hamiltoniano_bt(g, 1, ...) */
int encontrar_ciclo_hamiltoniano_bt(const Grafo *g, int *ciclo, long *nos);

/*
 * Verifica se 'caminho' é uma permutação dos n vértices em que
 * vértices consecutivos são adjacentes (e, se 'ciclo' for 1,
 * o último também é adjacente ao primeiro).
 */
int eh_caminho_hamiltoniano(const Grafo *g, const int *caminho, int ciclo);

/* Imprime uma sequência de vértices no formato a -> b -> c */
void caminho_imprimir(const int *caminho, int n, int ciclo);

#endif
