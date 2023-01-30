import sys
import numpy as np

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
