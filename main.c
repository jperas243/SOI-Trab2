#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "prints.c"
#include "queues.c"
#define ARR_SZ 11
#define SZ 225
#define QUANTUM 3
#define QUANTUM_FCFS 950
#define FNAME "input1.txt"

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

void rr(int n_process, process_t *process_arr[], int n_quantum)
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
                    j--;
                }
            }
            if (!empty(run) && (process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] == 0 || quantum == n_quantum - 1))
            {
                if (process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] != 0 && quantum == n_quantum - 1)
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
        
        if(i > 0 && quantum >= 0 && quantum < n_quantum - 1 && !empty(run))
        {
            update_run(find_PID(top(run),process_arr,n_process),process_arr);
        }
        
        if(empty(ready) && empty(run) && empty(blocked))
        {
            green("The program executed sucessfully!");
            return;
        }

        yellow("Instant");printf(" %d - ", i);
        yellow("Ready: ");printQueue(ready);printf(" |");
        yellow (" Run: "); printQueue(run);printf("|");
        yellow(" Blocked: "); printQueue(blocked);
        printf("\n");

        quantum++;
    }
}


int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        red("You forgot to pass the arguments. Use --help argument");
    }
    else if(argc > 3){
        red("I don`t need so many arguments. Use --help argument");
    }
    else
    {
        if (strcmp( argv[1], "--help") == 0)
        {
            yellow("Some examples how to use:"); printf("\n");
            printf("\n");
            printf("./compiled --fcfs filename \n");
            printf("./compiled --rr filename \n");
            printf("\n");
            red("Warning: Don`t forget to change the Quantum via the #Define");
        }
        else if (argc == 2)
        {
            red("You forgot to pass file name argument!\n Check the ./compiled --help.");
        }              
        else if (strcmp( argv[1], "--fcfs") == 0)
        {

            FILE *fp = fopen(argv[2], "r");

            if(fp == NULL)
            {
                red("File not found! Check path.");
                return 0;
            }

            process_t *process_arr [ARR_SZ];
            int inteiro, arr[SZ], comprimento = 0, ini = 0, n_processo = 0;

            while(fscanf(fp, "%d", &inteiro) != EOF)
            {
                arr[comprimento] = inteiro;
                comprimento++;

            }
            for (int j = 0; j < comprimento; ++j)
            {
                if(arr[j + 1] >= 100 || j == comprimento)
                {
                    process_arr[n_processo] = insert_process(ini, j, n_processo, arr);
                    ini = j + 1;
                    n_processo++;
                }
            }

            rr(n_processo, process_arr, QUANTUM_FCFS);


            fclose(fp);    
        }
        else if (strcmp( argv[1], "--rr") == 0)
        {
            FILE *fp = fopen(argv[2], "r");
            
            if(fp == NULL)
            {
                red("File not found! Check path.");
                return 0;
            }

            process_t *process_arr [ARR_SZ];
            int inteiro, arr[SZ], comprimento = 0, ini = 0, n_processo = 0;

            while(fscanf(fp, "%d", &inteiro) != EOF)
            {
                arr[comprimento] = inteiro;
                comprimento++;

            }
            for (int j = 0; j < comprimento; ++j)
            {
                if(arr[j + 1] >= 100 || j == comprimento)
                {
                    process_arr[n_processo] = insert_process(ini, j, n_processo, arr);
                    ini = j + 1;
                    n_processo++;
                }
            }

            rr(n_processo, process_arr, QUANTUM);


            fclose(fp);      
        }
        else
        {
            red("Use --help argument");
        }
        
    }
    return 0;
}