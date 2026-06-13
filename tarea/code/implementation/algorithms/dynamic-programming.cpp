#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Capitulo { long long t, c, v; };
struct Anime { string nombre; int q; long long b; vector<Capitulo> capitulos; };

/*
 * FUENTE DE INFORMACIÓN:
 * Idea base del algoritmo: "0/1 Knapsack Problem with Two Constraints" (2D Dynamic Programming).
 * Referencia teórica general: https://www.geeksforgeeks.org/2d-knapsack-problem/
 * * ADAPTACIÓN:
 * El problema clásico se adaptó para procesar agrupaciones mutuamente excluyentes.
 * Por cada anime, solo se puede escoger exactamente un prefijo válido (desde 0 hasta q_i).
 * Para evitar seleccionar múltiples prefijos de un mismo anime y acumular sus valores, se utiliza una matriz temporal que se actualiza leyendo estrictamente desde el estado de la iteración anterior, simulando un enfoque de Multiple-Choice Knapsack.
 */

long long runDynamicProgramming(int n, long long M, long long E, const vector<Anime>& animes) {
    // dp[m][e] almacena la máxima satisfacción con 'm' minutos y 'e' energía
    vector<vector<long long>> dp(M + 1, vector<long long>(E + 1, 0));

    for (int i = 0; i < n; ++i) {
        // next_dp inicia como una copia de dp (equivalente a no tomar ningún capítulo de este anime)
        vector<vector<long long>> next_dp = dp; 

        long long sumT = 0, sumE = 0, sumV = 0;
        
        for (int k = 1; k <= animes[i].q; ++k) {
            sumT += animes[i].capitulos[k-1].t;
            sumE += animes[i].capitulos[k-1].c;
            sumV += animes[i].capitulos[k-1].v;

            if (sumT > M || sumE > E) break;

            long long bonus = (k == animes[i].q) ? animes[i].b : 0;
            long long totalVal = sumV + bonus;

            // Recorrer la mochila verificando si podemos insertar este prefijo
            for (long long m = M; m >= sumT; --m) {
                for (long long e = E; e >= sumE; --e) {
                    next_dp[m][e] = max(next_dp[m][e], dp[m - sumT][e - sumE] + totalVal);
                }
            }
        }
        // Consolidar el estado para el siguiente anime
        dp = next_dp;
    }

    long long max_sat = 0;
    for (long long m = 0; m <= M; ++m) {
        for (long long e = 0; e <= E; ++e) {
            max_sat = max(max_sat, dp[m][e]);
        }
    }
    
    return max_sat;
}