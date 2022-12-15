#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  long K;
  long iter;
  FILE *textfile;

  /* For numbers import*/
  int r = 1, c = 1;
  double *p, **arr;
  int count = 0;
  int i = 0, j = 0, k;
  char line[256];

  /* For centroids */
  double *p_c, **arr_c;
  double **rel, ***arr_rel;

  /* Validate Input */
  if (argc < 3 || argc > 4)
  {
    printf("Wrong number of arguments!");
    return 1;
  }

  textfile = fopen("./input_1.txt", "r");
  if (textfile == NULL)
  {
    printf("An error has occured.\n");
    return 1;
  }

  /* Import Input */

  /*Read first row, determine column number*/
  while (fgets(line, sizeof(line), textfile))
  {
    char *l = line;
    double temp;
    int n;
    while (sscanf(l, "%lf,%n", &temp, &n) == 1)
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

  /*Double Array erstellen (columns)*/
  p = calloc(r * c, sizeof(double));
  /*Array für double Arrays erstellen (rows)*/
  arr = calloc(r, sizeof(double *));
  for (k = 0; k < r; k++)
  {
    arr[k] = p + k * c;
  }

  textfile = fopen("./input_1.txt", "r");
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
    while (sscanf(l, "%lf,%n", &temp, &n) == 1)
    {
      arr[i][j] = temp;
      j++;
      l += n;
    }
    j = 0;
    i++;
  }

  fclose(textfile);

  /* Set number of clusters */
  K = strtol(argv[1], NULL, 10);
  if (K < 2 || K > (count - 1))
  {
    printf("Invalid number of clusters!");
    return 1;
  }

  /* Set number of iterations */
  if (argc == 4)
  {
    iter = strtol(argv[2], NULL, 10);
    if (iter < 2 || iter > 999)
    {
      printf("Invalid maximum iteration!");
      return 1;
    }
  }
  else
  {
    iter = 200;
  }

  /* Initialize centroids */
  p_c = calloc(K * c, sizeof(double));
  arr_c = calloc(K, sizeof(double *));
  for (k = 0; k < K; k++)
  {
    arr_c[k] = p_c + k * c;
  }
  for (i = 0; i < K; i++)
  {
    for (j = 0; j < c; j++)
    {
      arr_c[i][j] = arr[i][j];
    }
  }

  /* Initiate centroid - point arrays*/
  rel = calloc(2 * count, sizeof(double *));
  arr_rel = calloc(count, sizeof(double **));
  for (k = 0; k < count; k++)
  {
    arr_rel[k] = rel + 2 * k;
  }
  for (i = 0; i < r; i++)
  {
    double diff = 999.00;
    int cent;
    /* Iterate over centroids*/
    for (k = 0; k < K; k++)
    {
      if (i != k)
      {
        double inter_sum;
        double inter_diff;
        for (j = 0; j < c; j++)
        {
          inter_sum += pow((arr[i][j] - arr_c[k][j]), 2.0);
        }
        inter_diff = sqrt(inter_sum);
        /* Save difference and nearest centroid*/
        if (inter_diff < diff)
        {
          diff = inter_diff;
          cent = k;
        }
      }
    }
    arr_rel[i][0] = arr[i];
    arr_rel[i][1] = arr_c[cent];
  }

  /* LOOP THE NEXT TWO STEPS*/

  /* Assign data points to nearest cluster */

  /* Re-initialize centroids */

  /* Print out Result */

  free(p);
  free(arr);
  free(p_c);
  free(arr_c);
  free(rel);
  free(arr_rel);

  return 0;
}