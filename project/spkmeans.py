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

    # Make Diagonal Degree Matrix
    diagdem = make_diagdem(wadjm, N)

    # Make Laplacian
    laplac = np.subtract(diagdem, wadjm)

    print(laplac)
