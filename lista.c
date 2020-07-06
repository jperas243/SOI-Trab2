#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>


#define MAX_TIMES 10
#define MAX_INSTRUCTIONS 50

//------------------------------------------------------------
typedef struct Instruction
{
    int cod_instruc;
    int cod_var;

}Instruction_t;

typedef struct Process
{
    int PID;
    int Dec_Time;
    int time_blocked;

    int Variables[10];

    Instruction_t *Instructions[MAX_INSTRUCTIONS];
    int Instruc_Index;
    int Instruc_Size;

    int size_MEM;

}Process_t;

typedef struct node
{
  Process_t *process;
  struct node *next;

}node;

typedef struct list
{
    struct node *header;

}list;

//------------------------------------------------------------
Instruction_t *new_null_Instruction()
{
    Instruction_t *new = malloc(sizeof(struct Instruction));

    return new;

}

Process_t *new_Process()
{
    Process_t *new = malloc(sizeof(Process_t));
    for (int i = 0; i < MAX_INSTRUCTIONS; i++)
    {
        new->Instructions[i]=new_null_Instruction();
    }
    
    new->time_blocked=0;
    new->Instruc_Index=0;
    new->Instruc_Size=0;
    new->size_MEM=10; //10 vars 

    return new;
}



Instruction_t *new_Instruction(int ins,int var)
{
    Instruction_t *new = malloc(sizeof(struct Instruction));

    new->cod_instruc=ins;
    new->cod_var=var;

    return new;

}


node *new_node()
{
    node *new = malloc(sizeof(struct node));
    new->process = new_Process();
    return new;
}

list *new_list()
{

    list *new = malloc(sizeof(struct list));
    new->header =  new_node();

    return new;
}
//------------------------------------------------------------

void process_print(Process_t *process){

    printf("[PID: %d, DEC: %d\n",process->PID,process->Dec_Time);
    for (int i = 0; i < process->Instruc_Size; i++)
    {
        printf("Instruc: %d %d\n",process->Instructions[i]->cod_instruc,process->Instructions[i]->cod_var);
    }
    printf("\n");
    
}

void list_insert(list *fila, Process_t *process){

    node *newValue = new_node(); 
    newValue->process = process;

    node *Atual = fila->header;
    while (Atual->next!=NULL)
    {
        Atual = Atual->next;
    }
    Atual->next = newValue; 
}



Process_t *list_remove(list *fila, Process_t *process){

    node *Atual = fila->header;
    node *Prev;

    while (Atual->next!=NULL)
    {
        Prev = Atual;
        Atual = Atual->next;
        if (Atual->process->PID==process->PID)
        {
            Prev->next = Atual->next;
            Atual->next=NULL;
            return Atual->process;
        }
    }
    return NULL;
}

void list_print(struct list *list){

    node *Atual = list->header;    
    if (Atual->next!=NULL)
    {
        Atual = Atual->next;
        while (Atual!=NULL)
        {   
            printf("%d ",Atual->process->PID);        
            Atual = Atual->next;
        }
    }
    
}

Process_t *list_first(list *fila){

    node *Atual = fila->header;

    if (Atual->next!=NULL)
    {
        return Atual->next->process;
    }
    
    return NULL;
}