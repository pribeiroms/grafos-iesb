#include <string.h>
#include "planaridade.h"

#define LIMITE 10
#define MASCARAS (1u << LIMITE)

int eh_planar_euler(GrafoLista *g) {
    if (g == NULL) return -1;
    if (g->n < 3) return 1;
    long long m = 0;
    for (int u = 0; u < g->n; u++)
        for (No *no = g->adj[u]; no != NULL; no = no->prox)
            if (u < no->destino) m++;
    return m <= 3LL * g->n - 6;
}

static int contar_bits(unsigned bits) {
    int total = 0;
    while (bits != 0) { bits &= bits - 1; total++; }
    return total;
}

/* Enumera caminhos simples entre dois vertices de ramificacao.
 * Os vertices internos nao podem ser outros vertices de ramificacao.
 * Uma mascara registra exatamente os vertices internos usados.
 */
static void caminhos(const unsigned *adj, int u, int destino, unsigned livres,
                     unsigned usados, unsigned char *opcoes) {
    if (adj[u] & (1u << destino)) {
        opcoes[usados] = 1;
        /* Caminhos mais longos com os mesmos vertices mais outros sao
         * dispensaveis: o caminho curto deixa mais vertices disponiveis. */
        return;
    }
    unsigned candidatos = adj[u] & livres & ~usados;
    for (int v = 0; v < LIMITE; v++)
        if (candidatos & (1u << v))
            caminhos(adj, v, destino, livres, usados | (1u << v), opcoes);
}

/* Para cada aresta do K5/K3,3, escolhe um caminho. A programacao
 * dinamica combina as escolhas de forca bruta exigindo interiores
 * disjuntos: dois caminhos so podem compartilhar suas extremidades.
 * Arestas extras do grafo podem ser ignoradas (subgrafo nao induzido).
 */
static int testar_modelo(const unsigned *adj, int n, unsigned ramos,
                         unsigned lado, int bipartido) {
    unsigned livres = ((1u << n) - 1) & ~ramos;
    unsigned char estados[MASCARAS] = {0};
    estados[0] = 1;
    for (int u = 0; u < n; u++) {
        if (!(ramos & (1u << u))) continue;
        for (int v = u + 1; v < n; v++) {
            if (!(ramos & (1u << v))) continue;
            if (bipartido && !!(lado & (1u << u)) == !!(lado & (1u << v)))
                continue;
            unsigned char opcoes[MASCARAS] = {0};
            unsigned char proximos[MASCARAS] = {0};
            caminhos(adj, u, v, livres, 0, opcoes);
            int encontrou = 0;
            unsigned usados = livres;
            for (;;) {
                if (estados[usados]) {
                    unsigned disponiveis = livres & ~usados;
                    unsigned caminho = disponiveis;
                    for (;;) {
                        if (opcoes[caminho]) {
                            proximos[usados | caminho] = 1;
                            encontrou = 1;
                        }
                        if (caminho == 0) break;
                        caminho = (caminho - 1) & disponiveis;
                    }
                }
                if (usados == 0) break;
                usados = (usados - 1) & livres;
            }
            if (!encontrou) return 0;
            memcpy(estados, proximos, sizeof(estados));
        }
    }
    return 1;
}

int tem_subdivisao_kuratowski(GrafoLista *g) {
    if (g == NULL || g->n > LIMITE) return -1;
    unsigned adj[LIMITE] = {0}, grau3 = 0, grau4 = 0;
    for (int u = 0; u < g->n; u++) {
        for (No *no = g->adj[u]; no != NULL; no = no->prox)
            adj[u] |= 1u << no->destino;
        int grau = contar_bits(adj[u]);
        if (grau >= 3) grau3 |= 1u << u;
        if (grau >= 4) grau4 |= 1u << u;
    }
    for (unsigned ramos = 0; ramos < (1u << g->n); ramos++) {
        int quantidade = contar_bits(ramos);
        if (quantidade == 5 && (ramos & grau4) == ramos &&
            testar_modelo(adj, g->n, ramos, 0, 0)) return 1;
        if (quantidade != 6 || (ramos & grau3) != ramos) continue;
        for (unsigned lado = ramos; lado != 0; lado = (lado - 1) & ramos) {
            /* Considera cada particao 3+3 uma unica vez. */
            if (contar_bits(lado) == 3 && lado < (ramos ^ lado) &&
                testar_modelo(adj, g->n, ramos, lado, 1)) return 1;
        }
    }
    return 0;
}

ResultadoPlanaridade verificar_planaridade(GrafoLista *g) {
    int euler = eh_planar_euler(g);
    if (euler < 0) return PLANARIDADE_ERRO;
    if (!euler) return NAO_PLANAR;
    if (g->n > LIMITE) return PLANARIDADE_INCONCLUSIVA;
    return tem_subdivisao_kuratowski(g) ? NAO_PLANAR : PLANAR;
}
