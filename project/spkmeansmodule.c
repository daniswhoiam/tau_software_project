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
  PyObject *data, *output_data, *py_double_arr, *py_double;
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

  output_data = PyList_New(N);
  for (i = 0; i < N; i++)
  {
    py_double_arr = PyList_New(N);
    for (j = 0; j < N; j++)
    {
      py_double = PyFloat_FromDouble(arr_output_data[i][j]);
      PyList_SetItem(py_double_arr, j, py_double);
    }
    PyList_SetItem(output_data, i, py_double_arr);
  }

  free(input_data);
  free(arr_output_data);

  if (!output_data)
    return NULL;
  return Py_BuildValue("O", output_data);
}

/*
static PyObject *py_ddg(PyObject *self, PyObject *args)
{
}

static PyObject *py_gl(PyObject *self, PyObject *args)
{
}

static PyObject *py_jacobi(PyObject *self, PyObject *args)
{
}
*/

// Define KMeansMethods[] PyMethodDef
static PyMethodDef SPKMeansMethods[] = {
    //{"spk", (PyCFunction)py_spk, METH_VARARGS, PyDoc_STR("spk function")},
    {"wam", (PyCFunction)py_wam, METH_VARARGS, PyDoc_STR("wam function")},
    //{"ddg", (PyCFunction)py_ddg, METH_VARARGS, PyDoc_STR("ddg function")},
    //{"gl", (PyCFunction)py_gl, METH_VARARGS, PyDoc_STR("gl function")},
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