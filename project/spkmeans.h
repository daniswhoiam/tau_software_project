#ifndef SPKMEANS_H
#define SPKMEANS_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void print_matrix(double **matrix, int N);

void rotate(double **A, double **P, int k, int l, int N);

double *diagonalize_matrix(double **matrix, int N);

double *jacobi(double **A, double ***eigenvectors, int N, int maxRot, double tol);

double **make_wadjm(double **data, int N, int dim);

double **make_diagdem(double **wadjm, int N);

double **subtract_matrices(double **mat1, double **mat2, int N);

double **identity_matrix(int N);

double **fit(double **data, double **centroids, int r, int c, int K, int iter, double epsilon);

// https://stackoverflow.com/questions/36940643/sorting-an-array-of-double-in-c
static int compare (const void * a, const void * b);

int max_eigengap(double *eigenvalues, int N);

int *maxElem(double **A, int N);

#endif