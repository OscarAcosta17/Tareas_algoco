#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <sys/resource.h>
#include <algorithm>
#include <cstdio>

using namespace std;
namespace fs = std::filesystem;

struct Capitulo {
    long long t, c, v;
};

struct Anime {
    string nombre;
    int q;
    long long b;
    vector<Capitulo> capitulos;
};

// Firmas de los algoritmos
long long runBruteForce(int n, long long M, long long E, const vector<Anime>& animes);
long long runGreedy1(int n, long long M, long long E, const vector<Anime>& animes);
long long runGreedy2(int n, long long M, long long E, const vector<Anime>& animes);
long long runDynamicProgramming(int n, long long M, long long E, const vector<Anime>& animes);

long long getMemoryUsageKB() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

int main() {
    int opcion;
    cout << "Selecciona el algoritmo a ejecutar:\n";
    cout << "1. Fuerza Bruta\n";
    cout << "2. Greedy 1\n";
    cout << "3. Greedy 2\n";
    cout << "4. Programacion Dinamica\n";
    cout << "Opcion: ";
    if (!(cin >> opcion)) return 1;

    string prefixOut, measFileName;
    if (opcion == 1) {
        prefixOut = "bf_";
        measFileName = "measurements_brute_force.txt";
    } else if (opcion == 2) {
        prefixOut = "g1_";
        measFileName = "measurements_greedy1.txt";
    } else if (opcion == 3) {
        prefixOut = "g2_";
        measFileName = "measurements_greedy2.txt";
    } else if (opcion == 4) {
        prefixOut = "dp_";
        measFileName = "measurements_dynamic_programming.txt";
    } else {
        cout << "Opcion no valida o en desarrollo.\n";
        return 0;
    }

    string inputDir = "data/inputs/";
    string outputDir = "data/outputs/";
    string measDir = "data/measurements/";

    if (!fs::exists(outputDir)) fs::create_directories(outputDir);
    if (!fs::exists(measDir)) fs::create_directories(measDir);

    ofstream measFile(measDir + measFileName);
    if (!measFile) {
        cerr << "Error al crear archivo de mediciones.\n";
        return 1;
    }
    measFile << "Archivo Tiempo(ms) Memoria(KB)\n";

    // 1. Recolectar todos los archivos válidos
    vector<string> archivos;
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".txt") {
            archivos.push_back(entry.path().filename().string());
        }
    }

    // 2. Ordenar numéricamente basándonos en "n" e "i" del formato testcases_{n}_{i}.txt
    sort(archivos.begin(), archivos.end(), [](const string& a, const string& b) {
        int n_a, i_a, n_b, i_b;
        sscanf(a.c_str(), "testcases_%d_%d.txt", &n_a, &i_a);
        sscanf(b.c_str(), "testcases_%d_%d.txt", &n_b, &i_b);
        if (n_a != n_b) return n_a < n_b;
        return i_a < i_b;
    });

    // 3. Ejecutar los algoritmos en el orden correcto
    for (const string& filename : archivos) {
        string filepath = inputDir + filename;

        ifstream file(filepath);
        if (!file) continue;

        int n;
        long long M, E;
        file >> n >> M >> E;

        vector<Anime> animes(n);
        for (int i = 0; i < n; ++i) {
            file >> animes[i].nombre >> animes[i].q >> animes[i].b;
            animes[i].capitulos.resize(animes[i].q);
            for (int j = 0; j < animes[i].q; ++j) {
                file >> animes[i].capitulos[j].t 
                     >> animes[i].capitulos[j].c 
                     >> animes[i].capitulos[j].v;
            }
        }
        file.close();

        auto start = chrono::high_resolution_clock::now();
        long long maxSatisfaccion = 0;
        
        if (opcion == 1) maxSatisfaccion = runBruteForce(n, M, E, animes);
        else if (opcion == 2) maxSatisfaccion = runGreedy1(n, M, E, animes);
        else if (opcion == 3) maxSatisfaccion = runGreedy2(n, M, E, animes);
        else if (opcion == 4) maxSatisfaccion = runDynamicProgramming(n, M, E, animes);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;
        long long memoryKB = getMemoryUsageKB();

        ofstream outFile(outputDir + prefixOut + filename);
        if (outFile) {
            outFile << maxSatisfaccion << "\n";
            outFile.close();
        }

        measFile << filename << " " << duration.count() << " " << memoryKB << "\n";
        measFile.flush();
        cout << "Procesado: " << filename << " | Satisfaccion: " << maxSatisfaccion << "\n";
    }
    
    measFile.close();
    return 0;
}