#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

// estrutura obrigatoria da matriz de adjacencia
typedef struct {
    int n;
    int **adj;
} GrafoMatriz;

// operacoes disponiveis para a matriz de adjacencia
GrafoMatriz *criar_grafo_matriz(int n);
int inserir_aresta_matriz(GrafoMatriz *grafo, int u, int v);
int remover_aresta_matriz(GrafoMatriz *grafo, int u, int v);
int grau_matriz(const GrafoMatriz *grafo, int vertice);
int sao_adjacentes_matriz(const GrafoMatriz *grafo, int u, int v);
void exibir_grafo_matriz(const GrafoMatriz *grafo);
void liberar_grafo_matriz(GrafoMatriz *grafo);

#endif
