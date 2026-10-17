#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

#define INFINITO INT_MAX

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

/* Digrafo ponderado: inserir u -> v nao insere v -> u.
 * Aceita pesos negativos, lacos e arestas paralelas.
 */
typedef struct {
    int n;
    No **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_lista_ponderado(int n);
int inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo_lista_ponderado(GrafoListaPonderado *g);

/* dist[] e pred[] devem ter n posicoes. Vertices inalcancaveis ficam com
 * dist = INFINITO e pred = -1; a origem fica com pred = -1.
 * Retorna 1 sem ciclo negativo alcancavel, 0 com ciclo negativo alcancavel
 * a partir da origem e -1 para argumento invalido.
 * Com retorno 0, dist[] nao representa menores caminhos, mas pred[] contem
 * o ciclo usado por reconstruir_ciclo_negativo().
 * Supoe que pesos e distancias cabem em int.
 */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Usa pred[] deixado por bellman_ford() com retorno 0: retrocede n vezes
 * a partir de cada vertice ate cair dentro de um ciclo e imprime o ciclo
 * na ordem das arestas, com o custo total.
 * Retorna o numero de arestas do ciclo, 0 se pred[] nao tiver ciclo e -1
 * para argumento invalido.
 */
int reconstruir_ciclo_negativo(GrafoListaPonderado *g, const int *pred);

#endif
