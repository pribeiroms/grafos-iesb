#ifndef PLANARIDADE_H
#define PLANARIDADE_H

#include "conectividade.h"

/* Euler: 1 satisfaz a condicao NECESSARIA m <= 3n-6, 0 viola,
 * -1 argumento invalido. Para n < 3, todo grafo simples e planar.
 * Retorno 1 sozinho NAO prova planaridade.
 */
int eh_planar_euler(GrafoLista *g);

/* Para n <= 10, busca exata de subdivisoes como subgrafos:
 * 1 encontrou K5 ou K3,3 subdividido; 0 nao encontrou; -1 erro/limite.
 */
int tem_subdivisao_kuratowski(GrafoLista *g);

typedef enum {
    PLANARIDADE_ERRO = -1,
    NAO_PLANAR = 0,
    PLANAR = 1,
    PLANARIDADE_INCONCLUSIVA = 2
} ResultadoPlanaridade;

/* Euler + Kuratowski. Acima de 10 vertices, passar em Euler e inconclusivo. */
ResultadoPlanaridade verificar_planaridade(GrafoLista *g);

#endif
