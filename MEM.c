#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <string.h>

#define MEM_SIZE 200

typedef struct Slot
{
    int value;
    bool ocupado;

}Slot_t;

typedef struct MEM
{
    Slot_t *slots[MEM_SIZE];
    int size;

}MEM_t;

Slot_t *new_Slot()
{
    Slot_t *new = malloc(sizeof(Slot_t));
    new->ocupado=false;
    
    return new;
}

MEM_t *new_MEM()
{
    MEM_t *new = malloc(sizeof(MEM_t));
    for (int i = 0; i < MEM_SIZE; i++)
    {
       new->slots[i]=new_Slot();
    }
    
    new->size=MEM_SIZE;

    return new;
}

Slot_t *find_MEM(MEM_t *MEM,int index)
{
    int i=0;
    while (i!=index)
    {
        i++;
    }
    return MEM->slots[i];
    
}

void print_MEM(MEM_t *MEM)
{
    for (int i = 0; i < MEM->size; i++)
    {
        printf("POS: %d, Ocp:%d ,VAL:%d\n",i,MEM->slots[i]->ocupado,MEM->slots[i]->value);
    }
    
}
