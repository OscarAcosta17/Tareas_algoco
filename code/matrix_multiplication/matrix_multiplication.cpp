#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;

// Declaraciones previas (Forward declarations) de tus algoritmos
// Esto le dice al compilador que estas funciones existen en otros archivos
vector<vector<int>> naive_multiply(const vector<vector<int>>& m1, const vector<vector<int>>& m2);
vector<vector<int>> strassen_multiply(vector<vector<int>>& mat1, vector<vector<int>>& mat2);

// Función para leer una matriz desde un archivo de texto
vector<vector<int>> read_matrix(const string& filepath, int n) {
    vector<vector<int>> matrix(n, vector<int>(n));
    ifstream file(filepath);
    
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << filepath << endl;
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
        }
    }
    file.close();
    return matrix;
}

// Función para guardar la matriz en el archivo *_out.txt
void save_matrix(const string& filepath, const vector<vector<int>>& matrix) {
    ofstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error al crear el archivo de salida: " << filepath << endl;
        return;
    }

    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file << matrix[i][j];
            if (j < n - 1) file << " ";
        }
        file << "\n";
    }
    file.close();
}

int main(int argc, char* argv[]) {
    // Verificamos que se pasen los argumentos necesarios
    // Ejemplo de uso: ./matrix_multiplication 16 densa D0 a naive
    if (argc < 6) {
        cerr << "Uso: " << argv[0] << " <n> <t> <d> <m> <algoritmo>" << endl;
        cerr << "Ejemplo: " << argv[0] << " 16 densa D0 a naive" << endl;
        return 1;
    }

    // Parseo de argumentos
    int n = stoi(argv[1]);
    string t = argv[2];
    string d = argv[3];
    string m = argv[4];
    string algorithm = argv[5];

    // Construcción de las rutas de los archivos (basado en la estructura de la imagen y el PDF)
    string base_filename = to_string(n) + "_" + t + "_" + d + "_" + m;
    string file1_path = "data/matrix_input/" + base_filename + "_1.txt";
    string file2_path = "data/matrix_input/" + base_filename + "_2.txt";
    string output_path = "data/matrix_output/" + base_filename + "_out.txt";

    // 1. Leer matrices
    vector<vector<int>> M1 = read_matrix(file1_path, n);
    vector<vector<int>> M2 = read_matrix(file2_path, n);
    vector<vector<int>> result;

    // 2. Medir tiempo y ejecutar el algoritmo seleccionado
    auto start_time = chrono::high_resolution_clock::now();

    if (algorithm == "naive") {
        result = naive_multiply(M1, M2);
    } else if (algorithm == "strassen") {
        result = strassen_multiply(M1, M2);
    } else {
        cerr << "Algoritmo no reconocido. Usa 'naive' o 'strassen'." << endl;
        return 1;
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end_time - start_time;

    // 3. Escribir la matriz resultante
    save_matrix(output_path, result);

    // 4. (Opcional por ahora) Guardar la medición de tiempo en la carpeta measurements
    // Deberás implementar la escritura en measurements/matrix_multiplication/ 
    cout << "Algoritmo: " << algorithm << " | Archivo: " << base_filename << endl;
    cout << "Tiempo de ejecución: " << duration.count() << " ms" << endl;

    return 0;
}