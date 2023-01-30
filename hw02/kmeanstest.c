#include <stdio.h>
#include <math.h>
#include <stdlib.h>

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

  /* Set number of clusters */
  if (K < 2 || K > (count - 1))
  {
    printf("Invalid number of clusters!\n");
    return NULL;
  }

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

  free(rel);
  free(arr_rel);
  free(centroid_sum);
  free(prev_centroid);
  free(new_centroid);

  return centroids;
}

int main(void)
{
  int k, j;
  double **as_x, **ac_x, *s_x, *c_x;
  int r = 4;
  int c = 3;
  int K = 2;
  int iter = 100;
  double epsilon = 0.001;
  double **result;

  double sampledata[4][3] = {
      {2.01, 3.01, 4.01},
      {1.99, 2.99, 3.99},
      {10.01, 11.01, 12.01},
      {11.01, 12.01, 13.01}};
  double centroids[2][3] = {
      {1.01, -1.01, 0.00},
      {9.01, 8.00, 15.01}};

  s_x = calloc(r * c, sizeof(double));
  as_x = calloc(r, sizeof(double *));
  for (k = 0; k < r; k++)
  {
    as_x[k] = s_x + k * c;
  }
  for (k = 0; k < r; k++)
  {
    for (j = 0; j < c; j++)
    {
      as_x[k][j] = sampledata[k][j];
    }
  }

  c_x = calloc(K * c, sizeof(double));
  ac_x = calloc(K, sizeof(double *));
  for (k = 0; k < K; k++)
  {
    ac_x[k] = c_x + k * c;
  }
  for (k = 0; k < K; k++)
  {
    for (j = 0; j < c; j++)
    {
      ac_x[k][j] = centroids[k][j];
    }
  }

  result = calloc(c * K, sizeof(double));
  result = fit(as_x, ac_x, r, c, K, iter, epsilon);

  /* Print out Result */
  for (k = 0; k < K; k++)
  {
    for (j = 0; j < c; j++)
    {
      if (j == (c - 1))
      {
        printf("%.4f", result[k][j]);
      }
      else
      {
        printf("%.4f,", result[k][j]);
      }
    }
    printf("\n");
  }

  free(result);

  return 0;
}