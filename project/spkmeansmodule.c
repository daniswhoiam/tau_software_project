#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "spkmeans.c"

// Define py_x PyObjects
/*
static PyObject *py_spk(PyObject *self, PyObject *args)
{
} */

static PyObject *py_wam(PyObject *self, PyObject *args)
{
  PyObject *data, *py_output_data, *py_double_arr, *py_double;
  int N, dim, i, j;
  double *input_data;
  double **arr_input_data, **arr_output_data;

  if (!PyArg_ParseTuple(args, "Oii", &data, &N, &dim))
    return NULL;

  input_data = calloc(N * dim, sizeof(double));
  arr_input_data = calloc(N, sizeof(double *));

  for (i = 0; i < N; i++)
  {
    arr_input_data[i] = input_data + i * dim;
  }
  for (i = 0; i < N; i++)
  {
    PyObject *array = PyList_GetItem(data, i);
    for (j = 0; j < dim; j++)
    {
      PyObject *item = PyList_GetItem(array, j);
      double num = PyFloat_AsDouble(item);
      arr_input_data[i][j] = num;
    }
  }

  arr_output_data = calloc(N * N, sizeof(double));
  arr_output_data = make_wadjm(arr_input_data, N, dim);

  py_output_data = PyList_New(N);
  for (i = 0; i < N; i++)
  {
    py_double_arr = PyList_New(N);
    for (j = 0; j < N; j++)
    {
      py_double = PyFloat_FromDouble(arr_output_data[i][j]);
      PyList_SetItem(py_double_arr, j, py_double);
    }
    PyList_SetItem(py_output_data, i, py_double_arr);
  }

  free(input_data);
  free(arr_output_data);

  if (!py_output_data)
    return NULL;
  return Py_BuildValue("O", py_output_data);
}


static PyObject *py_ddg(PyObject *self, PyObject *args)
{
  PyObject *data, *py_output_data, *py_double_arr, *py_double;
  int N, i, j;
  double *input_data;
  double **arr_input_data, **arr_output_data;

  if (!PyArg_ParseTuple(args, "Oi", &data, &N))
    return NULL;

  input_data = calloc(N * N, sizeof(double));
  arr_input_data = calloc(N, sizeof(double *));

  for (i = 0; i < N; i++)
  {
    arr_input_data[i] = input_data + i * N;
  }
  for (i = 0; i < N; i++)
  {
    PyObject *array = PyList_GetItem(data, i);
    for (j = 0; j < N; j++)
    {
      PyObject *item = PyList_GetItem(array, j);
      double num = PyFloat_AsDouble(item);
      arr_input_data[i][j] = num;
    }
  }

  arr_output_data = calloc(N * N, sizeof(double));
  arr_output_data = make_diagdem(arr_input_data, N);

  py_output_data = PyList_New(N);
  for (i = 0; i < N; i++)
  {
    py_double_arr = PyList_New(N);
    for (j = 0; j < N; j++)
    {
      py_double = PyFloat_FromDouble(arr_output_data[i][j]);
      PyList_SetItem(py_double_arr, j, py_double);
    }
    PyList_SetItem(py_output_data, i, py_double_arr);
  }

  free(input_data);
  free(arr_output_data);

  if (!py_output_data)
    return NULL;
  return Py_BuildValue("O", py_output_data);
}

static PyObject *py_gl(PyObject *self, PyObject *args)
{
  PyObject *matrix_one, *matrix_two, *py_output_matrix, *py_double_arr, *py_double;
  int N, i, j;
  double *input_arr_one, *input_arr_two;
  double **input_matrix_one, **input_matrix_two, **output_matrix;

  if (!PyArg_ParseTuple(args, "OOi", &matrix_one, &matrix_two, &N)) 
    return NULL;

  input_arr_one = calloc(N * N, sizeof(double));
  input_matrix_one = calloc(N, sizeof(double *));
  
  for (i = 0; i < N; i++)
  {
    input_matrix_one[i] = input_arr_one + i * N;
  }
  for (i = 0; i < N; i++)
  {
    PyObject *array = PyList_GetItem(matrix_one, i);
    for (j = 0; j < N; j++)
    {
      PyObject *item = PyList_GetItem(array, j);
      double num = PyFloat_AsDouble(item);
      input_matrix_one[i][j] = num;
    }
  }

  input_arr_two = calloc(N * N, sizeof(double));
  input_matrix_two = calloc(N, sizeof(double *));

  for (i = 0; i < N; i++)
  {
    input_matrix_two[i] = input_arr_two + i * N;
  }
  for (i = 0; i < N; i++)
  {
    PyObject *array = PyList_GetItem(matrix_two, i);
    for (j = 0; j < N; j++)
    {
      PyObject *item = PyList_GetItem(array, j);
      double num = PyFloat_AsDouble(item);
      input_matrix_two[i][j] = num;
    }
  }

  output_matrix = calloc(N * N, sizeof(double));
  output_matrix = subtract_matrices(input_matrix_one, input_matrix_two, N);

  py_output_matrix = PyList_New(N);
  for (i = 0; i < N; i++)
  {
    py_double_arr = PyList_New(N);
    for (j = 0; j < N; j++)
    {
      py_double = PyFloat_FromDouble(output_matrix[i][j]);
      PyList_SetItem(py_double_arr, j, py_double);
    }
    PyList_SetItem(py_output_matrix, i, py_double_arr);
  }

  free(input_matrix_one);
  free(input_matrix_two);
  free(output_matrix);

  if (!py_output_matrix)
    return NULL;
  return Py_BuildValue("O", py_output_matrix);
}

/*
static PyObject *py_jacobi(PyObject *self, PyObject *args)
{
}
*/

// Define KMeansMethods[] PyMethodDef
static PyMethodDef SPKMeansMethods[] = {
    //{"spk", (PyCFunction)py_spk, METH_VARARGS, PyDoc_STR("spk function")},
    {"wam", (PyCFunction)py_wam, METH_VARARGS, PyDoc_STR("wam function")},
    {"ddg", (PyCFunction)py_ddg, METH_VARARGS, PyDoc_STR("ddg function")},
    {"gl", (PyCFunction)py_gl, METH_VARARGS, PyDoc_STR("gl function")},
    //{"jacobi", (PyCFunction)py_jacobi, METH_VARARGS, PyDoc_STR("jacobi function")},
    {NULL, NULL, 0, NULL}};

// Define spkmeansmodule PyModuleDef
static struct PyModuleDef spkmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "myspkmeanssp",
    "C library for unnormalized spectral clustering",
    -1,
    SPKMeansMethods};

// Define PyInit_myspkmeanssp PyMODINIT_FUNC
PyMODINIT_FUNC PyInit_myspkmeanssp(void)
{
  PyObject *m;
  m = PyModule_Create(&spkmeansmodule);
  if (!m)
    return NULL;
  return m;
}