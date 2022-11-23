#include <stdio.h>
#include <math.h>

int i;

int source_base = 0;
int target_base;

int in_decimal = 0;
int in_target = 0;

/* SETTING THE SOURCE BASE */
void set_base(int *base)
{
  char c;
  if ((c = getchar()) != '\n')
  {
    set_base(base);
    *base += (int)((c - '0') * pow(10, i));
    i++;
  }
  return;
}

/* TRANSLATING INPUT FROM SOURCE BASE TO BASE 10 */
int to_decimal(void)
{
  char c;
  int translated;
  int input_legit = 1;
  if ((c = getchar()) != '\n')
  {
    input_legit = to_decimal();
    if (c < 58)
    {
      translated = c - '0';
      in_decimal += translated * pow(source_base, i);
    }
    else if (c > 96)
    {
      translated = c - 'W';
      in_decimal += translated * pow(source_base, i);
    }

    /* INPUT VALIDATION */
    if (translated > source_base - 1 || input_legit == 0)
    {
      input_legit = 0;
      in_decimal = 0;
      return 0;
    }

    i++;
  }
  return input_legit;
}

/* TRANSLATING BASE 10 TO TARGET BASE*/
void to_target(void)
{
  int inter;
  int mod;
  if (in_decimal != 0)
  {
    /* How many times does target_base fully fit into in_decimal */
    inter = in_decimal / pow(target_base, i);
    /* What remains */
    mod = in_decimal % ((int)pow(target_base, i + 1));
  }
  else
  {
    /* BASE CASE in_decimal == 0 */
    return;
  }

  if (inter >= target_base)
  {
    /* 
    * If target_base fits into in_decimal more than it's own value
    * We need to go to the next power (target_base^(i))
    */
    in_decimal = in_decimal - mod;
    i++;
    to_target();
    /*
    * After some recursive operations we need to calculate mod again
    * Because then we have already taken away the part of "inter"
    * Thas was too much to represent at this stage of i
    */
    mod = inter % ((int)pow(target_base, i));
  }
  else if (inter < target_base)
  {
    /*
    * When we don't need to go to a higher power dimension
    */
    if (inter < 9 && inter >= 0)
    {
      printf("%d", inter);
    }
    else if (inter > 9 && inter <= 16)
    {
      printf("%c", inter + 87);
    }
    /**
     * Necessary to reach base case confidently
    */
    if (in_decimal != mod)
    {
      in_decimal = mod;
    }
    else
    {
      in_decimal = 0;
    }
    /*
    * We go down a dimension and start from the beginning
    */
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

int main(void)
{
  do
  {
    i = 0;
    source_base = 0;
    printf("Enter the source base:\n");
    set_base(&source_base);
  } while ((source_base < 2 || source_base > 16) && printf("Invalid source base!\n"));
  printf("The source base is: %d\n", source_base);

  do
  {
    i = 0;
    target_base = 0;
    printf("Enter the target base:\n");
    set_base(&target_base);
  } while ((target_base < 2 || target_base > 16) && printf("Invalid target base!\n"));
  printf("The target base is: %d\n", target_base);

  do
  {
    i = 0;
    printf("Enter a number in base %d\n", source_base);
  } while (!to_decimal() && printf("Invalid input number!\n"));

  i = 0;
  printf("The entered number in base %d is: ", target_base);
  to_target();
  printf("\n");
  return 0;
}