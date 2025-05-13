import numpy as np
import time

def generar_matriz(n):
    return np.random.rand(n, n) * 10  # Matriz con valores flotantes aleatorios

# Algoritmo ingenuo
def naive_multiply(A, B):
    n = A.shape[0]
    C = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            for k in range(n):
                C[i][j] += A[i][k] * B[k][j]
    return C

# Suma de matrices
def sumar(A, B):
    return A + B

# Resta de matrices
def restar(A, B):
    return A - B

# Algoritmo de Strassen
def strassen(A, B):
    n = A.shape[0]
    if n == 1:
        return A * B

    mid = n // 2

    A11 = A[:mid, :mid]
    A12 = A[:mid, mid:]
    A21 = A[mid:, :mid]
    A22 = A[mid:, mid:]

    B11 = B[:mid, :mid]
    B12 = B[:mid, mid:]
    B21 = B[mid:, :mid]
    B22 = B[mid:, mid:]

    M1 = strassen(sumar(A11, A22), sumar(B11, B22))
    M2 = strassen(sumar(A21, A22), B11)
    M3 = strassen(A11, restar(B12, B22))
    M4 = strassen(A22, restar(B21, B11))
    M5 = strassen(sumar(A11, A12), B22)
    M6 = strassen(restar(A21, A11), sumar(B11, B12))
    M7 = strassen(restar(A12, A22), sumar(B21, B22))

    C11 = M1 + M4 - M5 + M7
    C12 = M3 + M5
    C21 = M2 + M4
    C22 = M1 - M2 + M3 + M6

    top = np.hstack((C11, C12))
    bottom = np.hstack((C21, C22))
    return np.vstack((top, bottom))

# Medición de tiempo
def medir_tiempos(n):
    print(f"\nTamaño: {n}x{n}")

    A = generar_matriz(n)
    B = generar_matriz(n)

    # Naive
    inicio = time.time()
    C1 = naive_multiply(A, B)
    fin = time.time()
    print(f"Naive:    {round((fin - inicio)*1000, 2)} ms")

    # Strassen solo si es potencia de 2
    if (n & (n - 1)) == 0:
        inicio = time.time()
        C2 = strassen(A, B)
        fin = time.time()
        print(f"Strassen: {round((fin - inicio)*1000, 2)} ms")
    else:
        print("Strassen no ejecutado (n no es potencia de 2)")

if __name__ == "__main__":
    tamanos = [1024]  # Puedes agregar 1024 si tu PC lo permite
    for n in tamanos:
        medir_tiempos(n)

