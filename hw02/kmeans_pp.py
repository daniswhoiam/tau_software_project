import sys
import os
import math
import numpy as np


def read_data(path):
    with open(path) as file:
        lines = file.readlines()

    lines = [line.replace("\n", "") for line in lines]
    data_str = [line.split(",") for line in lines]
    data = [[float(val) for val in coord] for coord in data_str]
    return data


def convert(list):
    res_dict = {}
    dim = len(list[0])
    for i in range(0, len(list) - 1):
        sl = slice(1, dim)
        res_dict[list[i][0]] = list[i][sl]
    return res_dict


def euclid_dist(x, centr):
    dist = 0
    for a, b in zip(x, centr):
        dist = dist + (a - b) ** 2
    return math.sqrt(dist)


if __name__ == "__main__":

    arg_num = len(sys.argv)
    # Check number of args
    if arg_num < 5 or arg_num > 6:
        print("Invalid number of arguments!")
        exit()
    has_iter = arg_num == 6

    data_1 = read_data(os.path.join(os.getcwd(), sys.argv[4] if has_iter else sys.argv[3]))
    data_2 = read_data(os.path.join(os.getcwd(), sys.argv[5] if has_iter else sys.argv[4]))
    N = len(data_1)  # Both files have the same length

    # Check number of clusters
    k = int(sys.argv[1])
    if k <= 1 or k >= N:
        print("Invalid number of clusters!")
        exit()

    # Check number of iterations
    iter = int(sys.argv[2]) if has_iter else 300  # Default: 300
    if iter <= 1 or iter >= 1000:
        print("Invalid number of iteration!")
        exit()

    # Perform inner join
    dict_1 = convert(data_1)
    dict_2 = convert(data_2)
    dict_joined = {k: dict_1[k] + dict_2[k] for k in dict_1 if k in dict_2}

    # Sort in ascending order
    dict_sorted = dict(sorted(dict_joined.items()))

    # k-means ++

    # Choose one center uniformly at random among the data points.
    np.random.seed(0)
    rand_center_key = np.random.choice(list(dict_sorted.keys()))
    rand_center = dict_joined[rand_center_key]
    chosen_points = [rand_center]
    del dict_sorted[rand_center_key]  # Since chosen, remove from choice pool

    # For each data point x not chosen yet, compute D(x), the distance between x and the nearest center that has already been chosen.
    dist = {}
    # Loop over every data point
    for key in dict_sorted:
        current_point = dict_sorted[key]
        dist[key] = euclid_dist(current_point, chosen_points[0])
        # Search for D(x) with nearest center that has already been chosen
        for point in chosen_points:
            inter_dist = euclid_dist(current_point, point)
            if inter_dist < dist[key]:
                dist[key] = inter_dist

    # Calculate probability list
    dist_sum = sum(dist.values())
    p_x = []
    for value in dist.values():
        proportion = value / dist_sum
        p_x.append(proportion)

    # Choose one new data point at random as a new center, using a weighted probability distri-
    # bution where a point x is chosen with probability proportional to P(x1)
    rand_center_key = np.random.choice(list(dict_sorted.keys()), p=p_x)
    rand_center = dict_sorted[rand_center_key]
    chosen_points.append(rand_center)
    del dict_sorted(rand_center_key)

    # Repeat Steps 2 and 3 until k centers have been chosen

    # Now that the initial centers have been chosen, proceed using standard k-means clustering.
