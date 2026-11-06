#ifndef COBERTURA_H
#define COBERTURA_H

#define MAX_VERTICES 32

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
 * Verifica se o conjunto 'cobertura' (vetor 0/1 de tamanho n)
 * cobre todas as arestas do grafo.
 * Retorna 1 se for cobertura, 0 caso contrário.
 */
int eh_cobertura(const Grafo *g, const int *cobertura);

/*
 * Força bruta com backtracking e poda por limite.
 * Preenche 'cobertura' (vetor 0/1) com uma cobertura mínima
 * e retorna o seu tamanho. Se 'nos' não for NULL, recebe
 * o número de nós explorados na árvore de busca.
 */
int cobertura_minima_bt(const Grafo *g, int *cobertura, long *nos);

/*
 * Heurística gulosa: escolhe repetidamente o vértice de maior
 * grau restante (considerando apenas arestas ainda não cobertas).
 * Preenche 'cobertura' (vetor 0/1) e retorna o seu tamanho.
 */
int cobertura_gulosa(const Grafo *g, int *cobertura);

/* Imprime os vértices de uma cobertura no formato { a b c } */
void cobertura_imprimir(const int *cobertura, int n);

#endif
