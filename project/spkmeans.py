import sys
import math
import numpy as np
import pandas as pd
import myspkmeanssp


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


def calc_max_offdiag(A):
    n = A.shape[0]
    aMax = 0.0

    for k in range(n - 1):
        for l in range(k + 1, n):
            if abs(A[k, l]) >= aMax:
                aMax = abs(A[k, l])
                i = k
                j = l

    return i, j


def rotate_matrix(A, i, j):
    n = A.shape[0]
    # Numbers for Rotation matrix
    theta = (A[j, j] - A[i, i]) / (2 * A[i, j])
    t = 1 / (abs(theta) + math.sqrt(theta**2 + 1))
    if theta < 0.0:
        t *= -1
    c = 1 / (math.sqrt(t**2 + 1))
    s = t * c

    A_new = A.copy()

    for r in range(n):
        if r != i and r != j:
            A_new[r][i] = c * A[r][i] - s * A[r][j]
            A_new[r][j] = c * A[r][j] + s * A[r][i]
    A_new[i][i] = c**2 * A[i][i] + s**2 * A[j][j] - 2 * s * c * A[i][j]
    A_new[j][j] = s**2 * A[i][i] + c**2 * A[j][j] + 2 * s * c * A[i][j]
    A_new[i][j] = 0.0

    return A_new

""" def jacobi_eigenvalue(A, tol=1.0e-10, max_iter=1000):
    n = A.shape[0]
    V = np.identity(n)
    P = A.copy()

    # Maximum iteration condition
    for m in range(max_iter):
        # Max Offdiagonal Absolute value
        i, j = calc_max_offdiag(A)

        # Numbers for Rotation matriprint(eigenvalues)
    print(eigenvectors)x
        theta = (A[j, j] - A[i, i]) / (2 * A[i, j])
        t = 1 / (abs(theta) + math.sqrt(theta**2 + 1))
        if theta < 0.0:
            t *= -1
        c = 1 / (math.sqrt(t**2 + 1))
        s = t * c

        # Rotation matrix
        P[i, i], P[j, j] = c, c
        P[i, j], P[j, i] = -s, s

        # For tolerance breaking condition
        off_A = np.sum(np.abs(A - np.diag(np.diag(A))) ** 2)

        # Rotation
        A = rotate_matrix(A, i, j)
        V = V @ P

        # For tolerance breaking condition
        off_A_new = np.sum(np.abs(A - np.diag(np.diag(A))) ** 2)

        if off_A - off_A_new <= tol:
            break

    return np.diag(A), V """

def jacobi(a,tol = 1.0e-6): # Jacobi method

    def maxElem(a): # Find largest off-diag. element a[k,l]
        n = len(a)
        aMax = 0.0
        for i in range(n-1):
            for j in range(i+1,n):
                if abs(a[i,j]) >= aMax:
                    aMax = abs(a[i,j])
                    k = i; l = j
        return aMax,k,l

    def rotate(a,p,k,l): # Rotate to make a[k,l] = 0
        n = len(a)
        aDiff = a[l,l] - a[k,k]
        if abs(a[k,l]) < abs(aDiff)*1.0e-36: t = a[k,l]/aDiff
        else:
            phi = aDiff/(2.0*a[k,l])
            t = 1.0/(abs(phi) + math.sqrt(phi**2 + 1.0))
            if phi < 0.0: t = -t
        c = 1.0/math.sqrt(t**2 + 1.0); s = t*c
        tau = s/(1.0 + c)
        temp = a[k,l]
        a[k,l] = 0.0
        a[k,k] = a[k,k] - t*temp
        a[l,l] = a[l,l] + t*temp
        for i in range(k):      # Case of i < k
            temp = a[i,k]
            a[i,k] = temp - s*(a[i,l] + tau*temp)
            a[i,l] = a[i,l] + s*(temp - tau*a[i,l])
        for i in range(k+1,l):  # Case of k < i < l
            temp = a[k,i]
            a[k,i] = temp - s*(a[i,l] + tau*a[k,i])
            a[i,l] = a[i,l] + s*(temp - tau*a[i,l])
        for i in range(l+1,n):  # Case of i > l
            temp = a[k,i]
            a[k,i] = temp - s*(a[l,i] + tau*temp)
            a[l,i] = a[l,i] + s*(temp - tau*a[l,i])
        for i in range(n):      # Update transformation matrix
            temp = p[i,k]
            p[i,k] = temp - s*(p[i,l] + tau*p[i,k])
            p[i,l] = p[i,l] + s*(temp - tau*p[i,l])
        
    n = len(a)
    maxRot = 5*(n**2)       # Set limit on number of rotations
    p = np.identity(n)*1.0     # Initialize transformation matrix
    for i in range(maxRot): # Jacobi rotation loop 
        aMax,k,l = maxElem(a)
        if aMax < tol: return np.diagonal(a),p
        rotate(a,p,k,l)
    print('Jacobi method did not converge')

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
            dim = matrix.shape[1]
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
            dim = matrix.shape[1]
        except:
            print("An error has occured!")
            exit()

    # Make Weight Adjusted Matrix
    wadjm = make_wadjm(matrix, N)
    
    # Make Diagonal Degree Matrix
    diagdem = make_diagdem(wadjm, N)
    
    # Make Laplacian
    laplac = np.subtract(diagdem, wadjm)
    
    # Eigenvalues and eigenvectors
    # CAREFUL: CHANGES ORIGINAL MATRIX
    # jacobi_result = jacobi(matrix)
    # eigenvalues = jacobi_result[0]
    # eigenvectors = jacobi_result[1]
    
    if (goal == "wam"):
        matrix = matrix.tolist()
        wadjm = myspkmeanssp.wam(matrix, N, dim)
        print(wadjm)
