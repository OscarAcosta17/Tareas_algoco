/*  
Algoritmo: Quick sort
Fuente: https://www.geeksforgeeks.org/dsa/quick-sort-algorithm/
Descripción: Implementación basada en el algoritmo de quick sort, se seelcciona un pivote y se reorganiza el arreglo en menores y mayores al pivote y se reordena recursivamente cada subarreglo
Extra:
    - El codigo fue sutilmente modificado para usar los casos de prueba dados, dejando comentado el main y añadiendo una funcion, igualito como en el mergesort
*/
#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int>& arr, int low, int high) {
  
    // choose the pivot
    int pivot = arr[high];
  
    // undex of smaller element and indicates 
    // the right position of pivot found so far
    int i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements on left side. Elements from low to 
    // i are smaller after every iteration
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    
    // move pivot after smaller elements and
    // return its position
    swap(arr[i + 1], arr[high]);  
    return i + 1;
}

// the QuickSort function implementation
void quickSort(vector<int>& arr, int low, int high) {
  
    if (low < high) {
      
        // pi is the partition return index of pivot
        int pi = partition(arr, low, high);

        // recursion calls for smaller elements
        // and greater or equals elements
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
/*
int main() {
    vector<int> arr = {10, 7, 8, 9, 1, 5};
    int n = arr.size();
    quickSort(arr, 0, n - 1);
  
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    return 0;
}*/

void run_quicksort(vector<int>& arr) {
    if (!arr.empty()) {
        quickSort(arr, 0, arr.size() - 1);
    }
}