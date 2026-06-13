#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Capitulo { long long t, c, v; };
struct Anime { string nombre; int q; long long b; vector<Capitulo> capitulos; };

/*
 * FUENTE DE INFORMACIÓN:
 * Idea base del algoritmo: "Fractional Knapsack Problem Greedy Approach"
 * Referencia teórica general: https://www.geeksforgeeks.org/fractional-knapsack-problem/
 * * ADAPTACIÓN:
 * A diferencia de Greedy 1, el criterio aquí es maximizar la satisfacción por minuto de visualización, ignorando el costo energético al ordenar las opciones. 
 * La restricción de energía actúa únicamente como un filtro de validez al momento de intentar insertar el prefijo.
 */

struct PrefixOption2 {
    int animeIdx;
    int k;
    long long totalT;
    long long totalE;
    long long totalV;
    double ratio;
};

bool comparePrefixes2(const PrefixOption2& a, const PrefixOption2& b) {
    return a.ratio > b.ratio; 
}

long long runGreedy2(int n, long long M, long long E, const vector<Anime>& animes) {
    vector<PrefixOption2> options;

    for (int i = 0; i < n; ++i) {
        long long sumT = 0, sumE = 0, sumV = 0;
        
        for (int k = 1; k <= animes[i].q; ++k) {
            sumT += animes[i].capitulos[k-1].t;
            sumE += animes[i].capitulos[k-1].c;
            sumV += animes[i].capitulos[k-1].v;
            
            if (sumT <= M && sumE <= E) {
                long long bonus = (k == animes[i].q) ? animes[i].b : 0;
                long long totalVal = sumV + bonus;
                double currentRatio = static_cast<double>(totalVal) / sumT;
                
                options.push_back({i, k, sumT, sumE, totalVal, currentRatio});
            }
        }
    }

    sort(options.begin(), options.end(), comparePrefixes2);

    vector<bool> animeTomado(n, false);
    long long currentM = 0, currentE = 0, maxSatisfaccion = 0;

    for (const auto& opt : options) {
        if (!animeTomado[opt.animeIdx] && (currentM + opt.totalT <= M) && (currentE + opt.totalE <= E)) {
            animeTomado[opt.animeIdx] = true;
            currentM += opt.totalT;
            currentE += opt.totalE;
            maxSatisfaccion += opt.totalV;
        }
    }

    return maxSatisfaccion;
}