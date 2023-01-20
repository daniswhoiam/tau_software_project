#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* Recursive */
LINK string_to_list(char s[])
{
  LINK head;

  if (s[0] == '\0')
    return NULL;
  else
  {
    head = malloc(sizeof(ELEMENT));
    head->d = s[0];
    head->next = string_to_list(s + 1);
    return head;
  }
}

/* Count a list recursively */
int count(LINK head)
{
  if (head == NULL)
    return 0;
  else
    return (1 + count(head->next));
}

/* Print a list recursively */
void print(LINK head)
{
  if (head == NULL)
    printf("NULL");
  else
  {
    printf("%c --> ", head->d);
    print(head->next);
  }
}

/* Concatenate list a and b */
void concatenate(LINK a, LINK b)
{
  assert(a != NULL);
  if (a->next == NULL)
    a->next = b;
  else
    concatenate(a->next, b);
}

/* Insert element in a linked list */
void insert(LINK p1, LINK p2, LINK q)
{
  assert(p1->next == p2);
  p1->next = q;
  q->next = p2;
}

/* Delete list */
void delete_list(LINK head)
{
  if (head != NULL)
  {
    delete_list(head->next);
    free(head);
  }
}

LINK find_previous(LINK start, LINK target)
{
  if (start->next == target)
    return start;
  else
    return find_previous(start->next, target);
}

int main(void)
{
  LINK h;

  h = string_to_list("erste hälfte ");
  printf("The resulting list is\n");
  print(h);
  printf("\nThis list has %d elements.\n", count(h));

  LINK e;

  e = string_to_list("zweite hälfte");
  concatenate(h, e);
  printf("The resulting list is\n");
  print(h);

  LINK p = find_previous(h, e);
  insert(p, e, string_to_list(","));
  printf("The resulting list is\n");
  print(h);

  delete_list(h);
  printf("The resulting list is gone:\n");
  printf("%i", h == NULL);
}