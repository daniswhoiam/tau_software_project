#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "spkmeans.h"

#define MAX_ITER 100
#define TOL 1e-6

void print_matrix(double **matrix, int N)
{
  int i, j;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if (j != N - 1)
      {
        printf("%.4f, ", matrix[i][j]);
      }
      else
      {
        printf("%.4f", matrix[i][j]);
      }
    }
    printf("\n");
  }
}

double *diagonalize_matrix(double **matrix, int N)
{
  int i;
  double *res = calloc(N, sizeof(double));

  for (i = 0; i < N; i++)
  {
    res[i] = matrix[i][i];
  }

  return res;
}

double **make_wadjm(double **data, int N, int dim)
{
  /* Counters */
  int i = 0, j = 0, k = 0;

  /* Creating array / matrix */
  double **W = calloc(N, sizeof(double *));
  for (i = 0; i < N; i++)
  {
    W[i] = calloc(N, sizeof(double));
  }

  /* Loop over input matrix */
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if (i != j)
      {
        /* Calculate distance */
        double dist = 0.0;
        for (k = 0; k < dim; k++)
        {
          dist += (data[i][k] - data[j][k]) * (data[i][k] - data[j][k]);
        }
        /* Save Weighted Adjacency */
        W[i][j] = exp(-(dist / 2));
      }
    }
  }

  return W;
}

double **make_diagdem(double **wadjm, int N)
{
  /* Counters */
  int i = 0, j = 0;

  /* Create matrix */
  double **D = calloc(N, sizeof(double *));
  for (i = 0; i < N; i++)
  {
    D[i] = calloc(N, sizeof(double));
  }

  /* Fill up matrix */
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      D[i][i] += wadjm[i][j];
      if (i != j)
      {
        D[i][j] = 0.0;
      }
    }
  }

  return D;
}

double **subtract_matrices(double **mat1, double **mat2, int N)
{
  /* Counters */
  int i = 0, j = 0;

  /* Creating result matrix */
  double **res = calloc(N, sizeof(double *));
  for (i = 0; i < N; i++)
  {
    res[i] = calloc(N, sizeof(double));
  }

  /* Subtraction */
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      res[i][j] = mat1[i][j] - mat2[i][j];
    }
  }

  return res;
}

double **identity_matrix(int N)
{
  double **i_m;
  int i, j;

  i_m = calloc(N, sizeof(double *));
  for (i = 0; i < N; i++)
  {
    i_m[i] = calloc(N, sizeof(double));
  }

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if (i == j)
      {
        i_m[i][j] = 1.0;
      }
      else
      {
        i_m[i][j] = 0.0;
      }
    }
  }

  return i_m;
}

int *maxElem(double **A, int N)
{
  int i, j, k, l;
  double aMax = 0.0;
  int *maxIndices;

  maxIndices = calloc(2, sizeof(int));

  k = -1;
  l = -1;

  for (i = 0; i < N - 1; i++)
  {
    for (j = i + 1; j < N; j++)
    {
      if (fabs(A[i][j]) >= aMax)
      {
        aMax = fabs(A[i][j]);
        k = i;
        l = j;
      }
    }
  }

  maxIndices[0] = k;
  maxIndices[1] = l;

  return maxIndices;
}

void rotate(double **A, double **P, int k, int l, int N)
{
  double aDiff, t, phi, c, s, tau, temp;
  int i;

  aDiff = A[l][l] - A[k][k];

  if (fabs(A[k][l]) < fabs(aDiff) * 1.0E-36)
  {
    t = A[k][l] / aDiff;
  }
  else
  {
    phi = aDiff / (2.0 * A[k][l]);
    t = 1.0 / (fabs(phi) + sqrt(pow(phi, 2.0) + 1.0));
    if (phi < 0.0)
    {
      t = -t;
    }
  }

  c = 1.0 / sqrt(pow(t, 2.0) + 1.0);
  s = t * c;
  tau = s / (1.0 + c);

  temp = A[k][l];
  A[k][l] = 0.0;
  A[k][k] = A[k][k] - t * temp;
  A[l][l] = A[l][l] + t * temp;

  /* Case of i < k */
  for (i = 0; i < k; i++)
  {
    temp = A[i][k];
    A[i][k] = temp - s * (A[i][l] + tau * temp);
    A[i][l] = A[i][l] + s * (temp - tau * A[i][l]);
  }
  /* Case of k < i < l */
  for (i = k + 1; i < l; i++)
  {
    temp = A[k][i];
    A[k][i] = temp - s * (A[i][l] + tau * A[k][i]);
    A[i][l] = A[i][l] + s * (temp - tau * A[i][l]);
  }
  /* Case of i > l */
  for (i = l + 1; i < N; i++)
  {
    temp = A[k][i];
    A[k][i] = temp - s * (A[l][i] + tau * temp);
    A[l][i] = A[l][i] + s * (temp - tau * A[l][i]);
  }
  /* Update transformation matrix */
  for (i = 0; i < N; i++)
  {
    temp = P[i][k];
    P[i][k] = temp - s * (P[i][l] + tau * P[i][k]);
    P[i][l] = P[i][l] + s * (temp - tau * P[i][l]);
  }
}

