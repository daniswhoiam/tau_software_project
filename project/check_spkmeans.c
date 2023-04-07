#include <check.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "spkmeans.h"

START_TEST(test_max_elem)
{
  int *maxIndices;
  double maxValue;
  double **matrix;
  int i, j;
  double list[16] = {1.0, 1.0, 0.0, 54.0,
                     1.0, -123.0, 0.0, 0.0,
                     0.0, -0.7, 1.0, 1.0,
                     0.0, 0.8, 1.0, 1.0};

  matrix = malloc(4 * sizeof(double *));
  for (i = 0; i < 4; i++)
  {
    matrix[i] = malloc(4 * sizeof(double));
  }

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      matrix[i][j] = list[i + j];
    }
  }

  maxIndices = malloc(2 * sizeof(int));
  maxIndices = maxElem(matrix, 4);
  maxValue = matrix[maxIndices[0]][maxIndices[1]];

  ck_assert_double_eq(-123.0, maxValue);
}
END_TEST

START_TEST(test_rotate)
{
  double **matrix_a, **matrix_p;
  int k, l, n, i, j;

  double a_list[16] = {1.0, 1.0, 0.0, 54.0,
                       1.0, -123.0, 0.0, 0.0,
                       0.0, -0.7, 1.0, 1.0,
                       0.0, 0.8, 1.0, 1.0};
  double p_list[16] = {1.0, 0.0, 0.0, 0.0,
                       0.0, 1.0, 0.0, 0.0,
                       0.0, 0.0, 1.0, 0.0,
                       0.0, 0.0, 0.0, 1.0};

  matrix_a = malloc(4 * sizeof(double *));
  for (i = 0; i < 4; i++)
  {
    matrix_a[i] = malloc(4 * sizeof(double));
  }

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      matrix_a[i][j] = a_list[i * 4 + j];
    }
  }
  matrix_p = malloc(4 * sizeof(double *));
  for (i = 0; i < 4; i++)
  {
    matrix_p[i] = malloc(4 * sizeof(double));
  }

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      matrix_p[i][j] = p_list[i * 4 + j];
    }
  }

  k = 1;
  l = 1;

  rotate(matrix_a, matrix_p, k, l, 4);

  ck_assert_double_eq(0.0, matrix_a[k][l]);
}
END_TEST

START_TEST(test_jacobi)
{
  double **matrix, **eigenvectors;
  double *eigenvalues;
  int i, j;

  double matrix_list[16] = {1.0, 1.0, 0.0, 0.0,
                            1.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 1.0,
                            0.0, 0.0, 1.0, 1.0};
  int maxRot = 100;
  double tol = 1E-6;

  matrix = malloc(4 * sizeof(double *));
  for (i = 0; i < 4; i++)
  {
    matrix[i] = malloc(4 * sizeof(double));
  }
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      matrix[i][j] = matrix_list[4 * i + j];
    }
  }

  eigenvectors = malloc(4 * sizeof(double *));
  for (i = 0; i < 4; i++)
  {
    eigenvectors[i] = malloc(4 * sizeof(double));
  }
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      eigenvectors[i][j] = (i == j) ? 1.0 : 0.0;
    }
  }

  eigenvalues = malloc(4 * sizeof(double));

  eigenvalues = jacobi(matrix, &eigenvectors, 4, maxRot, tol);

  ck_assert_double_eq(eigenvalues[0], 0.0);
  ck_assert_double_eq(eigenvalues[1], 2.0);
  ck_assert_double_eq(eigenvalues[2], 0.0);
  ck_assert_double_eq(eigenvalues[3], 2.0);
}

Suite *spkmeans(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("spkmeans");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_max_elem);
  tcase_add_test(tc_core, test_rotate);
  tcase_add_test(tc_core, test_jacobi);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = spkmeans();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}