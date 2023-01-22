import sys
import numpy as np
import pandas as pd
import mykmeanssp


def inds_to_str(inds):
    str = ""
    for i in range(inds.shape[0] - 1):
        str = str + f"{inds[i]:.0f},"
    return str + f"{inds[-1]:.0f}"


def read_data(path1, path2):
    df1 = pd.read_csv(path1, index_col=0, header=None)  # make the 'observation index' (ie, key) the row label
    df2 = pd.read_csv(path2, index_col=0, header=None)
    df = df1.merge(df2, left_index=True, right_index=True)
    df = df.sort_values(by=[0], axis="index")

    dim = df.shape[1]  # dimensionality of data
    headers = np.arange(1, dim + 1)
    df.columns = headers  # give each column its dim as header
    return df


def euclid_dist(row, centroid):
    return np.linalg.norm(row - centroid)


# Initializing k centroids for k-means++
def init_centroids_pp(K, df):
    indices = np.zeros(K)

    # Initializing first centroid
    np.random.seed(0)
    index = np.random.randint(low=0, high=df.shape[0])
    indices[0] = index
    centroids = df.iloc[[index]]  # DataFrame to save the centroids

    # Initializing other K-1 centroids
    for k in range(1, K):
        # distances, D(x), holds the distance for each point to its closes centroid
        distances = df.apply(
            lambda row: min([euclid_dist(row, centroids.iloc[i]) for i in range(centroids.shape[0])]), axis=1
        )
        sum = np.sum(distances)
        distribution = distances.apply(lambda row: row / sum)

        new_index = np.random.choice(a=df.shape[0], p=distribution)  # df.shape[0] = N, so a=np.arange(N)
        new_centr = df.iloc[[new_index]]  # double brackets means return type is df, not series
        indices[k] = new_index

        # Update centroids
        centroids = pd.concat([centroids, new_centr], ignore_index=True)

    return indices, centroids


if __name__ == "__main__":

    # ASSERTIONS ON INPUT

    # 6 arguments, so iter is given but could be invalid
    if len(sys.argv) == 6:
        try:
            max_iter = int(sys.argv[2])
            epsilon = float(sys.argv[3])
            df = read_data(sys.argv[4], sys.argv[5])
        except:
            print("Invalid maximum iteration!")
            exit()

    # 5 arguments, so iter is not given
    elif len(sys.argv) == 5:
        max_iter = 300
        epsilon = float(sys.argv[2])
        df = read_data(sys.argv[3], sys.argv[4])

    else:
        print("Invalid number of arguments!")
        exit()

    N = df.shape[0]

    # Check if k is an integer
    try:
        K = int(sys.argv[1])
    except:
        print("Invalid number of clusters!")
        exit()

    if K <= 1 or K >= N:
        print("Invalid number of clusters!")
        exit()

    if max_iter <= 1 or max_iter >= 1000:
        print("Invalid maximum iteration!")
        exit()

    # ALGORITHM

    # Variables
    curr_iter = 0
    max_diff = 1
    dim = df.shape[1]
    indices, centroids = init_centroids_pp(K, df)

    print(inds_to_str(indices))

    arr_sorted = df.values.tolist()
    c = len(arr_sorted[0])
    chosen_points = centroids.values.tolist()
    final_centroids = mykmeanssp.fit(arr_sorted, chosen_points, N, c, K, max_iter, epsilon)

    for centroid in final_centroids:
        rounded = [round(value, 4) for value in centroid]
        print(','.join(str(item) for item in rounded))