#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <sys/resource.h> // Necesario para medir uso de memoria en Linux/Mac

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

long long runBruteForce(int n, long long M, long long E, const vector<Anime>& animes);

// Función para obtener la memoria RAM máxima utilizada (Resident Set Size) en KB
long long getMemoryUsageKB() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

int main() {
    string inputDir = "data/inputs/";
    string outputDir = "data/outputs/";
    string measDir = "data/measurements/";

    if (!fs::exists(outputDir)) fs::create_directories(outputDir);
    if (!fs::exists(measDir)) fs::create_directories(measDir);

    // Abrimos un solo archivo para todas las mediciones de Fuerza Bruta
    ofstream measFile(measDir + "measurements_brute_force.txt");
    if (!measFile) {
        cerr << "Error al crear archivo de mediciones.\n";
        return 1;
    }
    measFile << "Archivo Tiempo(ms) Memoria(KB)\n";

    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.path().extension() == ".txt") {
            string filepath = entry.path().string();
            string filename = entry.path().filename().string();

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
            long long maxSatisfaccion = runBruteForce(n, M, E, animes);
            auto end = chrono::high_resolution_clock::now();
            
            chrono::duration<double, milli> duration = end - start;
            long long memoryKB = getMemoryUsageKB();

            // Guardamos el output específico del caso
            ofstream outFile(outputDir + "bf_" + filename);
            if (outFile) {
                outFile << maxSatisfaccion << "\n";
                outFile.close();
            }

            // Registramos las métricas en el archivo consolidado
            measFile << filename << " " << duration.count() << " " << memoryKB << "\n";
            
            cout << "Procesado: " << filename << " | Satisfaccion Optima: " << maxSatisfaccion << "\n";
        }
    }
    
    measFile.close();
    return 0;
}