double *jacobi(double **A, double ***eigenvectors, int N)
{
  int i, k, l;
  int *maxCoords;
  double **P;
  double aMax;

  P = identity_matrix(N);

  for (i = 0; i < MAX_ITER; i++)
  {
    maxCoords = maxElem(A, N);
    k = maxCoords[0];
    l = maxCoords[1];
    aMax = A[k][l];
    if (aMax < TOL)
    {
      *eigenvectors = P;
      return diagonalize_matrix(A, N);
    }
    else
    {
      rotate(A, P, k, l, N);
    }
  }
  return diagonalize_matrix(A, N);
}

/* https://stackoverflow.com/questions/36940643/sorting-an-array-of-double-in-c */
static int compare(const void *a, const void *b)
{
  if (*(double *)a > *(double *)b)
    return 1;
  else if (*(double *)a < *(double *)b)
    return -1;
  else
    return 0;
}

int max_eigengap(double *eigenvalues, int N)
{
  int i, result;
  double max;
  double *eigengaps;

  /* Sort Ascendingly */
  qsort(eigenvalues, N, sizeof(double), compare);

  /* Calculate Eigengaps */
  eigengaps = calloc(N - 1, sizeof(double));
  for (i = 0; i < N - 1; i++)
  {
    eigengaps[i] = fabs(eigenvalues[i] - eigenvalues[i + 1]);
  }

  /* Determine Argmax */
  result = -1;
  max = 0.0;
  for (i = 0; i < N - 1; i++)
  {
    if (eigengaps[i] > max)
    {
      max = eigengaps[i];
      result = i;
    }
  }

  return result;
}

double **fit(double **data, double **centroids, int r, int c, int K, int iter, double epsilon)
{
  /* Dimensions */
  int count;

  /* Iterators */
  int i = 0, j = 0, k = 0;

  /* For centroids */
  double **rel, ***arr_rel;

  /* For loop */
  int iteration_number = 0;
  int treshold_reached = 0;
  double *prev_centroid;
  double *new_centroid;
  double *centroid_sum;

  /* Calculating Dimensions */
  count = c * r;

  /* Set number of iterations */
  if (iter < 2 || iter > 999)
  {
    printf("Invalid maximum iteration!\n");
    return NULL;
  }
  else
  {
    iter = 200;
  }

  /* Initiate centroid - point arrays*/
  rel = calloc(2 * count, sizeof(double *));
  arr_rel = calloc(count, sizeof(double **));
  for (k = 0; k < count; k++)
  {
    arr_rel[k] = rel + 2 * k;
  }

  prev_centroid = calloc(c, sizeof(double));
  new_centroid = calloc(c, sizeof(double));
  centroid_sum = calloc(c, sizeof(double));
  /* LOOP THE NEXT TWO STEPS*/
  while (treshold_reached == 0 && iteration_number < iter)
  {
    treshold_reached = 1;

    /* Assign data points to nearest cluster */

    for (i = 0; i < r; i++)
    {
      double diff = 999.00;
      int cent = 0;
      /* Iterate over centroids*/
      for (k = 0; k < K; k++)
      {
        double inter_sum = 0.0;
        double inter_diff = 0.0;
        for (j = 0; j < c; j++)
        {
          inter_sum += pow((data[i][j] - centroids[k][j]), 2.0);
        }
        inter_diff = sqrt(inter_sum);
        /* Save difference and nearest centroid*/
        if (inter_diff < diff)
        {
          diff = inter_diff;
          cent = k;
        }
      }
      arr_rel[i][0] = data[i];
      arr_rel[i][1] = centroids[cent];
    }

    /* Update centroids */
    for (k = 0; k < K; k++)
    {
      double centroid_distance = 0.0;
      int k_count = 0;

      for (j = 0; j < c; j++)
      {
        prev_centroid[j] = centroids[k][j];
      }

      for (i = 0; i < r; i++)
      {
        if (arr_rel[i][1] == centroids[k])
        {
          for (j = 0; j < c; j++)
          {
            centroid_sum[j] += arr_rel[i][0][j];
          }
          k_count++;
        }
      }
      for (j = 0; j < c; j++)
      {
        centroid_sum[j] = centroid_sum[j] / k_count;
        centroids[k][j] = centroid_sum[j];
      }

      for (j = 0; j < c; j++)
      {
        new_centroid[j] = centroids[k][j];
      }

      for (j = 0; j < c; j++)
      {
        centroid_distance += pow((prev_centroid[j] - new_centroid[j]), 2);
      }
      centroid_distance = sqrt(centroid_distance);
      if (centroid_distance > epsilon)
      {
        treshold_reached = 0;
      }
      for (j = 0; j < c; j++)
      {
        centroid_sum[j] = 0.0;
      }
    }
    iteration_number++;
  }

  free(prev_centroid);
  free(new_centroid);
  free(centroid_sum);
  free(rel);
  free(arr_rel);

  return centroids;
}

