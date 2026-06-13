#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Capitulo {
    long long t, c, v;
};

struct Anime {
    string nombre;
    int q;
    long long b;
    vector<Capitulo> capitulos;
};

/*
 * FUENTE DE INFORMACIÓN:
 * Idea base del algoritmo: "0/1 Knapsack Problem using Backtracking" 
 * Referencia teórica general: https://www.geeksforgeeks.org/0-1-knapsack-problem-dp-10/
 * * ADAPTACIÓN:
 * El concepto de tomar o no tomar un elemento de la mochila fue adaptado para iterar sobre la cantidad de elementos a tomar de una lista dependiente d animes
 * Se modificó para manejar dos restricciones simultáneas (Tiempo M y Energía E) en lugar de un solo peso y se añadio la lógica de sumar un bono extra condicional 'b' si se toman todos los capítulos 'q' de un anime.
 */
long long solve(int idx, long long currentM, long long currentE, int n, long long M, long long E, const vector<Anime>& animes) {
    if (idx == n) return 0;

    // Opción 1: No ver ningún capítulo de este anime, pasamos al siguiente
    long long max_sat = solve(idx + 1, currentM, currentE, n, M, E, animes);

    long long sumT = 0;
    long long sumE = 0;
    long long sumV = 0;

    // Opción 2: Ver 'k' capítulos de este anime (desde 1 hasta q)
    for (int k = 1; k <= animes[idx].q; ++k) {
        sumT += animes[idx].capitulos[k-1].t;
        sumE += animes[idx].capitulos[k-1].c;
        sumV += animes[idx].capitulos[k-1].v;

        // Poda: Solo continuamos si no superamos los límites de tiempo y energía
        if (currentM + sumT <= M && currentE + sumE <= E) {
            long long bonus = (k == animes[idx].q) ? animes[idx].b : 0;
            
            // Llamada recursiva para el siguiente anime sumando el estado actual
            long long result = sumV + bonus + solve(idx + 1, currentM + sumT, currentE + sumE, n, M, E, animes);
            max_sat = max(max_sat, result);
        } else {
            // Si nos pasamos con 'k' capítulos, intentar 'k+1' también excederá los límites (los costos son positivos)
            break; 
        }
    }

    return max_sat;
}

long long runBruteForce(int n, long long M, long long E, const vector<Anime>& animes) {
    // Inicia la recursión desde el anime 0, con 0 minutos y 0 de energía consumidos
    return solve(0, 0, 0, n, M, E, animes);
}