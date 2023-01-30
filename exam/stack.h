/* Linked list implementation of a stack */
#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0
#define FULL 10000

typedef char data;
typedef enum
{
  TRUE,
  FALSE
} boolean;

struct elem
{
  data d;
  struct elem *next;
};
typedef struct elem elem;

struct stack
{
  int cnt;
  elem *top;
};
typedef struct stack stack;

void initialize(stack *stk);
void push(data d, stack *stk);
data pop(stack *stk);
data top(stack *stk);
boolean empty(const stack *stk);
boolean full(const stack *stk);
