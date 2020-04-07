#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "prints.h"
#define ARR_SZ 11
#define SZ 225
#define QUANTUM 3
#define QUANTUM_FCFS 950
#define FNAME "input1.txt"

void green(char input[]) 
{
  printf("\033[0;32m");
  printf("%s", input);
  printf("\033[0m");
}

void yellow(char input[]) 
{
  printf("\033[0;33m");
  printf("%s", input);
  printf("\033[0m");
}

void red(char input[]) 
{
  printf("\033[0;31m");
  printf("%s", input);
  printf("\033[0m");
}

void blue(char input[]) 
{
  printf("\033[0;34m");
  printf("%s", input);
  printf("\033[0m");
}