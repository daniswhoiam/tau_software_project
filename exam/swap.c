#include <stdlib.h>
#include <stdio.h>
#define sw(x,y) {x=x^y;y=x^y;x=x^y;}

int main(void) {
  int x = 123;
  int y = 456;

  sw(x,y);

  printf("x: %d, y: %d", x, y);

}