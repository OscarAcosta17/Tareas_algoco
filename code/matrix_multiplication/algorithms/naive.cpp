/*
Fuente:
GeeksforGeeks - Program to multiply two matrices
https://www.geeksforgeeks.org/c-plus-plus-program-to-multiply-two-matrices/
Adaptado para retornar el vector resultante.
*/

#include <iostream>
#include <vector>

using namespace std;

// Función original de la fuente, manteniendo la lógica O(n^3)
vector<vector<int>> naive_multiply(const vector<vector<int>>& row1, const vector<vector<int>>& row2) {
    int n = row1.size();
    vector<vector<int>> res(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                res[i][j] += row1[i][k] * row2[k][j];
            }
        }
    }
    return res;
}

/*
int main() {
    vector<vector<int>> row1 = { { 1, 1 }, { 2, 2 } };
    vector<vector<int>> row2 = { { 1, 1 }, { 2, 2 } };

    vector<vector<int>> res = naive_multiply(row1, row2);

    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res[0].size(); j++)
            cout << res[i][j] << " ";
        cout << endl;
    }

    return 0;
}
*/