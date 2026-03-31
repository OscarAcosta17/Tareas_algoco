#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

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
    archivo << endl;

    archivo << "quicksort: ";
    for(int x: quick) archivo << x << " ";
    archivo << endl;

    archivo << "std::sort: ";
    for(int x: sort_std) archivo << x << " ";
    archivo << endl;
}
void mediciones(const string& rutaSalida, const string& caso, double tiempoMerge, double tiempoQuick, double tiempoSort){
    ofstream archivo(rutaSalida, ios::app);
    archivo << "caso: " << caso << endl;
    archivo << "mergesort_tiempo: " << tiempoMerge << " ms" << endl;
    archivo << "quicksort_tiempo: " << tiempoQuick << " ms" << endl;
    archivo << "std_sort_tiempo: " << tiempoSort << " ms" << endl;
    archivo << endl;
    archivo.close();
}

int main(){
    string input_dir= "data/array_input/";
    string output_dir="data/array_output/";
    string measurements_dir= "data/measurements/";
    string measurements_ruta= measurements_dir + "mediciones.txt";

    for(const auto& entry: fs::directory_iterator(input_dir)){
        string input_ruta= entry.path().string();
        string nombre= entry.path().filename().string();

        vector<int> arr= leer_archivo(input_ruta);
        vector<int> arr_merge = arr;
        vector<int> arr_quick = arr;
        vector<int> arr_sort = arr;

        auto inicio_merge = chrono::high_resolution_clock::now();
        run_mergesort(arr_merge);
        auto fin_merge = chrono::high_resolution_clock::now();
        double tiempo_merge = chrono::duration<double, milli>(fin_merge - inicio_merge).count();

        auto inicio_quick = chrono::high_resolution_clock::now();
        run_quicksort(arr_quick);
        auto fin_quick = chrono::high_resolution_clock::now();
        double tiempo_quick = chrono::duration<double, milli>(fin_quick - inicio_quick).count();

        auto inicio_sort = chrono::high_resolution_clock::now();
        arr_sort = sortArray(arr_sort);
        auto fin_sort = chrono::high_resolution_clock::now();
        double tiempo_sort = chrono::duration<double, milli>(fin_sort - inicio_sort).count();

        string output_ruta= output_dir + nombre.substr(0, nombre.find(".txt"))+ "_out.txt";
        guardar_archivo(output_ruta, arr_merge, arr_quick, arr_sort);
        mediciones(measurements_ruta, nombre, tiempo_merge,tiempo_quick,tiempo_sort);
    }
    return 0;
}