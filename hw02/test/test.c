#define PY_SSIZE_T_CLEAN
#include <python3.10/Python.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int calc_sum(int first, int second)
{
  int result = first + second;
  return result;
}

static PyObject *py_sum(PyObject *self, PyObject *args)
{
  int first, second, sum;

  if (!PyArg_ParseTuple(args, "ii", &first, &second))
    return NULL;

  sum = calc_sum(first, second);

  return Py_BuildValue("i", sum);
}

static PyMethodDef SumMethods[] = {
    {"calc_sum", (PyCFunction)py_sum, METH_VARARGS, PyDoc_STR("Function to sum")},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef summodule = {
    PyModuleDef_HEAD_INIT,
    "mysummodule",
    "C library bla",
    -1,
    SumMethods};

PyMODINIT_FUNC PyInit_mysummodule(void)
{
  PyObject *m;
  m = PyModule_Create(&summodule);
  if (!m)
    return NULL;
  return m;
};