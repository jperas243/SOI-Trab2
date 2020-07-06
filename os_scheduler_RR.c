#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <string.h>
#include "lista.c"
#include "TLB.c"
#include "MEM.c"



#define C 3
#define pag_size 10
#define mem_size 200

char* integer_to_string(int x)
{
    char* buffer = malloc(sizeof(char) * sizeof(int) * 4 + 1);
    if (buffer)
    {
         sprintf(buffer, "%d", x);
    }
    return buffer; // caller is expected to invoke free() on this buffer to release memory
}

void clear_process_MEM(MEM_t *MEM,TLB_t *TLB,int start_index_MEM,int final_index_MEM)
{
    for(int i=start_index_MEM;i<=final_index_MEM;i++)
    {
        MEM->slots[i]=NULL;
    }
}

int insert_process_MEM(TLB_t *TLB,MEM_t *MEM,int index_atual,Process_t *process)
{
    int index;
    int count=0;
    bool volta = false;
    bool found = false;



    int ocupation=process->size_MEM/pag_size;
    if (process->size_MEM%pag_size!=0)
    {
        ocupation++;
    }
    
    index=index_atual;


    while (!volta && !found)
    {
        if (index>=mem_size)
        {
            index=0;
        }

        if (MEM->slots[index]->ocupado==false)
        {
            bool valid=true;
            int save=index;
            
            for (int i = 0; i < ocupation*pag_size; i++)
            {          
                if (MEM->slots[index]->ocupado!=false || index>=mem_size)
                {
                    valid=false;
                    break;
                }
                index++;
            }
            index=save;

            if (valid==true)
            {

                insert_Match(TLB,process->PID,index);

                for (int i = index; i < index+10; i++)
                {
                    MEM->slots[i]->value=0;
                    MEM->slots[i]->ocupado=true;
                }
                index=index+10;

                int ins_index = 0;

                for (int i = index; i < index+process->size_MEM-10; i=i+2)
                {
                    MEM->slots[i]->value=process->Instructions[ins_index]->cod_instruc;
                    MEM->slots[i+1]->value=process->Instructions[ins_index]->cod_var;

                    MEM->slots[i]->ocupado=true;
                    MEM->slots[i+1]->ocupado=true;

                    ins_index++;

                }
                index=index+process->size_MEM-10;

                found=true;
                break;
            }


        }
        
        if (index==index_atual && count!=0)
        {
            printf("no space");
            volta=true;
            return index_atual;
        }
        count++;
        index=index+pag_size;
    }
    

    return index;
}

