#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <new>

namespace fs = std::filesystem;
using namespace std;

// =====================================================================
// RASTREADOR DE MEMORIA EXPERIMENTAL (Sobrecarga de operadores)
// =====================================================================
long long memoria_actual = 0;
long long memoria_peak = 0;

// Interceptamos cada vez que C++ pide memoria
void* operator new(size_t size) {
    memoria_actual += size;
    if (memoria_actual > memoria_peak) {
        memoria_peak = memoria_actual;
    }
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    return ptr;
}

// Interceptamos cuando C++ libera memoria (C++14/17 Sized Delete)
void operator delete(void* ptr, size_t size) noexcept {
    memoria_actual -= size;
    free(ptr);
}

// Fallback estándar
void operator delete(void* ptr) noexcept {
    free(ptr);
}
// =====================================================================

vector<vector<int>> naive_multiply(const vector<vector<int>>& A, const vector<vector<int>>& B);
vector<vector<int>> strassen_multiply(vector<vector<int>>& A, vector<vector<int>>& B);

vector<vector<int>> leer_matriz(const string& rutaEntrada, int n) {
    vector<vector<int>> mat(n, vector<int>(n));
    ifstream archivo(rutaEntrada);
    if (archivo.is_open()) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                archivo >> mat[i][j];
            }
        }
        archivo.close();
    }
    return mat;
}

void guardar_archivo(const string& rutaSalida, const vector<vector<int>>& mat) {
    ofstream archivo(rutaSalida);
    int n = mat.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            archivo << mat[i][j];
            if (j < n - 1) archivo << " ";
        }
        archivo << "\n";
    }
    archivo.close();
}

void mediciones(const string& rutaSalida, const string& caso, double tiempoNaive, double tiempoStrassen, size_t memoriaNaive, size_t memoriaStrassen) {
    ofstream archivo(rutaSalida, ios::app);
    archivo << "caso: " << caso << "\n";
    archivo << "naive_tiempo: " << tiempoNaive << " ms\n";
    archivo << "naive_memoria: " << memoriaNaive << " Bytes\n";
    archivo << "strassen_tiempo: " << tiempoStrassen << " ms\n";
    archivo << "strassen_memoria: " << memoriaStrassen << " Bytes\n\n";
    archivo.close();
}

int main() {
    string input_dir = "data/matrix_input/";
    string output_dir = "data/matrix_output/";
    string measurements_dir = "data/measurements/";
    string measurements_ruta = measurements_dir + "mediciones.txt";

    vector<string> archivos;
    for (const auto& entry : fs::directory_iterator(input_dir)) {
        string filename = entry.path().filename().string();
        if (filename.find("_1.txt") != string::npos) {
            archivos.push_back(filename);
        }
    }

    sort(archivos.begin(), archivos.end(), [](const string& a, const string& b) {
        int n_a = stoi(a.substr(0, a.find('_')));
        int n_b = stoi(b.substr(0, b.find('_')));
        if (n_a != n_b) return n_a < n_b;
        return a < b;
    });

    remove(measurements_ruta.c_str());

    for (const string& filename : archivos) {
        string basename = filename.substr(0, filename.find("_1.txt"));
        int n = stoi(basename.substr(0, basename.find('_')));

        string file1_path = input_dir + filename;
        string file2_path = input_dir + basename + "_2.txt";

        cout << "\n========================================\n";
        cout << "Procesando caso: " << basename << "\n";

        vector<vector<int>> M1 = leer_matriz(file1_path, n);
        vector<vector<int>> M2 = leer_matriz(file2_path, n);

        // ================= NAIVE =================
        cout << "  [*] Naive..." << flush;
        
        // Reiniciamos los contadores a CERO justo antes de iniciar
        memoria_actual = 0; 
        memoria_peak = 0;
        
        auto inicio_naive = chrono::high_resolution_clock::now();
        vector<vector<int>> res_naive = naive_multiply(M1, M2);
        auto fin_naive = chrono::high_resolution_clock::now();
        
        double tiempo_naive = chrono::duration<double, milli>(fin_naive - inicio_naive).count();
        size_t memoria_naive = memoria_peak; // Capturamos exactamente lo que usó Naive
        cout << " Listo! (" << tiempo_naive << " ms)\n";

        // ================= STRASSEN =================
        cout << "  [*] Strassen..." << flush;
        
        // Reiniciamos los contadores a CERO para aislar a Strassen
        memoria_actual = 0; 
        memoria_peak = 0;
        
        auto inicio_strassen = chrono::high_resolution_clock::now();
        vector<vector<int>> res_strassen = strassen_multiply(M1, M2);
        auto fin_strassen = chrono::high_resolution_clock::now();
        
        double tiempo_strassen = chrono::duration<double, milli>(fin_strassen - inicio_strassen).count();
        size_t memoria_strassen = memoria_peak; // Capturamos exactamente lo que usó Strassen
        cout << " Listo! (" << tiempo_strassen << " ms)\n";

        // ================= GUARDADO =================
        string output_ruta = output_dir + basename + "_out.txt";
        guardar_archivo(output_ruta, res_naive); 
        mediciones(measurements_ruta, basename, tiempo_naive, tiempo_strassen, memoria_naive, memoria_strassen);
    }
    
    return 0;
}