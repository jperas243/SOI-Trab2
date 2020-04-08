#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#define ARR_SZ 11
#define SZ 225

struct process
{
    int PID, inicial_time, *run, *blocked;
};

process_t *create_process(int sz) //Bob
{
    process_t *process_v = malloc(sizeof(process_t));
    process_v->inicial_time = 0;
    process_v->run = malloc(sz * sizeof(int));
    process_v->blocked = malloc(sz * sizeof(int));

    return process_v;
}

process_t *insert_process(int beg, int end, int queues, int arr[])
{
    process_t *process_v = create_process(SZ);

    int p_run = 0, p_blocked = 0, count = 0;

    for (int i = beg; i <= end; ++i)
    {
        if (arr[i] > 99)
            process_v->PID = arr[beg];

        else if (count == 1)
            process_v->inicial_time = arr[beg + 1];

        else if (count%2 == 0)
        {
            process_v->run[p_run] = arr[i];
            p_run++;
        }

        else if(count%2 != 0)
        {
            process_v->blocked[p_blocked] = arr[i];
            p_blocked++;
        }

        else if (arr[i] > 99)
        {
            process_v->PID = arr[i];
        }
        count++;
    }
    return process_v;
}

int find_PID(int PID, process_t *process_arr[], int n_process)
{
    for(int i = 0; i < n_process; i++)
    {
        if(process_arr[i]->PID == PID)
        {
            return i;
        }
    }
    return -1;
    red("PID not found!");
}

void update_run(int n_process, process_t *process_arr[])
{
    process_arr[n_process]->run[0]--;
}

void update_blocked(int n_process, process_t *process_arr[])
{
    process_arr[n_process]->blocked[0]--;
}

void update_index_run(int n_process, process_t *process_arr[], int size)
{
    for (int i = 0; i <= size; ++i)
    {
        if(process_arr[n_process] -> run[i] > 1000)
        {
            process_arr[n_process] -> run[i] = 0;
        }
        process_arr[n_process] -> run[i] = process_arr[n_process] -> run[i+1];
    }
}

void update_index_blocked(int n_process, process_t *process_arr[], int size)
{
    for (int i = 0; i <= size; i++)
    {
        if(process_arr[n_process] -> blocked[i] > 1000)
        {
            process_arr[n_process] -> blocked[i] = 0;
        }
        process_arr[n_process] -> blocked[i] = process_arr[n_process] -> blocked[i+1];
    }
}