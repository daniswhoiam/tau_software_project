import sys
import os
import math

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
  for i in range(0, len(list)-1):
    sl = slice(1, dim)
    res_dict[list[i][0]] = list[i][sl]
  return res_dict


if __name__ == "__main__":

  arg_num = len(sys.argv)
  # Check number of args
  if arg_num < 5 or arg_num > 6:
    print("Invalid number of arguments!")
    exit()
  has_iter = arg_num == 6

  data_1 = read_data(os.path.join(os.getcwd(), sys.argv[4] if has_iter else sys.argv[3]))
  data_2 = read_data(os.path.join(os.getcwd(), sys.argv[5] if has_iter else sys.argv[4]))
  N = len(data_1) # Both files have the same length

  # Check number of clusters
  k = int(sys.argv[1])
  if k <= 1 or k >= N:
    print("Invalid number of clusters!")
    exit()

  # Check number of iterations
  iter = int(sys.argv[2]) if has_iter else 300 # Default: 300
  if iter <= 1 or iter >= 1000:
    print("Invalid number of iteration!")
    exit()

  # Perform inner join
  dict_1 = convert(data_1)
  dict_2 = convert(data_2)
  dict_joined = {k: dict_1[k] + dict_2[k] for k in dict_1 if k in dict_2}