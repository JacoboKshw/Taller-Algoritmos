#include <vector>
using namespace std;

// Multiplicación clásica
void naive_multiply(const vector<vector<double>> &A,
                    const vector<vector<double>> &B,
                    vector<vector<double>> &C) {
    int n = A.size();
    C.assign(n, vector<double>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];
}

// Puedes implementar Strassen usando submatrices y recursión.
