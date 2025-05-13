#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

double** crearMatriz(int n) {
    double** matriz = (double**) malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        matriz[i] = (double*) malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            matriz[i][j] = ((double) rand() / RAND_MAX) * 10;
        }
    }
    return matriz;
}

void liberarMatriz(double** mat, int n) {
    for (int i = 0; i < n; i++) free(mat[i]);
    free(mat);
}

double** naiveMultiply(double** A, double** B, int n) {
    double** C = crearMatriz(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    return C;
}

// Suma de matrices
double** sumar(double** A, double** B, int n) {
    double** R = crearMatriz(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            R[i][j] = A[i][j] + B[i][j];
    return R;
}

// Resta de matrices
double** restar(double** A, double** B, int n) {
    double** R = crearMatriz(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            R[i][j] = A[i][j] - B[i][j];
    return R;
}

// Algoritmo de Strassen
double** strassen(double** A, double** B, int n) {
    if (n == 1) {
        double** C = crearMatriz(1);
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    int mid = n / 2;
    double **A11 = crearMatriz(mid), **A12 = crearMatriz(mid), **A21 = crearMatriz(mid), **A22 = crearMatriz(mid);
    double **B11 = crearMatriz(mid), **B12 = crearMatriz(mid), **B21 = crearMatriz(mid), **B22 = crearMatriz(mid);

    for (int i = 0; i < mid; i++)
        for (int j = 0; j < mid; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + mid];
            A21[i][j] = A[i + mid][j];
            A22[i][j] = A[i + mid][j + mid];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + mid];
            B21[i][j] = B[i + mid][j];
            B22[i][j] = B[i + mid][j + mid];
        }

    double** M1 = strassen(sumar(A11, A22, mid), sumar(B11, B22, mid), mid);
    double** M2 = strassen(sumar(A21, A22, mid), B11, mid);
    double** M3 = strassen(A11, restar(B12, B22, mid), mid);
    double** M4 = strassen(A22, restar(B21, B11, mid), mid);
    double** M5 = strassen(sumar(A11, A12, mid), B22, mid);
    double** M6 = strassen(restar(A21, A11, mid), sumar(B11, B12, mid), mid);
    double** M7 = strassen(restar(A12, A22, mid), sumar(B21, B22, mid), mid);

    double** C11 = sumar(restar(sumar(M1, M4, mid), M5, mid), M7, mid);
    double** C12 = sumar(M3, M5, mid);
    double** C21 = sumar(M2, M4, mid);
    double** C22 = sumar(restar(sumar(M1, M3, mid), M2, mid), M6, mid);

    double** C = crearMatriz(n);
    for (int i = 0; i < mid; i++)
        for (int j = 0; j < mid; j++) {
            C[i][j] = C11[i][j];
            C[i][j + mid] = C12[i][j];
            C[i + mid][j] = C21[i][j];
            C[i + mid][j + mid] = C22[i][j];
        }

    liberarMatriz(A11, mid); liberarMatriz(A12, mid); liberarMatriz(A21, mid); liberarMatriz(A22, mid);
    liberarMatriz(B11, mid); liberarMatriz(B12, mid); liberarMatriz(B21, mid); liberarMatriz(B22, mid);
    liberarMatriz(M1, mid); liberarMatriz(M2, mid); liberarMatriz(M3, mid); liberarMatriz(M4, mid);
    liberarMatriz(M5, mid); liberarMatriz(M6, mid); liberarMatriz(M7, mid);
    liberarMatriz(C11, mid); liberarMatriz(C12, mid); liberarMatriz(C21, mid); liberarMatriz(C22, mid);

    return C;
}

int esPotenciaDe2(int n) {
    return (n & (n - 1)) == 0;
}

void medirRendimiento(int n) {
    printf("\nTamaño: %dx%d\n", n, n);

    double** A = crearMatriz(n);
    double** B = crearMatriz(n);

    clock_t start = clock();
    double** C1 = naiveMultiply(A, B, n);
    clock_t end = clock();
    printf("Naive:    %.2f ms\n", 1000.0 * (end - start) / CLOCKS_PER_SEC);
    liberarMatriz(C1, n);

    if (esPotenciaDe2(n)) {
        start = clock();
        double** C2 = strassen(A, B, n);
        end = clock();
        printf("Strassen: %.2f ms\n", 1000.0 * (end - start) / CLOCKS_PER_SEC);
        liberarMatriz(C2, n);
    } else {
        printf("Strassen no ejecutado (n no es potencia de 2)\n");
    }

    liberarMatriz(A, n);
    liberarMatriz(B, n);
}

int main() {
    srand(time(NULL));
    int tamanos[] = {128, 256, 512};  // Puedes incluir 1024 si tu PC lo soporta
    int num = sizeof(tamanos) / sizeof(tamanos[0]);
    for (int i = 0; i < num; i++) {
        medirRendimiento(tamanos[i]);
    }
    return 0;
}
