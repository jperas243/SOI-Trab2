#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "prints.c"
#include "process.c"
#define ARR_SZ 11
#define SZ 225
#define QUANTUM_RR 3
#define QUANTUM_FCFS 950

void scheduler(int n_process, process_t *process_arr[], int n_quantum)
{
    queue_t *ready = create_queue(SZ); 
    queue_t *run = create_queue(SZ);
    queue_t *blocked = create_queue(SZ);
    int quantum = -1;


    for (int i = 0; true; i++) //Instant (infinite loop)
    {
        if (!empty(run) || !empty(ready) || !empty(blocked))
        {
            for (int j = 0; j < blocked->final_pos; j++)
            {
                if(process_arr[find_PID( blocked->array[j], process_arr, n_process)]->blocked[0] == 0) //If blocked has reached the end
                {
                    insert(ready,get_pos(blocked,j));
                    update_index_blocked(find_PID(top(ready),process_arr,n_process),process_arr,(ready->final_pos - ready->inicial_pos));
                    j--;
                }
            }
            if (!empty(run) && (process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] == 0 || quantum == n_quantum - 1)) //When RUN reaches the end or Quantum
            {
                if (process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] != 0 && quantum == n_quantum - 1) //If reached the max quantum
                {
                    insert(ready,get(run));    
                }
                else if (process_arr[find_PID(top(run), process_arr, n_process)]->blocked[0] != 0 && process_arr[find_PID( run->array[0], process_arr, n_process)]->run[0] == 0) //If run reached the end
                { 
                    insert(blocked, get(run));
                    update_index_run(find_PID(top(blocked),process_arr,n_process),process_arr, (blocked->final_pos - blocked->inicial_pos));
                }
                else //When everything finishes
                {
                    get(run);
                }
                
            }
            for (int j = 0; j < n_process; j++) //Add processes to READY
            {
                if(process_arr[j]->inicial_time == i)  
                {
                    insert(ready, process_arr[j]->PID);
                }
            }
            if (!empty(blocked)) //If there is anything on the blocked it will decrease the blocked
            {
                for (int j = 0; j < blocked->final_pos; j++)
                {
                    update_blocked(find_PID(blocked->array[j],process_arr,n_process),process_arr);
                } 
            }
            if(!empty(ready) && empty(run)) //Add to run if there`s nothing there
            { 
                insert(run, get(ready));
                quantum = -1; 
                update_run(find_PID(top(run),process_arr,n_process),process_arr); 
            }
        }

        else
        {
            for (int j = 0; j < n_process; j++) //Add processes to READY
            {
                if(process_arr[j]->inicial_time == i)  
                {
                    insert(ready, process_arr[j]->PID);
                }
            }
            insert(run, get(ready));
            quantum = -1;
            update_run(find_PID(top(run),process_arr, n_process), process_arr);
        }
        
        if(i > 0 && quantum >= 0 && quantum < n_quantum - 1 && !empty(run)) //Decrease the run if there is something there
        {
            update_run(find_PID(top(run),process_arr,n_process),process_arr);
        }
        
        if(empty(ready) && empty(run) && empty(blocked)) //If everything is done it finishes
        {
            green("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
            printf("\n");
            green("┃  The program run Successfully (✓) ┃");
            printf("\n");
            green("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
            printf("\n");
	        return;
        }

        print_scheduler(i,ready,run,blocked);

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

            int beg = 0, process_n = 0,size = 0, integer,arr[SZ];
            process_t *process_arr [ARR_SZ];

            while(fscanf(fp, "%d", &integer) != EOF)
            {
                arr[size] = integer;
                size++;
            }

            for (int j = 0; j < size; ++j)
            {
                if(arr[j + 1] >= 100 || j == size)
                {
                    process_arr[process_n] = insert_process(beg, j, process_n, arr);
                    beg = j + 1;
                    process_n++;
                }
            }

            scheduler(process_n, process_arr, QUANTUM_FCFS);


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

            int beg = 0, process_n = 0,size = 0, integer,arr[SZ];
            process_t *process_arr [ARR_SZ];

            while(fscanf(fp, "%d", &integer) != EOF)
            {
                arr[size] = integer;
                size++;
            }

            for (int j = 0; j < size; ++j)
            {
                if(arr[j + 1] >= 100 || j == size)
                {
                    process_arr[process_n] = insert_process(beg, j, process_n, arr);
                    beg = j + 1;
                    process_n++;
                }
            }

            scheduler(process_n, process_arr, QUANTUM_RR);


            fclose(fp);      
        }
        else
        {
            red("Use --help argument");
        }
        
    }
    return 0;
}
