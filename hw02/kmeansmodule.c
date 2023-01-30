#define PY_SSIZE_T_CLEAN
#include <Python.h> // NOT IDEAL, need to find solution
// https://stackoverflow.com/questions/21530577/fatal-error-python-h-no-such-file-or-directory
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

  free(prev_centroid);
  free(new_centroid);
  free(centroid_sum);
  free(rel);
  free(arr_rel);

  return centroids;
}

static PyObject *
py_fitter(PyObject *self, PyObject *args)
{
  PyObject *data, *centroids;
  int K, iter, r, c;
  double epsilon;
  int i, j, k;
  double *input_data;
  double **arr_input_data;
  double *input_centroids;
  double **arr_input_centroids;
  double **final_centroids;
  PyObject *py_final_centroids;
  PyObject *py_double_arr;
  PyObject *py_double;

  if (!PyArg_ParseTuple(args, "OOiiiid", &data, &centroids, &r, &c, &K, &iter, &epsilon))
    return NULL;

  input_data = calloc(r * c, sizeof(double));
  arr_input_data = calloc(r, sizeof(double *));

  for (k = 0; k < r; k++)
  {
    arr_input_data[k] = input_data + k * c;
  }
  for (i = 0; i < r; i++)
  {
    PyObject *array = PyList_GetItem(data, i);
    for (j = 0; j < c; j++)
    {
      PyObject *item = PyList_GetItem(array, j);
      double num = PyFloat_AsDouble(item);
      arr_input_data[i][j] = num;
    }
  }

  input_centroids = calloc(K * c, sizeof(double));
  arr_input_centroids = calloc(K, sizeof(double *));
  for (k = 0; k < K; k++)
  {
    arr_input_centroids[k] = input_centroids + k * c;
  }
  for (i = 0; i < K; i++)
  {
    PyObject *array = PyList_GetItem(centroids, i);
    for (j = 0; j < c; j++)
    {
      PyObject *item = PyList_GetItem(array, j);
      double num = PyFloat_AsDouble(item);
      arr_input_centroids[i][j] = num;
    }
  }

  final_centroids = calloc(K, sizeof(double *));
  final_centroids = fit(arr_input_data, arr_input_centroids, r, c, K, iter, epsilon);

  py_final_centroids = PyList_New(K);
  for (i = 0; i < K; i++)
  {
    py_double_arr = PyList_New(c);
    for (j = 0; j < c; j++)
    {
      py_double = PyFloat_FromDouble(final_centroids[i][j]);
      PyList_SetItem(py_double_arr, j, py_double);
    }
    PyList_SetItem(py_final_centroids, i, py_double_arr);
  }

  free(final_centroids);
  free(input_centroids);
  free(input_data);

  if (!py_final_centroids)
    return NULL;
  return Py_BuildValue("O", py_final_centroids);
}

// https://towardsdatascience.com/write-your-own-c-extension-to-speed-up-python-x100-626bb9d166e7
static PyMethodDef KMeansMethods[] = {
    {"fit", (PyCFunction)py_fitter, METH_VARARGS, PyDoc_STR("Function to fit")},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef kmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp",
    "C library for k means algorithm",
    -1,
    KMeansMethods};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
  PyObject *m;
  m = PyModule_Create(&kmeansmodule);
  if (!m)
    return NULL;
  return m;
};