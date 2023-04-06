#include <math.h>

int *maxElem(double **A, int N)
{
  int i, j, k, l;
  double aMax = 0.0;
  static int maxIndices[2];

  for (i = 0; i < N - 1; i++)
  {
    for (j = i + 1; j < N; j++)
    {
      if (fabs(A[i][j]) >= aMax)
      {
        aMax = fabs(A[i][j]);
        k = i;
        l = j;
      }
    }
  }

  maxIndices[0] = k;
  maxIndices[1] = l;

  return maxIndices;
}