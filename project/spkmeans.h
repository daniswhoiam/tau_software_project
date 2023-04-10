#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void print_matrix(double **matrix, int N)
{
  int i, j;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      printf("%.6E\t", matrix[i][j]);
    }
    printf("\n");
  }
}

double *diagonalize_matrix(double **matrix, int N)
{
  int i;
  double *res = malloc(N * sizeof(double));

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
  double **W = malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
  {
    W[i] = malloc(N * sizeof(double));
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
  double **D = malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
  {
    D[i] = malloc(N * sizeof(double));
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


double **identity_matrix(int N)
{
  double **i_m;
  int i, j;

  i_m = malloc(N * sizeof(double *));
  for (i = 0; i < N; i++)
  {
    i_m[i] = malloc(N * sizeof(double));
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
  static int maxIndices[2];

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

  // Case of i < k
  for (i = 0; i < k; i++)
  {
    temp = A[i][k];
    A[i][k] = temp - s * (A[i][l] + tau * temp);
    A[i][l] = A[i][l] + s * (temp - tau * A[i][l]);
  }
  // Case of k < i < l
  for (i = k + 1; i < l; i++)
  {
    temp = A[k][i];
    A[k][i] = temp - s * (A[i][l] + tau * A[k][i]);
    A[i][l] = A[i][l] + s * (temp - tau * A[i][l]);
  }
  // Case of i > l
  for (i = l + 1; i < N; i++)
  {
    temp = A[k][i];
    A[k][i] = temp - s * (A[l][i] + tau * temp);
    A[l][i] = A[l][i] + s * (temp - tau * A[l][i]);
  }
  // Update transformation matrix
  for (i = 0; i < N; i++)
  {
    temp = P[i][k];
    P[i][k] = temp - s * (P[i][l] + tau * P[i][k]);
    P[i][l] = P[i][l] + s * (temp - tau * P[i][l]);
  }
}

double *jacobi(double **A, double ***eigenvectors, int N, int maxRot, double tol)
{
  int i, k, l;
  int *maxCoords;
  double **P;
  double aMax;

  maxRot = 5 * pow(N, 2.0);
  P = identity_matrix(N);

  for (i = 0; i < maxRot; i++)
  {
    maxCoords = maxElem(A, N);
    k = maxCoords[0];
    l = maxCoords[1];
    aMax = A[k][l];
    if (aMax < tol)
    {
      *eigenvectors = P;
      return diagonalize_matrix(A, N);
    }
    else
    {
      rotate(A, P, k, l, N);
    }
  }
  printf("Jacobi method did not converge.");
}