#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "prints.h"
#include "queues.c"
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

void print_scheduler(int inst, queue_t *ready, queue_t *run, queue_t *blocked)
{
  yellow("Instant");
  if(inst <= 9)
  {
    printf(" ");
  }
  printf(" %d - ", inst); //Print the Instant
  yellow("Ready: ");printQueue(ready);
  if(ready->final_pos == 0)
  {
    printf("       ");
  }
  else if(ready->final_pos == 1)
  {
    printf("         ");
  }
  else if (ready->final_pos == 2)
  {
    printf("     ");
  }
  else if (ready->final_pos == 3)
  {
    printf(" ");
  }
  ;printf(" |");
  yellow (" Run: "); printQueue(run);
  if(run->final_pos == 1)
  {
    printf("  ");
  }
  printf("|");
  yellow(" Blocked: "); printQueue(blocked);
  printf("\n");
}