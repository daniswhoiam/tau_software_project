import sys
import numpy as np
import pandas as pd
import myspkmeanssp

def max_eigengap(eigenvalues):
    np.sort(eigenvalues)
    eigengaps = np.empty(len(eigenvalues) - 1)
    for i in range(0, len(eigenvalues) - 1):
        eigengaps[i] = abs(eigenvalues[i] - eigenvalues[i + 1])
    return np.argmax(eigengaps)

def eigengap_heuristic(matrix, N, dim):
    matrix = matrix.tolist()
    #wadjm = myspkmeanssp.wam(matrix, N, dim)
    #diagdem = myspkmeanssp.ddg(wadjm, N)
    #laplac = myspkmeanssp.gl(diagdem, wadjm, N)
    jacobi_result = myspkmeanssp.jacobi(matrix, N, dim, 100, 1.0e-6)
    eigenvalues = jacobi_result[0]
    result = max_eigengap(eigenvalues)

    return result

def print_matrix(matrix, N):
    for i in range(0, N):
        print_list = list(np.around(np.array(matrix[i]),4))
        print(', '.join(map(str, print_list)))

def euclid_dist(row, centroid):
    return np.linalg.norm(row - centroid)

def init_centroids(K, df):
    indices = np.zeros(K)

    # Initializing first centroid
    index = np.random.randint(low=0, high=df.shape[0])
    indices[0] = index
    centroids = df.iloc[[index]]  # DataFrame to save the centroids

    # Initializing other K-1 centroids
    for k in range(1, K):
        # distances, D(x), holds the distance for each point to its closes centroid
        distances = df.apply(
            lambda row: min([euclid_dist(row, centroids.iloc[i]) for i in range(centroids.shape[0])]), axis=1)
        sum = np.sum(distances)
        distribution = distances.apply(lambda row: row / sum)

        new_index = np.random.choice(a=df.shape[0], p=distribution)  # df.shape[0] = N, so a=np.arange(N)
        new_centr = df.iloc[[new_index]]  # double brackets means return type is df, not series
        indices[k] = new_index

        # Update centroids
        centroids = pd.concat([centroids, new_centr], ignore_index=True)

    return indices, centroids

if __name__ == "__main__":

    np.random.seed(0)

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
            goal = sys.argv[1]
            file_path = sys.argv[2]
            matrix = np.loadtxt(file_path, delimiter = ",")
            N = len(matrix)
            dim = matrix.shape[1]
            k = eigengap_heuristic(matrix, N, dim)
        except:
            print("An error has occured!")
            exit()

    matrix = matrix.tolist()

    if goal == "spk":
        wadjm = myspkmeanssp.wam(matrix, N, dim)
        diagdem = myspkmeanssp.ddg(wadjm, N)
        laplac = myspkmeanssp.gl(diagdem, wadjm, N)
        jacobi_result = myspkmeanssp.jacobi(matrix, N, dim, 100, 1.0e-6)
        eigenvalues = jacobi_result[0]
        eigenvectors = jacobi_result[1]
        sorted_eigenvectors = [x for _, x in sorted(zip(eigenvalues, eigenvectors))]
        selected_eigenvectors = sorted_eigenvectors[0:(k-1)]
        transposed_eigenvectors = np.array(selected_eigenvectors).transpose().tolist()
        indices, centroids = init_centroids(k, pd.DataFrame(transposed_eigenvectors))
        spk_result = myspkmeanssp.spk(transposed_eigenvectors, centroids.values.tolist(), N, len(transposed_eigenvectors[0]), k, 100, 1.0e-6)
        print(', '.join(map(str, indices)))
        print_matrix(spk_result, k)


    if goal == "wam":
        wadjm = myspkmeanssp.wam(matrix, N, dim)
        print_matrix(wadjm, N)

    if goal == "ddg":
        diagdem = myspkmeanssp.ddg(matrix, N)
        print_matrix(diagdem, N)

    if goal == "gl":
        wadjm = myspkmeanssp.wam(matrix, N, dim)
        diagdem = myspkmeanssp.ddg(wadjm, N)
        laplac = myspkmeanssp.gl(diagdem, wadjm, N)
        print_matrix(laplac, N)

    if goal == "jacobi":
        jacobi_result = myspkmeanssp.jacobi(matrix, N, dim, 100, 1.0e-6)
        eigenvalues = jacobi_result[0]
        eigenvectors = jacobi_result[1]
        print_matrix(eigenvalues, 0)
        # as columns???
        print_matrix(eigenvectors, N)
