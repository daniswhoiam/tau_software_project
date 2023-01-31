#include <stdio.h>
#include <math.h>
#include <stdlib.h>

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

void print_matrix(double **matrix, int N)
{
  int i, j;
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      printf("%.3f\t", matrix[i][j]);
    }
    printf("\n");
  }
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
  double **wadjm, **diagdem;

  /* READING INPUT START */
  if (argc != 3)
  {
    printf("Wrong number of arguments!\n");
    return 1;
  }

  goal = argv[1];
  printf("%s", goal);
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

  diagdem = make_diagdem(wadjm, r);

  return 0;
}