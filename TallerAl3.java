/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package nicaragua;

/**
 *
 * @author Jacobo
 */
import java.util.Random;

public class MatrixMultiplication {

    // Generar una matriz cuadrada aleatoria
    public static double[][] generarMatriz(int n) {
        double[][] matriz = new double[n][n];
        Random rand = new Random();
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                matriz[i][j] = rand.nextDouble() * 10;
        return matriz;
    }

    // Algoritmo ingenuo
    public static double[][] naiveMultiply(double[][] A, double[][] B) {
        int n = A.length;
        double[][] C = new double[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    C[i][j] += A[i][k] * B[k][j];
        return C;
    }

    // Suma de matrices
    private static double[][] sumar(double[][] A, double[][] B) {
        int n = A.length;
        double[][] R = new double[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                R[i][j] = A[i][j] + B[i][j];
        return R;
    }

    // Resta de matrices
    private static double[][] restar(double[][] A, double[][] B) {
        int n = A.length;
        double[][] R = new double[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                R[i][j] = A[i][j] - B[i][j];
        return R;
    }

    // Algoritmo de Strassen
    public static double[][] strassenMultiply(double[][] A, double[][] B) {
        int n = A.length;
        if (n == 1) {
            double[][] C = {{A[0][0] * B[0][0]}};
            return C;
        }

        int newSize = n / 2;
        double[][] A11 = new double[newSize][newSize];
        double[][] A12 = new double[newSize][newSize];
        double[][] A21 = new double[newSize][newSize];
        double[][] A22 = new double[newSize][newSize];
        double[][] B11 = new double[newSize][newSize];
        double[][] B12 = new double[newSize][newSize];
        double[][] B21 = new double[newSize][newSize];
        double[][] B22 = new double[newSize][newSize];

        // Dividir matrices en submatrices
        for (int i = 0; i < newSize; i++) {
            for (int j = 0; j < newSize; j++) {
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

        // Cálculo de productos de Strassen
        double[][] M1 = strassenMultiply(sumar(A11, A22), sumar(B11, B22));
        double[][] M2 = strassenMultiply(sumar(A21, A22), B11);
        double[][] M3 = strassenMultiply(A11, restar(B12, B22));
        double[][] M4 = strassenMultiply(A22, restar(B21, B11));
        double[][] M5 = strassenMultiply(sumar(A11, A12), B22);
        double[][] M6 = strassenMultiply(restar(A21, A11), sumar(B11, B12));
        double[][] M7 = strassenMultiply(restar(A12, A22), sumar(B21, B22));

        // Combinación de resultados
        double[][] C11 = sumar(restar(sumar(M1, M4), M5), M7);
        double[][] C12 = sumar(M3, M5);
        double[][] C21 = sumar(M2, M4);
        double[][] C22 = sumar(restar(sumar(M1, M3), M2), M6);

        // Combinar submatrices en matriz resultado
        double[][] C = new double[n][n];
        for (int i = 0; i < newSize; i++) {
            for (int j = 0; j < newSize; j++) {
                C[i][j] = C11[i][j];
                C[i][j + newSize] = C12[i][j];
                C[i + newSize][j] = C21[i][j];
                C[i + newSize][j + newSize] = C22[i][j];
            }
        }

        return C;
    }

    // Función de prueba de rendimiento
    public static void medirRendimiento(int n) {
        double[][] A = generarMatriz(n);
        double[][] B = generarMatriz(n);

        System.out.println("Tamaño: " + n);

        // Naive
        long start1 = System.nanoTime();
        double[][] C1 = naiveMultiply(A, B);
        long end1 = System.nanoTime();
        System.out.println("Naive: " + (end1 - start1) / 1_000_000.0 + " ms");

        // Strassen (solo potencias de 2)
        if ((n & (n - 1)) == 0) {
            long start2 = System.nanoTime();
            double[][] C2 = strassenMultiply(A, B);
            long end2 = System.nanoTime();
            System.out.println("Strassen: " + (end2 - start2) / 1_000_000.0 + " ms");
        } else {
            System.out.println("Strassen no se ejecuta (n no es potencia de 2)");
        }
    }

    public static void main(String[] args) {
        int[] tamanos = {1024};
        for (int n : tamanos) {
            medirRendimiento(n);
            System.out.println();
        }
    }
}

