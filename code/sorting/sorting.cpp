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

long long memoria_actual = 0;
long long memoria_peak = 0;

void* operator new(size_t size) {
    memoria_actual += size;
    if (memoria_actual > memoria_peak) {
        memoria_peak = memoria_actual;
    }
    void* ptr = malloc(size);
    if (!ptr) throw std::bad_alloc();
    return ptr;
}

void operator delete(void* ptr, size_t size) noexcept {
    memoria_actual -= size;
    free(ptr);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}
// =====================================================================

void run_mergesort(vector<int>& arr);
void run_quicksort(vector<int>& arr);
vector<int> sortArray(vector<int>& arr);

vector<int> leer_archivo(const string& rutaEntrada){
    vector<int> arr;
    ifstream archivo(rutaEntrada);
    int numero;
    while (archivo >> numero){
        arr.push_back(numero);
    }
    archivo.close();
    return arr;
}

void guardar_archivo(const string& rutaSalida, const vector<int>& merge, const vector<int>& quick, const vector<int>& sort_std){
    ofstream archivo(rutaSalida);
    archivo << "mergesort: ";
    for(int x: merge) archivo << x << " ";
    archivo << "\n";

    archivo << "quicksort: ";
    for(int x: quick) archivo << x << " ";
    archivo << "\n";

    archivo << "std::sort: ";
    for(int x: sort_std) archivo << x << " ";
    archivo << "\n";
    archivo.close();
}

void mediciones(const string& rutaSalida, const string& caso, double tiempoMerge, double tiempoQuick, double tiempoSort, size_t memoriaMerge, size_t memoriaQuick, size_t memoriaSort) {
    ofstream archivo(rutaSalida, ios::app);

    archivo << "caso: " << caso << "\n";
    archivo << "mergesort_tiempo: " << tiempoMerge << " ms\n";
    archivo << "mergesort_memoria: " << memoriaMerge << " Bytes\n";

    archivo << "quicksort_tiempo: " << tiempoQuick << " ms\n";
    archivo << "quicksort_memoria: " << memoriaQuick << " Bytes\n";

    archivo << "std_sort_tiempo: " << tiempoSort << " ms\n";
    archivo << "std_sort_memoria: " << memoriaSort << " Bytes\n\n";

    archivo.close();
}

int main(){
    string input_dir = "data/array_input/";
    string output_dir = "data/array_output/";
    string measurements_dir = "data/measurements/";
    string measurements_ruta = measurements_dir + "mediciones.txt";

    vector<string> archivos;
    for(const auto& entry: fs::directory_iterator(input_dir)){
        string filename = entry.path().filename().string();
        if (filename.find(".txt") != string::npos) {
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

    for(const string& filename : archivos){
        string input_ruta = input_dir + filename;
        string basename = filename.substr(0, filename.find(".txt"));
        
        cout << "\n========================================\n";
        cout << "Procesando caso: " << basename << "\n";

        vector<int> arr = leer_archivo(input_ruta);
        vector<int> arr_merge = arr;
        vector<int> arr_quick = arr;
        vector<int> arr_sort = arr;

        // ================= MERGE SORT =================
        cout << "  [*] Merge Sort..." << flush;
        memoria_actual = 0; memoria_peak = 0;
        
        auto inicio_merge = chrono::high_resolution_clock::now();
        run_mergesort(arr_merge);
        auto fin_merge = chrono::high_resolution_clock::now();
        
        double tiempo_merge = chrono::duration<double, milli>(fin_merge - inicio_merge).count();
        size_t memoria_merge = memoria_peak;
        cout << " Listo! (" << tiempo_merge << " ms)\n";

        // ================= QUICK SORT =================
        cout << "  [*] Quick Sort..." << flush;
        memoria_actual = 0; memoria_peak = 0; 
        
        auto inicio_quick = chrono::high_resolution_clock::now();
        run_quicksort(arr_quick);
        auto fin_quick = chrono::high_resolution_clock::now();
        
        double tiempo_quick = chrono::duration<double, milli>(fin_quick - inicio_quick).count();
        size_t memoria_quick = memoria_peak;
        cout << " Listo! (" << tiempo_quick << " ms)\n";

        // ================= SORT =================
        cout << "  [*] std::sort..." << flush;
        memoria_actual = 0; memoria_peak = 0; 
        
        auto inicio_sort = chrono::high_resolution_clock::now();
        arr_sort = sortArray(arr_sort);
        auto fin_sort = chrono::high_resolution_clock::now();
        
        double tiempo_sort = chrono::duration<double, milli>(fin_sort - inicio_sort).count();
        size_t memoria_sort = memoria_peak;
        cout << " Listo! (" << tiempo_sort << " ms)\n";

        // ================= GUARDADO =================
        string output_ruta = output_dir + basename + "_out.txt";
        guardar_archivo(output_ruta, arr_merge, arr_quick, arr_sort);
        mediciones(measurements_ruta, basename, tiempo_merge, tiempo_quick, tiempo_sort, memoria_merge, memoria_quick, memoria_sort);
        
        arr.clear(); arr.shrink_to_fit();
        arr_merge.clear(); arr_merge.shrink_to_fit();
        arr_quick.clear(); arr_quick.shrink_to_fit();
        arr_sort.clear(); arr_sort.shrink_to_fit();
    }
    
    return 0;
}