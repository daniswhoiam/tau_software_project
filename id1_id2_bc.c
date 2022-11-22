#include <stdio.h>
#include <math.h>

int i = 0;
int sum = 0;

int source_base = 0;
int target_base = 0;

int in_decimal = 0;
int in_target = 0;

void reverse(void)
{
  char c;
  if ((c = getchar()) != '\n')
  {
    reverse();
    sum += (c - '0') * pow(2, i);
    i++;
  }
  return;
}

void to_decimal(void)
{
  char c;
  if ((c = getchar()) != '\n')
  {
    to_decimal();
    if (c < 58)
    {
      in_decimal += (c - '0') * pow(source_base, i);
    }
    else if (c > 96)
    {
      in_decimal += (c - 'W') * pow(source_base, i);
    }
    i++;
  }
  return;
}

void to_target(void)
{
  int inter;
  int mod;
  if (in_decimal != 0)
  {
    inter = in_decimal / pow(target_base, i);
    mod = in_decimal % ((int)pow(target_base, i + 1));
  }
  else
  {
    /* BASE CASE */
    return;
  }

  if (inter >= target_base)
  {
    in_decimal = in_decimal - mod;
    i++;
    to_target();
    mod = inter % ((int)pow(target_base, i));
  }
  else if (inter < target_base)
  {
    if (inter < 9 && inter >= 0)
    {
      printf("%d", inter);
    }
    else if (inter > 9 && inter <= 16)
    {
      printf("%c", inter + 87);
    }
    if (in_decimal != mod)
    {
      in_decimal = mod;
    }
    else
    {
      in_decimal = 0;
    }
    i--;
    to_target();
    return;
  }

  if (mod < 9 && mod >= 0)
  {
    printf("%d", mod);
  }
  else if (mod > 9 && mod <= 16)
  {
    printf("%c", mod + 87);
  }
  return;
}

void set_source(void)
{
  char c;
  if ((c = getchar()) != '\n')
  {
    set_source();
    source_base += (c - '0') * pow(10, i);
    i++;
  }
  return;
}

void set_target(void)
{
  char c;
  if ((c = getchar()) != '\n')
  {
    set_target();
    target_base += (c - '0') * pow(10, i);
    i++;
  }
  return;
}

int main(void)
{
  printf("Enter the source base:\n");
  set_source();
  while (source_base < 2 || source_base > 16) {
    printf("Invalid source base!\n");
    source_base = 0;
    i = 0;
    printf("Enter the source base:\n");
    set_source();
  }
  printf("The source base is: %d\n", source_base);
  i = 0;
  printf("Enter the target base:\n");
  set_target();
  while (target_base < 2 || target_base > 16) {
    printf("Invalid target base!\n");
    target_base = 0;
    i = 0;
    printf("Enter the target base:\n");
    set_target();
  }
  printf("The target base is: %d\n", target_base);
  i = 0;
  printf("Enter a number in base %d\n", source_base);
  to_decimal();
  /*printf("The entered number in base 10 is: %d\n", in_decimal);*/
  i = 0;
  printf("The entered number in base %d is: ", target_base);
  to_target();
  printf("\n");
  return 0;
}