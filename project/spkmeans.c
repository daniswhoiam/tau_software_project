#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "spkmeans.h"

#define MAX_ITER 100
#define TOL 1e-5

double **subtract_matrices(double **mat1, double **mat2, int N)
{
  /* Counters */
  int i = 0, j = 0;

  /* Creating result matrix */
  double **res = malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
  {
    res[i] = malloc(N * sizeof(double));
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

double *jacobi_eigenvalue(double **A, int N, double ***eigenvectors)
{
  int i, j, k, p, q;
  double max_offdiag, theta, t, c, s, off_A, off_A_new;
  double *eigenvalues;
  double **P;

  eigenvalues = malloc(N * sizeof(double));
  /* Rotation Matrix */
  P = malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
  {
    P[i] = malloc(N * sizeof(double));
  }
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      P[i][j] = (i == j) ? 1 : 0;
    }
  }

  for (k = 0; k < MAX_ITER; k++)
  {
    max_offdiag = 0.0;
    for (p = 0; p < N; p++)
    {
      for (q = 0; q < N; q++)
      {
        if (fabs(A[p][q]) > max_offdiag)
        {
          max_offdiag = fabs(A[p][q]);
          i = p;
          j = q;
        }
      }
    }

    theta = (A[j][i] - A[i][i]) / (2 * A[i][j]);
    t = 1 / (fabs(theta) + sqrt(pow(theta, 2) + 1));
    if (theta < 0)
      t *= -1;
    c = 1 / (sqrt(pow(t, 2) + 1));
    s = t * c;

    P[i][i] = c;
    P[j][j] = c;
    P[i][j] = -s;
    P[j][i] = s;

    off_A = off(A, N);

    A = multiply_matrices(multiply_matrices(transpose_matrix(P, N), A, N), P, N);

    *eigenvectors = multiply_matrices(*eigenvectors, P, N);

    off_A_new = off(A, N);

    if (off_A - off_A_new <= TOL)
      break;
  }

  eigenvalues = diagonalize_matrix(A, N);
  return eigenvalues;
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
  printf("%s\n", goal);
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

  data = malloc(r * sizeof(double *));
  for (i = 0; i < r; i++)
  {
    data[i] = malloc(c * sizeof(double));
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

  wadjm = make_wadjm(data, r, c);
  print_matrix(wadjm, r);

  diagdem = make_diagdem(wadjm, r);

  laplace = subtract_matrices(diagdem, wadjm, r);

  eigenvectors = malloc(r * sizeof(double *));
  for (i = 0; i < r; i++)
  {
    eigenvectors[i] = malloc(r * sizeof(double));
  }
  for (i = 0; i < r; i++)
  {
    for (j = 0; j < r; j++)
    {
      eigenvectors[i][j] = (i == j) ? 1 : 0;
    }
  }

  eigenvalues = malloc(r * sizeof(double));
  eigenvalues = jacobi_eigenvalue(laplace, r, &eigenvectors);

  for (i = 0; i < r; i++)
  {
    printf("%E, ", eigenvalues[i]);
  }

  free(eigenvectors);
  free(eigenvalues);

  return 0;
}