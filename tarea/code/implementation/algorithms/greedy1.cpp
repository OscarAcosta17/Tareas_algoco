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
 * Idea base del algoritmo: "Fractional Knapsack Problem Greedy Approach" adaptada a múltiples restricciones.
 * Referencia teórica general: https://www.geeksforgeeks.org/fractional-knapsack-problem/
 * * ADAPTACIÓN:
 * Dado que un anime solo permite tomar prefijos completos de capítulos, se generaron 
 * "Opciones de Prefijo" como si fueran objetos indivisibles de una mochila.
 * La rentabilidad (ratio) de cada prefijo se calculó como la satisfacción total (incluyendo bono) 
 * dividida por la suma del tiempo y la energía consumida. Luego se ordenan de forma decreciente.
 */

struct PrefixOption {
    int animeIdx;
    int k; // Cantidad de capitulos tomados
    long long totalT;
    long long totalE;
    long long totalV;
    double ratio;
};

bool comparePrefixes(const PrefixOption& a, const PrefixOption& b) {
    return a.ratio > b.ratio; // Orden descendente por rentabilidad
}

long long runGreedy1(int n, long long M, long long E, const vector<Anime>& animes) {
    vector<PrefixOption> options;

    // 1. Construir todas las opciones posibles (todos los prefijos para cada anime)
    for (int i = 0; i < n; ++i) {
        long long sumT = 0, sumE = 0, sumV = 0;
        
        for (int k = 1; k <= animes[i].q; ++k) {
            sumT += animes[i].capitulos[k-1].t;
            sumE += animes[i].capitulos[k-1].c;
            sumV += animes[i].capitulos[k-1].v;
            
            // Si el prefijo excede los límites máximos por sí solo, no sirve evaluarlo
            if (sumT <= M && sumE <= E) {
                long long bonus = (k == animes[i].q) ? animes[i].b : 0;
                long long totalVal = sumV + bonus;
                double currentRatio = static_cast<double>(totalVal) / (sumT + sumE);
                
                options.push_back({i, k, sumT, sumE, totalVal, currentRatio});
            }
        }
    }

    // 2. Ordenar las opciones de mayor a menor rentabilidad
    sort(options.begin(), options.end(), comparePrefixes);

    // 3. Selección Greedy
    vector<bool> animeTomado(n, false);
    long long currentM = 0;
    long long currentE = 0;
    long long maxSatisfaccion = 0;

    for (const auto& opt : options) {
        // Validar que el anime no haya sido procesado y que los recursos alcancen
        if (!animeTomado[opt.animeIdx] && (currentM + opt.totalT <= M) && (currentE + opt.totalE <= E)) {
            
            animeTomado[opt.animeIdx] = true;
            currentM += opt.totalT;
            currentE += opt.totalE;
            maxSatisfaccion += opt.totalV;
        }
    }

    return maxSatisfaccion;
}