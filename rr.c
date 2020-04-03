#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define SZ 101
#define QUANTUM 2
#define FNAME "input3.txt"

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
int top(queue_t *queue);
int get_pos(queue_t *queue, int pos);

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
    printf("Empty!");
}
else
{
    for (int i = 0; i < queue->final_pos; i++)
    {
        printf("%d ", queue->array[i]);
    }
    
}
}

int top(queue_t *queue)
{
return queue->array[queue->final_pos - 1];
}

int get_pos(queue_t *queue, int pos)
{
int tmp = queue->array[pos];
for (int i = pos; i < queue->final_pos; i++)
{
    queue->array[i] = queue->array[i + 1];
}
queue->final_pos--;
return tmp;
}


typedef struct process
{
int PID, inicial_time, *run, *blocked;
}process_t;
process_t *create_process(int sz);
process_t *insert_process(int beg, int end, int queues, int arr[]);
int find_PID(int PID, process_t *process_arr[], int n_process);
void update_run(int n_process, process_t *process_arr[]);
void update_blocked(int n_process, process_t *process_arr[]);
void update_index_run(int n_process, process_t *process_arr[], int size);
void update_index_blocked(int n_process, process_t *process_arr[], int size);

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
printf("PID not found!");
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
    for (int i = 0; i <= size; i++)
    {
        if(process_arr[n_process] -> run[i] > 1000)
        {
            process_arr[n_process] -> run[i] = 0;
        }
    }

    for (int i = 0; i <= size; ++i)
    {
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
    }
    
    for (int i = 0; i <= size; i++)
    {
        process_arr[n_process] -> blocked[i] = process_arr[n_process] -> blocked[i+1];
    }
}

void rr(int n_process, process_t *process_arr[])
{
queue_t *ready = create_queue(SZ); 
queue_t *run = create_queue(SZ);
queue_t *blocked = create_queue(SZ);
int quantum = -1;

for (int i = 0; i < 65; i++) //Instantes
{
    if (!empty(run) || !empty(ready) || !empty(blocked))
    {
        for (int j = 0; j < blocked->final_pos; j++)
        {
            if(process_arr[find_PID( blocked->array[j], process_arr, n_process)]->blocked[0] == 0) //Do blocked para o Ready
            {
                insert(ready,get_pos(blocked,j));
                update_index_blocked(find_PID(top(ready),process_arr,n_process),process_arr,(ready->final_pos - ready->inicial_pos)); 
            }
        }
        if (!empty(run) && (process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] == 0 || quantum == QUANTUM))
        {
            if (process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] != 0 && quantum == QUANTUM)
            {
                insert(ready,get(run));    
            }
            else if (process_arr[find_PID(top(run), process_arr, n_process)]->blocked[0] != 0 && process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] == 0)
            { 
                insert(blocked, get(run));
                update_index_run(find_PID(top(blocked),process_arr,n_process),process_arr, (blocked->final_pos - blocked->inicial_pos));
            }
            else
            {
                get(run);
            }
            
        }
        for (int j = 0; j < n_process; j++)
        {
            if(process_arr[j]->inicial_time == i)  //Add processes to READY
            {
                insert(ready, process_arr[j]->PID);
            }
        }
        if (!empty(blocked))
        {
            
            for (int j = 0; j < blocked->final_pos; j++)
            {
                update_blocked(find_PID(blocked->array[j],process_arr,n_process),process_arr);
            } 
        }
        if(!empty(ready) && empty(run)) //Inserir se nao estiver nada no run
        { 
            insert(run, get(ready));
            quantum = -1; 
            update_run(find_PID(top(run),process_arr,n_process),process_arr); 
        }
    }

    else
    {
        for (int j = 0; j < n_process; j++)
        {
            if(process_arr[j]->inicial_time == i)  //Add processes to READY
            {
                insert(ready, process_arr[j]->PID);
            }
        }
        insert(run, get(ready));
        quantum = -1;
        update_run(find_PID(top(run),process_arr, n_process), process_arr);
    }
    
    if(i > 0 && quantum >= 0 && quantum < QUANTUM && !empty(run))
    {
        update_run(find_PID(top(run),process_arr,n_process),process_arr);
    }
    
    if(empty(ready) && empty(run) && empty(blocked))
    {
        return;
    }

    printf("Instante: %d || Ready:  ", i); printQueue(ready);
    printf("  || Run:  "); printQueue(run);
    printf("  || Blocked:  "); printQueue(blocked);
    //printf("  || Quantum: %d", quantum);
    printf("\n");

    quantum++;
}


}






int main()
{

FILE *fp = fopen(FNAME, "r");

process_t *process_arr [SZ];
int inteiro, arr[SZ], comprimento = 0, ini = 0, n_processo = 0;

while(fscanf(fp, "%d", &inteiro) != EOF)
{
    arr[comprimento] = inteiro;
    comprimento++;

}

for (int j = 0; j < comprimento; ++j)
{
    if(arr[j + 1] >= 100 || arr[j + 1] == EOF)
    {
        process_arr[n_processo] = insert_process(ini, j, n_processo, arr);
        ini = j + 1;
        n_processo++;
    }
}

rr(n_processo, process_arr);
printf("Acabou");



fclose(fp);

return 0;
}