char *execute(TLB_t *TLB,MEM_t *MEM,list *BLOCKED,Process_t *process, Instruction_t *ins)
{
    Match_t *match;
    Slot_t *slot_var;
                    


    if (ins->cod_instruc==0) //ZER
    {
        if (ins->cod_var<0 || ins->cod_var>10)
        {
            return "falha de segurança";
        }
        


        match = find_TLB(TLB,process->PID);

        int index_var= match->MEM_index + ins->cod_var;

        slot_var = find_MEM(MEM,index_var);

        slot_var->value=0;


    }
    else if (ins->cod_instruc==1) //INC
    {
        if (ins->cod_var<0 || ins->cod_var>10)
        {
            return "falha de segurança";
        }

        match = find_TLB(TLB,process->PID);

        int index_var= match->MEM_index + ins->cod_var;

        slot_var = find_MEM(MEM,index_var);

        slot_var->value++;


    }
    else if (ins->cod_instruc==2) //DEC
    {
        if (ins->cod_var<0 || ins->cod_var>10)
        {
            return "falha de segurança";
        }

        match = find_TLB(TLB,process->PID);

        int index_var= match->MEM_index + ins->cod_var;

        slot_var = find_MEM(MEM,index_var);

        slot_var->value--;

    }
    else if (ins->cod_instruc==3) //FRK
    {


    }
    else if (ins->cod_instruc==4) //JFW
    {
        match = find_TLB(TLB,process->PID);

        int index_ins= match->MEM_index + 10 + process->Instruc_Index+ins->cod_var;

        //printf("index_ins:%d\n",index_ins);
        //printf("index_sup:%d\n",match->MEM_index+process->Instruc_Size+10-1);
        //printf("index_inf:%d\n",match->MEM_index + 10);


        if (index_ins>match->MEM_index+process->Instruc_Size+10-1 || index_ins < match->MEM_index+10)
        {
            return "Falha de segmentação";
        }
        process->Instruc_Index=process->Instruc_Index+ins->cod_var;

        return "jfw";

    }
    else if (ins->cod_instruc==5) //JBK
    {
        match = find_TLB(TLB,process->PID);

        int index_ins= match->MEM_index + 10 + process->Instruc_Index-ins->cod_var;

        //printf("index_ins:%d\n",index_ins);
        //printf("index_sup:%d\n",match->MEM_index+process->Instruc_Size+10-1);
        //printf("index_inf:%d\n",match->MEM_index + 10);


        if (index_ins>match->MEM_index+process->Instruc_Size+10-1 || index_ins < match->MEM_index+10)
        {
            return "Falha de segmentação";
        }
        process->Instruc_Index=process->Instruc_Index-ins->cod_var;

        return "jbk"; 

    }
    else if (ins->cod_instruc==6) //DSK
    {
        process->time_blocked=5+process->time_blocked;
        list_insert(BLOCKED,process);
        return "dsk";

    }
    else if (ins->cod_instruc==7) //JIZ
    {
        if (ins->cod_var<0 || ins->cod_var>10)
        {
            return "falha de segurança";
        }

        match = find_TLB(TLB,process->PID);

        int index_var= match->MEM_index + ins->cod_var;


        slot_var = find_MEM(MEM,index_var);

        if (slot_var->value==0)
        {
            int index_ins= match->MEM_index + 10 + process->Instruc_Index+2;

            printf("index_ins:%d\n",index_ins);
            printf("index_sup:%d\n",match->MEM_index+process->Instruc_Size+10-1);
            printf("index_inf:%d\n",match->MEM_index + 10);

            if (index_ins>=match->MEM_index+process->Instruc_Size+10-1 || index_ins < match->MEM_index+10)
            {
                return "Falha de segmentação";
            }
            process->Instruc_Index=process->Instruc_Index+2;
        }
        

        //jizz e fork + delete process MEM
    }
    else if (ins->cod_instruc==8) //print
    {

        if (ins->cod_var<0 || ins->cod_var>10)
        {
            return "falha de segurança";
        }
        
        Match_t *match = find_TLB(TLB,process->PID);

        int index_var= match->MEM_index + ins->cod_var;

        Slot_t *slot_var = find_MEM(MEM,index_var);

        return integer_to_string(slot_var->value);
        
    }
    else //HLT
    {
        return "done";
    }
    
    return "";

}


