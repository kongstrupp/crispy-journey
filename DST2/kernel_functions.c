#include "kernel_functions.h"
#include "list.h"
#include "system_sam3x.h"
#include "at91sam3x8.h"

int TimerInt;
TCB *NextTask;
TCB *PreviousTask;

int init_kernel(void){
         Ticks = 0;
         configList();
         create_task(&idleTask,0xFFFFFFFF);                                     //HMMMMMMMMMMM deadline
         KernelMode = INIT;
         return OK;
  
}

exception create_task (void (*taskBody)(), unsigned int deadline){
         TCB *new_tcb;
         new_tcb = (TCB *) calloc (1, sizeof(TCB));
         listobj *objectTask;
         /* you must check if calloc was successful or not! */
         if(new_tcb == NULL) 
           return FAIL;
         
         objectTask = (listobj*) malloc (sizeof(listobj));
         if(objectTask == NULL) 
           return FAIL;
         
         new_tcb->PC = taskBody;
         new_tcb->SPSR = 0x21000000;
         new_tcb->Deadline = deadline;

         new_tcb->StackSeg [STACK_SIZE - 2] = 0x21000000;                       //StackSeg IS A STACK IMPLEMENTATION OF MEMORY LOCATIONS
         new_tcb->StackSeg [STACK_SIZE - 3] = (unsigned int) taskBody;
         new_tcb->SP = &(new_tcb->StackSeg [STACK_SIZE - 9]); 
         
         
         objectTask->pTask = new_tcb;
         objectTask->pMessage = NULL;
         objectTask->pNext = NULL;
         objectTask->pPrevious  = NULL;
         
         // after the mandatory initialization you can implement the rest of the code
         if(KernelMode == INIT){
           insertList(ReadyList, objectTask);
           return OK;
         }
         else{
            isr_off();
            PreviousTask = ReadyList->pHead->pPrevious->pTask;//HMMMMMMMMMMMMMMMM //UPDATE PREV TASK
            insertList(ReadyList, objectTask);
            NextTask = ReadyList->pHead->pNext->pTask;
            SwitchContext();
         }   
         
         return OK;
}

void terminate( void ){
         isr_off();
         listobj *leavingObj = extract(ReadyList->pHead);//HMMMMMMMMMMMM
         NextTask = ReadyList->pHead->pTask;
         switch_to_stack_of_next_task();

         free(leavingObj->pTask);
         free(leavingObj);
         LoadContext_In_Terminate();
  
}

void run( void ){
         Ticks = 0;
         KernelMode = RUNNING;
         NextTask = ReadyList->pHead->pTask;
         LoadContext_In_Run();
}
                         
void idleTask(){
  while(1){}                                                                    //Do nothing task
}
