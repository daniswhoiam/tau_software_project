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

  /* Import and check number of data points*/

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

  /*Read first row, determine column number*/
  while (fgets(line, sizeof(line), textfile))
  {
    char *l = line;
    double temp;
    int n;
    while (sscanf(l, "%lf,%n", &temp, &n) == 1)
    {
      /*
      printf("%f\n", temp);
      p[count] = temp;
      p = realloc(p, (count + 2) * sizeof(double));
      */
      if (r == 1)
        c++;
      l += n;
      count++;
    }
    /*
    arr[r] = arr[r - 1] + c * sizeof(double);
    */
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
  free(p);
  free(arr);

  /* Set number of clusters */
  K = strtol(argv[1], NULL, 10);
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

  /* LOOP THE NEXT TWO STEPS*/

  /* Assign data points to nearest cluster */

  /* Re-initialize centroids */

  /* Print out Result */

  return 0;
}