void simulate_OS(list *Processos)
{
    int clock = 0;
    int C_Atual = 0;
    char *status;

    list *READY = new_list();
    Process_t *RUN = NULL;
    list *BLOCKED = new_list();
    Process_t *new = NULL; //varialvel global de auxilio  

    TLB_t *TLB = new_TLB();
    MEM_t *MEM = new_MEM(); //memoria do sistema dividida 
    int MEM_index=0;

    
    while (Processos->header->next!=NULL || RUN!=NULL || BLOCKED->header->next!=NULL)
    {

        if (RUN!=NULL)
        {
            printf("RUN PID:%d,SIZE_INS:%d ,INDEX_INS:%d\n",RUN->PID,RUN->Instruc_Size,RUN->Instruc_Index);
            printf("INS_cod:%d, VAR:%d\n",RUN->Instructions[RUN->Instruc_Index]->cod_instruc,RUN->Instructions[RUN->Instruc_Index]->cod_var);
            status=execute(TLB,MEM,BLOCKED,RUN,RUN->Instructions[RUN->Instruc_Index]);
            if (strcmp("done",status)==0)
            {
                status="";
                RUN=NULL;
            }
            else if(strcmp("dsk",status)==0)
            {
                status="";
                RUN->Instruc_Index++;
                RUN=NULL;
            }
            else if(strcmp("jfw",status)==0)
            {
                status="";
            }
            else if(strcmp("jbk",status)==0)
            {
                status="";
            }
            else
            {
                RUN->Instruc_Index++;
            }
        
        } 

        status = "";
        bool repeat = true;
        node *Atual = new_node();

        while (repeat)
        {
            repeat=false;
            Atual = BLOCKED->header->next;   
            if (Atual!=NULL)
            {
                while (Atual!=NULL)
                {   
                    if (Atual->process->time_blocked==0)
                    {
                        new = list_remove(BLOCKED,Atual->process);
                        list_insert(READY,new);
                        repeat=true;
                    }
                    Atual = Atual->next;
                }
            }
        }

        Atual = BLOCKED->header->next;   
        if (Atual!=NULL)
        {
            while (Atual!=NULL)
            {   
                if (Atual->process->time_blocked!=0)
                {
                    Atual->process->time_blocked--;
                }
                Atual = Atual->next;
            }
        }

        //done
        //---------------------------------------------------------------



        if (RUN!=NULL && C_Atual==C)
        {
            list_insert(READY,RUN);
            RUN=NULL;
            C_Atual=0;
        }

        //---------------------------------------------------------------


        repeat = true;
        Atual = new_node();

        while (repeat)
        {
            repeat=false;
            Atual = Processos->header->next;   
            while (Atual!=NULL)
            {   
                if (Atual->process->Dec_Time==clock)
                {
                    MEM_index=insert_process_MEM(TLB,MEM,MEM_index,Atual->process);
                    new = list_remove(Processos,Atual->process);
                    list_insert(READY,new);
                    repeat=true;
                    break;
                }
                Atual=Atual->next;
            }  
        } 

       

        if (RUN==NULL)
        {
            new = list_first(READY);
            new = list_remove(READY,new);
            RUN = new; 
        }



        printf("%d| ",clock);
        
        printf("%s | ",status);
        
        printf("READY ");
        list_print(READY);
        printf(" |RUN ");
        if (RUN!=NULL)
        {
            printf("%d ",RUN->PID);
        }
        printf(" |BLOCKED ");
        list_print(BLOCKED);
        printf("\n");

        C_Atual++;
        clock++;
        printf("-------------------------\n");

        
        //print_MEM(MEM);
        //print_TLB(TLB);

        //sleep(2);
    }

    
}


int main(int argc, char const *argv[])
{
    int pid=1;
    char instruc[4];
    int var;
    Process_t *actual_process = new_Process();
    actual_process->PID = 0;


    list *processos = new_list();

    FILE *file_process;
    file_process = fopen("teste1.txt","r");
    
    char buffer[8];

    while (fgets(buffer,8,file_process)!=NULL)
    {
        sprintf(instruc,"%c%c%c",buffer[0],buffer[1],buffer[2]);

        char buffer_var[3];
        buffer_var[0]=buffer[4];
        buffer_var[1]=buffer[5];
        var=atoi(buffer_var); 
        

        
        if (strcmp(instruc,"INI")==0)
        {
            if (actual_process->PID==0)
            {
                actual_process->PID = pid;
                pid=pid+1;
                
                actual_process->Dec_Time = var;

            }
            else
            {
                list_insert(processos,actual_process);
                actual_process = new_Process();
                actual_process->PID = pid;
                pid=pid+1;
                
                actual_process->Dec_Time = var;

            }

        }
        else if (strcmp(instruc,"ZER")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(0,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

            
        }
        else if (strcmp(instruc,"INC")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(1,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

        }
        else if (strcmp(instruc,"DEC")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(2,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

        }
        else if (strcmp(instruc,"FRK")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(3,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

        }
        else if (strcmp(instruc,"JFW")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(4,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

        }
        else if (strcmp(instruc,"JBK")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(5,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

        }
        else if (strcmp(instruc,"DSK")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(6,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;
        
        }
        else if (strcmp(instruc,"JIZ")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(7,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

        }
        else if (strcmp(instruc,"PRT")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(8,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;

        }
        else if (strcmp(instruc,"HLT")==0)
        {
            actual_process->Instructions[actual_process->Instruc_Size]=new_Instruction(9,var);
            actual_process->Instruc_Size++;
            actual_process->size_MEM=actual_process->size_MEM+2;
        }
               
    }
    list_insert(processos,actual_process);

    simulate_OS(processos);

    return 0;
}
