import sys
import math
import numpy as np
import pandas as pd


def sq_eq_dist(point1, point2):
    return np.linalg.norm(point1 - point2) ** 2


def make_wadjm(data, N):
    wadjm = np.zeros((N, N))

    for i in range(0, N):
        for j in range(0, N):
            if i != j:
                point1 = data[i]
                point2 = data[j]
                wadjm[i][j] = math.exp(-(sq_eq_dist(point1, point2) / 2))

    return wadjm


def make_diagdem(wadjm, N):
    diagdem = np.zeros((N, N))

    for i in range(0, N):
        for j in range(0, N):
            diagdem[i][i] += wadjm[i][j]

    return diagdem


def jacobi_eigenvalue(A, tol=1e-5, max_iter=100):
    n = A.shape[0]
    V = np.identity(n)

    # Maximum iteration condition
    for m in range(max_iter):
        # Max Offdiagonal Absolute value
        max_offdiag = 0
        i, j = 0, 0
        for p in range(n):
            for q in range(p + 1, n):
                if abs(A[p, q]) > max_offdiag:
                    max_offdiag = abs(A[p, q])
                    i, j = p, q

        # Numbers for Rotation matrix
        theta = (A[j, j] - A[i, i]) / (2 * A[i, j])
        t = 1 / (abs(theta) + math.sqrt(theta**2 + 1))
        if theta < 0:
            t *= -1
        c = 1 / (math.sqrt(t**2 + 1))
        s = t * c

        # Rotation matrix
        P = np.identity(n)
        P[i, i], P[j, j] = c, c
        P[i, j], P[j, i] = -s, s

        # For tolerance breaking condition
        off_A = np.sum(np.abs(A - np.diag(np.diag(A))) ** 2)

        # Rotation
        A = P.T @ A @ P
        V = V @ P

        # For tolerance breaking condition
        off_A_new = np.sum(np.abs(A - np.diag(np.diag(A))) ** 2)

        if off_A - off_A_new <= tol or m == max_iter:
            break

    return np.diag(A), V


if __name__ == "__main__":

    # INPUT CHECK

    # 3 Arguments, k given but could be invalid

    if len(sys.argv) == 4:
        try:
            k = int(sys.argv[1])
            goal = sys.argv[2]
            file_path = sys.argv[3]
            matrix = np.loadtxt(file_path)
            N = len(matrix)
            # Check k
            if k >= N:
                print("Invalid number of clusters!")
                exit()
        except:
            print("An error has occured!")
            exit()

    # 2 Arguments

    elif len(sys.argv) == 3:
        try:
            goal = sys.argv[2]
            file_path = sys.argv[3]
            matrix = np.loadtxt(file_path)
            N = len(matrix)
        except:
            print("An error has occured!")
            exit()

    # Make Weight Adjusted Matrix
    wadjm = make_wadjm(matrix, N)
    print(wadjm)

    # Make Diagonal Degree Matrix
    diagdem = make_diagdem(wadjm, N)

    # Make Laplacian
    laplac = np.subtract(diagdem, wadjm)

    # Eigenvalues and eigenvectors
    jacobi_result = jacobi_eigenvalue(laplac)
    eigenvalues = jacobi_result[0]
    eigenvectors = jacobi_result[1]

    
