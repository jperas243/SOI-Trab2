#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queues.h"

struct queue //Makiko
{
    int inicial_pos, final_pos, *array, sz;
};

queue_t *create_queue (int sz) //Bob
{
    queue_t *queue_v = malloc(sizeof(queue_t));
    queue_v -> sz = sz;
    queue_v -> array = malloc(queue_v -> sz* sizeof(int));
    queue_v -> inicial_pos = 0;
    queue_v -> final_pos = 0;

    return queue_v;
}

void insert (queue_t *queue, int element)
{
    if (!full (queue))
    {
        queue -> array [queue -> final_pos] = element;
        queue -> final_pos ++;
    }    
}

bool full (queue_t *queue)
{
    return queue -> sz == queue -> final_pos;
}

bool empty (queue_t *queue) 
{
    return queue -> inicial_pos == queue -> final_pos;
}

int get (queue_t *queue) //Remove first and return it, and move the others
{
    if (empty (queue))
    {
        return -1;
    }

    int temp = queue -> array[queue -> inicial_pos];
    for (int i = 0; i < (queue -> final_pos); i++)
    {
        queue -> array[i] = queue -> array[i+1];
    }
    queue -> final_pos --;   
    return temp;
}

void printQueue(queue_t *queue)
{
    if(empty(queue))
    {
        blue("Empty!");
    }
    else
    {
        for (int i = 0; i < queue->final_pos; i++)
        {
            printf("%d ", queue->array[i]);
        }
        
    }
}

int top(queue_t *queue) //return first
{
    return queue->array[queue->final_pos - 1];
}

int get_pos(queue_t *queue, int pos) //get pos and return it, and move others
{
    int tmp = queue->array[pos];
    for (int i = pos; i < queue->final_pos; i++)
    {
        queue->array[i] = queue->array[i + 1];
    }
    queue->final_pos--;
    return tmp;
}