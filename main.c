#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define SZ 50
#define QUANTUM 3

typedef struct queue //Makiko
{
    int inicial_pos, final_pos, *array, sz;
}queue_t;
queue_t *create_queue (int sz);
void insert (queue_t *queue, int element);
bool full (queue_t *queue);
bool empty (queue_t *queue);
int get (queue_t *queue);
void printQueue(queue_t *queue);

queue_t *create_queue (int sz) //Bob
{
    queue_t *queue_v = malloc(sizeof(queue_t));
    queue_v -> sz = sz;
    queue_v -> array = malloc(queue_v -> sz* sizeof(int));
    queue_v -> inicial_pos = 0;
    queue_v -> final_pos = 0;

    return queue_v;
}


void insert (queue_t *queue, int element) //Inserir
{
    if (!full (queue))
    {
        queue -> array [queue -> final_pos] = element;
        queue -> final_pos ++;
    }    
}

bool full (queue_t *queue) //Checka para saber se esta cheio
{
    return queue -> sz == queue -> final_pos;
}

bool empty (queue_t *queue) //Checka para saber se esta vazio
{
    return queue -> inicial_pos == queue -> final_pos;
}

int get (queue_t *queue) //Get primeiro elemento, o de baixo
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
        printf("Queue is empty!");
    }
    else
    {
        for (int i = 0; i < queue->final_pos; i++)
        {
            printf("%d ", queue->array[i]);
        }
        
    }
}

typedef struct process
{
    int PID, inicial_time, *run, *blocked;
}process_t;
process_t *create_process(int sz);
process_t *insert_process(int beg, int end, int queues, int arr[]);
int find_PID(int PID, process_t *process_arr[], int n_process);

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
    process_t *process_v;
    if(queues % 2 == 0)
    {
        process_v = create_process(3);   
    }
    else
    {
        process_v = create_process(2)
    }

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
			process_v->PID = arr[i];

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
    printf("PID not found!")
}









int main()
{   

    return 0;
}