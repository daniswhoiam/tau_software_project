import sys
import os
import math
from statistics import mean

def print_neat(centroids):
    for c in centroids:
        line = ""
        for i, value in enumerate(c):
            if i != len(c)-1:
                line += f"{value:.4f},"
            else:
                line += f"{value:.4f}"
        print(line)


def read_data(path):
    with open(path) as file:
        lines = file.readlines()

    lines = [line.replace("\n", "") for line in lines]
    data_str = [line.split(",") for line in lines]
    data = [[float(val) for val in coord] for coord in data_str]
    return data


def euclid_dist(x, centr):
  dist = 0
  for a, b in zip(x, centr):
    dist = dist + (a - b)**2
  return math.sqrt(dist)


if __name__ == "__main__":

    # ASSERTIONS ON INPUT

    # When max_iter is given but invalid (not natural number)
    if len(sys.argv) == 4:
        try:
            max_iter = int(sys.argv[2])
            data = read_data(os.path.join(os.getcwd(), sys.argv[3]))
        except:
            print("Invalid number of iterations!")
            exit()

    # When max_iter is not given
    else:
        max_iter = 200
        data = read_data(os.path.join(os.getcwd(), sys.argv[2]))

    # Variables
    dim = len(data[0])
    N = len(data)

    # Check if k is an integer
    try:
        k = int(sys.argv[1])
    except:
        print("Invalid number of clusters!")
        exit()

    if k >= N:
        print("Invalid number of clusters!")
        exit()

    if max_iter >= 1000:
        print("Invalid number of iterations!")
        exit()

    print(f"k = {k}")
    print(f"Max iterations = {max_iter}")

    epsilon = 0.001
    curr_iter = 0
    max_diff = 1

    # Initialize centroids
    centroids = data[0:k]

    # k-means algorithm
    while (curr_iter < max_iter and epsilon < max_diff):
        cluster_values = [[] for i in range(k)]

        for x in data:
            distances = [euclid_dist(x, c) for c in centroids]
            min_centroid_index = distances.index(min(distances))
            cluster_values[min_centroid_index].append(x)

        # Update centroids
        cluster_means = []  # to store the mean coordinates for all clusters

        for i in range(k):
            dim_means = []  # to store dimension means for a single cluster
            clust_vals = cluster_values[i]

            for d in range(dim):
                dim_vals = [clust_vals[j][d] for j in range(len(clust_vals))]
                dim_means.append(mean(dim_vals))

            cluster_means.append(dim_means)

        diffs = [euclid_dist(m, c) for m, c in zip(cluster_means, centroids)]

        max_diff = max(diffs)

        centroids = cluster_means
        curr_iter += 1

    print_neat(centroids)
