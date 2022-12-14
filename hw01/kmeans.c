#include <stdio.h>
#include <math.h>
#include <stdlib.h>



int main(int argc, char **argv)
{
  long K;
  long iter;
  int N;
  FILE *textfile;
  double d1, d2, d3;
 /* Import and check number of data points*/

  /* Validate Input */
  if (argc < 2 || argc > 3)
  {
    printf("Wrong number of arguments!");
    return 1;
  }

  textfile = fopen("./input_1.txt", "r");
  if (textfile == NULL) {
    printf("An error has occured.\n");
    return 1;
  }

  fscanf(textfile, "%lf,%lf,%lf\n", &d1, &d2, &d3);
  while(!feof (textfile)) {
    printf("%f,%f,%f\n", d1, d2, d3);
    fscanf(textfile, "%lf,%lf,%lf\n", &d1, &d2, &d3);
    N = N + 3;
  }
  fclose(textfile);

  K = strtol(argv[1], NULL, 10);
  if (K < 2 || K > (N - 1)) {
    printf("Invalid number of clusters!");
    return 1;
  }
  if (argc == 3) {
    iter = strtol(argv[2], NULL, 10);
    if (iter < 2 || iter > 999) {
      printf("Invalid maximum iteration!");
      return 1;
    }
  } else {
    iter = 200;
  }

  /* Set number of clusters */

  /* Initialize centroids */

  /* LOOP THE NEXT TWO STEPS*/

  /* Assign data points to nearest cluster */

  /* Re-initialize centroids */

  /* Print out Result */

  return 0;
}