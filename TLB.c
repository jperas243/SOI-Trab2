#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <string.h>

#define TLB_SIZE 10

typedef struct Match
{
    int PID;
    int MEM_index;

}Match_t;

typedef struct node_TLB
{
  Match_t *match;
  struct node_TLB *next;

}node_TLB;

typedef struct TLB
{
    struct node_TLB *header;

}TLB_t;


Match_t *new_Match(int PID, int index_MEM)
{
    Match_t *new = malloc(sizeof(Match_t));
    new->PID=PID;
    new->MEM_index=index_MEM;
    
    return new;
}

node_TLB *new_node_TLB()
{
    node_TLB *new = malloc(sizeof(struct node_TLB));
    new->match = NULL;
    return new;
}


TLB_t *new_TLB()
{
    TLB_t *new = malloc(sizeof(struct TLB));
    new->header = new_node_TLB();

    return new;
}

void insert_Match(TLB_t *TLB,int PID, int index_MEM)
{
    node_TLB *newValue = new_node_TLB(); 
    newValue->match = new_Match(PID,index_MEM);

    node_TLB *Atual = TLB->header;
    while (Atual->next!=NULL)
    {
        Atual = Atual->next;
    }
    Atual->next = newValue; 
}

Match_t *remove_Match(TLB_t *TLB,int PID)
{
    node_TLB *Atual = TLB->header;
    node_TLB *Prev;

    while (Atual->next!=NULL)
    {
        Prev = Atual;
        Atual = Atual->next;
        if (Atual->match->PID==PID)
        {
            Prev->next = Atual->next;
            Atual->next=NULL;
            return Atual->match;
        }
    }

    return NULL;
}

void print_TLB(TLB_t *TLB)
{
    node_TLB *Atual = TLB->header;
    if (Atual->next != NULL)
    {
        Atual = Atual->next;
        while (Atual!=NULL)
        {
            printf("PID: %d,INDEX: %d\n",Atual->match->PID,Atual->match->MEM_index);
            Atual = Atual->next;
        }  
    }
    
  
}

Match_t *find_TLB(TLB_t *TLB,int PID)
{
    node_TLB *Atual = TLB->header;
    if (Atual->next != NULL)
    {
        Atual = Atual->next;
        while (Atual!=NULL)
        {
            if (Atual->match->PID==PID)
            {
                return Atual->match;
            }
            
            Atual = Atual->next;
        }  
    }

    return NULL;
}