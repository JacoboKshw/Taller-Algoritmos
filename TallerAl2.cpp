#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

typedef vector<vector<double>> Matrix;

// Genera una matriz n x n con valores aleatorios
Matrix generarMatriz(int n) {
    Matrix mat(n, vector<double>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            mat[i][j] = (double) rand() / RAND_MAX * 10;
    return mat;
}

// Algoritmo ingenuo de multiplicación
Matrix naiveMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<double>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Suma de matrices
Matrix sumar(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix R(n, vector<double>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            R[i][j] = A[i][j] + B[i][j];
    return R;
}

// Resta de matrices
Matrix restar(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix R(n, vector<double>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            R[i][j] = A[i][j] - B[i][j];
    return R;
}

// Algoritmo de Strassen
Matrix strassenMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    if (n == 1) {
        return {{A[0][0] * B[0][0]}};
    }

    int newSize = n / 2;
    Matrix A11(newSize, vector<double>(newSize));
    Matrix A12(newSize, vector<double>(newSize));
    Matrix A21(newSize, vector<double>(newSize));
    Matrix A22(newSize, vector<double>(newSize));
    Matrix B11(newSize, vector<double>(newSize));
    Matrix B12(newSize, vector<double>(newSize));
    Matrix B21(newSize, vector<double>(newSize));
    Matrix B22(newSize, vector<double>(newSize));

    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

    Matrix M1 = strassenMultiply(sumar(A11, A22), sumar(B11, B22));
    Matrix M2 = strassenMultiply(sumar(A21, A22), B11);
    Matrix M3 = strassenMultiply(A11, restar(B12, B22));
    Matrix M4 = strassenMultiply(A22, restar(B21, B11));
    Matrix M5 = strassenMultiply(sumar(A11, A12), B22);
    Matrix M6 = strassenMultiply(restar(A21, A11), sumar(B11, B12));
    Matrix M7 = strassenMultiply(restar(A12, A22), sumar(B21, B22));

    Matrix C11 = sumar(restar(sumar(M1, M4), M5), M7);
    Matrix C12 = sumar(M3, M5);
    Matrix C21 = sumar(M2, M4);
    Matrix C22 = sumar(restar(sumar(M1, M3), M2), M6);

    Matrix C(n, vector<double>(n));
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }

    return C;
}

// Medir tiempos y mostrar resultados
void medirRendimiento(int n) {
    cout << "Tamaño: " << n << "x" << n << endl;

    Matrix A = generarMatriz(n);
    Matrix B = generarMatriz(n);

    // Naive
    auto start_naive = high_resolution_clock::now();
    Matrix C1 = naiveMultiply(A, B);
    auto end_naive = high_resolution_clock::now();
    auto dur_naive = duration_cast<milliseconds>(end_naive - start_naive);
    cout << "Naive:    " << dur_naive.count() << " ms" << endl;

    // Strassen solo para tamaños potencias de 2
    if ((n & (n - 1)) == 0) {
        auto start_strassen = high_resolution_clock::now();
        Matrix C2 = strassenMultiply(A, B);
        auto end_strassen = high_resolution_clock::now();
        auto dur_strassen = duration_cast<milliseconds>(end_strassen - start_strassen);
        cout << "Strassen: " << dur_strassen.count() << " ms" << endl;
    } else {
        cout << "Strassen no ejecutado (n no es potencia de 2)" << endl;
    }

    cout << endl;
}

int main() {
    srand(time(0));
    vector<int> tamanos = {512}; // Puedes agregar 1024 si tu PC lo permite
    for (int n : tamanos) {
        medirRendimiento(n);
    }
    return 0;
}
