#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef char DATA;

struct node {
  DATA  d;
  struct node *left;
  struct node *right;
};

typedef struct node NODE;
typedef NODE *BTREE;
