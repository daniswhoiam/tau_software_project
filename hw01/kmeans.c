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

  /* For loop */
  int iteration_number = 0;
  double epsilon = 0.001;
  int treshold_reached = 0;
  double *prev_centroid;
  double *new_centroid;
  double *centroid_sum;

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
  K = (int)strtol(argv[1], NULL, 10);
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
      printf("Pre-k: i = %d, diff = %f\n", i, diff);
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
        printf("Step i = %d k = %d, diff = %f, inter_diff = %f, cent = %d\n", i, k, diff, inter_diff, cent);
      }
      arr_rel[i][0] = arr[i];
      arr_rel[i][1] = arr_c[cent];
      printf("Arr_rel pointer zu c: %p\n", (void *)arr_rel[i][1]);
      printf("Arr_c address: %p, Cent: %d\n", (void *)arr_c[cent], cent);
    }

    /* Update centroids */
    for (k = 0; k < K; k++)
    {
      double centroid_distance = 0.0;
      int k_count = 0;

      for (j = 0; j < c; j++)
      {
        prev_centroid[j] = arr_c[k][j];
        printf("Prev %d : %f\n", j, prev_centroid[j]);
      }

      for (i = 0; i < r; i++)
      {
        printf("Same pointer: %d\n", arr_rel[i][1] == arr_c[k]);
        printf("Arr_rel: %p\n", (void *)arr_rel[i][1]);
        printf("Arr_c: %p\n", (void *)arr_c[k]);
        if (arr_rel[i][1] == arr_c[k])
        {
          for (j = 0; j < c; j++)
          {
            /* printf("C_sum[%d] + = %f\n", j, arr_rel[i][0][j]); */
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
        printf("New %d : %f\n", j, new_centroid[j]);
      }

      for (j = 0; j < c; j++)
      {
        centroid_distance += pow((prev_centroid[j] - new_centroid[j]), 2);
      }
      centroid_distance = sqrt(centroid_distance);
      printf("Centroid Distance: %f\n", centroid_distance);
      if (centroid_distance > epsilon)
      {
        treshold_reached = 0;
      }
      printf("%f\n", centroid_distance);
      for (j = 0; j < c; j++)
      {
        centroid_sum[j] = 0.0;
      }
    }
    iteration_number++;
    printf("Iter: %d\n", iteration_number);
  }

  /* Print out Result */
  printf("Final Centroids:\n");
  for (k = 0; k < K; k++)
  {
    for (j = 0; j < c; j++)
    {
      printf("%f, ", arr_c[k][j]);
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