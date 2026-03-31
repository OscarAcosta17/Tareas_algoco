#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

void run_mergesort(vector<int>& arr);
void run_quicksort(vector<int>& arr);

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
void guardar_archivo(const string& rutaSalida, const vector<int>& merge, const vector<int>& quick){
    ofstream archivo(rutaSalida);
    archivo << "mergesort: ";
    for(int x: merge) archivo << x << " ";
    archivo << endl;
    archivo << "quicksort: ";
    for(int x: quick) archivo << x << " ";
    archivo << endl;
}

int main(){
    string input_dir= "data/array_input/";
    string output_dir="data/array_output/";

    for(const auto& entry: fs::directory_iterator(input_dir)){
        string input_ruta= entry.path().string();
        string nombre= entry.path().filename().string();

        vector<int> arr= leer_archivo(input_ruta);
        vector<int> arr_merge = arr;
        vector<int> arr_quick = arr;

        run_mergesort(arr_merge);
        run_quicksort(arr_quick);

        string output_ruta= output_dir + nombre.substr(0, nombre.find(".txt"))+ "_out.txt";
        guardar_archivo(output_ruta, arr_merge, arr_quick);
    }
    return 0;
}