double **transpose_matrix(double **matrix, int N)
{
  /* Counters */
  int i = 0, j = 0;

  double **res = malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
  {
    res[i] = malloc(N * sizeof(double));
  }

  /* Transposing */
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      res[i][j] = matrix[j][i];
    }
  }

  return res;
}

double **multiply_matrices(double **mat1, double **mat2, int N)
{
  /* Counters */
  int i = 0, j = 0, k = 0;

  double value;

  double **res = malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
  {
    res[i] = malloc(N * sizeof(double));
  }

  value = 0.0;
  for (i = 0; i < N; i++)
  {
    double *cur_row = mat1[i];
    for (k = 0; k < N; k++)
    {
      for (j = 0; j < N; j++)
      {
        value = cur_row[j] * mat2[j][k];
      }
      res[i][k] = value;
      value = 0;
    }
  }

  return res;
}

double off(double **matrix, int N)
{
  int i, j;
  double res = 0.0;

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if (i != j)
      {
        res += pow(matrix[i][j], 2);
      }
    }
  }

  return res;
}

int main(int argc, char **argv)
{
  /* Arguments */
  char *goal;
  char *filename;

  /* For data input */
  FILE *textfile;
  int r = 1, c = 1, count = 1;
  double **data;
  char line[256];

  /* Counters */
  int i = 0, j = 0;

  /* Matrices */
  double **wadjm, **diagdem, **laplace;

  /* Eigen */
  double **eigenvectors;
  double *eigenvalues;

  /* READING INPUT START */
  if (argc != 3)
  {
    printf("Wrong number of arguments!\n");
    return 1;
  }

  goal = argv[1];
  filename = argv[2];
  textfile = fopen(filename, "r");
  if (textfile == NULL)
  {
    printf("An error has occured.\n");
    return 1;
  }

  while (fgets(line, sizeof(line), textfile))
  {
    char *l = line;
    double temp;
    int n;

    while (sscanf(l, "%lf %n", &temp, &n) == 1)
    {
      if (r == 1)
        c++;
      l += n;
      count++;
    }
    r++;
  }
  r = r - 1;
  c = c - 1;
  fclose(textfile);

  data = calloc(r, sizeof(double *));
  for (i = 0; i < r; i++)
  {
    data[i] = calloc(c, sizeof(double));
  }

  textfile = fopen(filename, "r");
  if (textfile == NULL)
  {
    printf("An error has occured.\n");
    return 1;
  }

  i = 0;
  j = 0;
  while (fgets(line, sizeof(line), textfile))
  {
    char *l = line;
    double temp;
    int n;

    while (sscanf(l, "%lf %n", &temp, &n) == 1)
    {
      data[i][j] = temp;
      j++;
      l += n;
    }
    j = 0;
    i++;
  }
  fclose(textfile);

  /* READING INPUT END */

  if (strcmp(goal, "wam") == 0)
  {
    wadjm = make_wadjm(data, r, c);
    print_matrix(wadjm, r);
  }

  if (strcmp(goal, "ddg") == 0)
  {
    wadjm = make_wadjm(data, r, c);
    diagdem = make_diagdem(wadjm, r);
    print_matrix(diagdem, r);
  }

  if (strcmp(goal, "gl") == 0)
  {
    wadjm = make_wadjm(data, r, c);
    diagdem = make_diagdem(wadjm, r);
    laplace = subtract_matrices(diagdem, wadjm, r);
    print_matrix(laplace, r);
  }

  if (strcmp(goal, "jacobi") == 0)
  {
    wadjm = make_wadjm(data, r, c);
    diagdem = make_diagdem(wadjm, r);
    laplace = subtract_matrices(diagdem, wadjm, r);
    eigenvectors = calloc(r, sizeof(double *));
    for (i = 0; i < r; i++)
    {
      eigenvectors[i] = calloc(r, sizeof(double));
    }
    for (i = 0; i < r; i++)
    {
      for (j = 0; j < r; j++)
      {
        eigenvectors[i][j] = (i == j) ? 1.0 : 0.0;
      }
    }

    eigenvalues = calloc(r, sizeof(double));
    eigenvalues = jacobi(laplace, &eigenvectors, r);

    for (i = 0; i < r; i++)
    {
      if (i != r - 1)
      {
        printf("%.4f, ", eigenvalues[i]);
      }
      else
      {
        printf("%.4f", eigenvalues[i]);
      }
    }
    printf("\n");
    print_matrix(eigenvectors, r);

    free(eigenvectors);
    free(eigenvalues);
  }

  return 0;
}