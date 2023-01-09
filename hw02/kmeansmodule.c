#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h> // NOT IDEAL, need to find solution
// https://stackoverflow.com/questions/21530577/fatal-error-python-h-no-such-file-or-directory
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static PyObject *
py_fitter(PyObject *self, PyObject *args)
{
  double **data, **centroids = NULL;
  int K, iter = NULL;
  double epsilon = 0.0;

  if (!PyArg_ParseTuple(args, "00iid", &data, &centroids, &K, &iter, &epsilon))
    return NULL;

  double **final_centroids = fit(data, centroids, K, iter, epsilon);

  return PyLong_FromLong(final_centroids);
}

// https://towardsdatascience.com/write-your-own-c-extension-to-speed-up-python-x100-626bb9d166e7
static PyMethodDef KMeansMethods[] = {
    {"fit", py_fitter, METH_VARARGS, "Function to fit"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    "C library for k means algorithm",
    -1,
    KMeansMethods};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
  import_array();
  return PyModule_Create(&kmeansmodule);
};

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

int fit(double **data, double **centroids, int K, int iter, double epsilon)
{
  /* Dimensions */
  int r, c, count;

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
  c = sizeof(data[0]) / sizeof(data[0][0]);
  r = sizeof(data) / sizeof(data[0]);
  count = c * r;

  /* Set number of clusters */
  if (K < 2 || K > (count - 1))
  {
    printf("Invalid number of clusters!\n");
    return 1;
  }

  /* Set number of iterations */
  if (iter < 2 || iter > 999)
  {
    printf("Invalid maximum iteration!\n");
    return 1;
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

  // free(arr);
  // free(centroids);
  free(rel);
  free(arr_rel);
  free(centroid_sum);
  free(prev_centroid);
  free(new_centroid);

  return centroids;
}