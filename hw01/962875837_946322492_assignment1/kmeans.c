#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int isNatNumber(char number[])
{
  int i;
  for (i = 0; number[i] != 0; ++i)
  {
    char c = number[i];
    if (c < 48 || c > 57)
      return 0;
  }
  return 1;
}

int main(int argc, char **argv)
{
  long K;
  long iter;
  FILE *textfile;
  char *filename;

  /* For numbers import*/
  int r = 1, c = 1;
  double *p, **arr;
  int count = 0;
  int i = 0, j = 0, k;
  char line[256];

  /* For centroids */
  double *p_c, **arr_c;
  double **rel, ***arr_rel;

  /* For loop */
  int iteration_number = 0;
  double epsilon = 0.001;
  int treshold_reached = 0;
  double *prev_centroid;
  double *new_centroid;
  double *centroid_sum;

  /* Validate number of args */
  if (argc < 3 || argc > 4)
  {
    printf("Wrong number of arguments!");
    return 1;
  }

  /* Get filename */
  if (argc == 3)
  {
    filename = argv[2];
  }
  else
  {
    filename = argv[3];
  }

  textfile = fopen(filename, "r");
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

  p = calloc(r * c, sizeof(double));
  arr = calloc(r, sizeof(double *));
  for (k = 0; k < r; k++)
  {
    arr[k] = p + k * c;
  }

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
  if (isNatNumber(argv[1]) == 0)
  {
    printf("Invalid number of clusters!\n");
    return 1;
  }
  K = (int)strtol(argv[1], NULL, 10);
  if (K < 2 || K > (count - 1))
  {
    printf("Invalid number of clusters!\n");
    return 1;
  }

  /* Set number of iterations */
  if (argc == 4)
  {
    if (isNatNumber(argv[2]) == 0)
    {
      printf("Invalid maximum iteration!\n");
      return 1;
    }
    iter = strtol(argv[2], NULL, 10);
    if (iter < 2 || iter > 999)
    {
      printf("Invalid maximum iteration!\n");
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
      arr_rel[i][0] = arr[i];
      arr_rel[i][1] = arr_c[cent];
    }

    /* Update centroids */
    for (k = 0; k < K; k++)
    {
      double centroid_distance = 0.0;
      int k_count = 0;

      for (j = 0; j < c; j++)
      {
        prev_centroid[j] = arr_c[k][j];
      }

      for (i = 0; i < r; i++)
      {
        if (arr_rel[i][1] == arr_c[k])
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
        arr_c[k][j] = centroid_sum[j];
      }

      for (j = 0; j < c; j++)
      {
        new_centroid[j] = arr_c[k][j];
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

  /* Print out Result */
  for (k = 0; k < K; k++)
  {
    for (j = 0; j < c; j++)
    {
      if (j == (c - 1))
      {
        printf("%.4f", arr_c[k][j]);
      }
      else
      {
        printf("%.4f,", arr_c[k][j]);
      }
    }
    printf("\n");
  }

  free(p);
  free(arr);
  free(p_c);
  free(arr_c);
  free(rel);
  free(arr_rel);
  free(centroid_sum);
  free(prev_centroid);
  free(new_centroid);

  return 0;
}