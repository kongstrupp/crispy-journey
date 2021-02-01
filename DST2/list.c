#include "kernel_functions.h"
#include "list.h"
#include <stdlib.h>

int Ticks;
int KernelMode;



list *ReadyList;
list *WaitingList;
list *TimerList;

exception configList(){
  /*READYLIST*/
  ReadyList = (list*) malloc (sizeof(list));
  if(ReadyList == NULL)
    return FAIL;
  
  ReadyList->pHead = (listobj*) malloc (sizeof(listobj));
  if(ReadyList->pHead == NULL)
    return FAIL;
  
  ReadyList->pTail = ReadyList->pHead;
  if(ReadyList->pTail == NULL)
    return FAIL;
  
  ReadyList->pHead->pNext = ReadyList->pTail;
  ReadyList->pHead->pPrevious = NULL;
  ReadyList->pTail->pNext = NULL;
  ReadyList->pTail->pPrevious = ReadyList->pHead;
  
  /*WAITINGLIST*/
  WaitingList = (list*) malloc (sizeof(list));
  if(WaitingList == NULL)
    return FAIL;
  
  WaitingList->pHead = (listobj*) malloc (sizeof(listobj));
  if(WaitingList->pHead == NULL)
    return FAIL;
  
  WaitingList->pTail = WaitingList->pHead;
  if(WaitingList->pTail == NULL)
    return FAIL;
  
  WaitingList->pHead->pNext = WaitingList->pTail;
  WaitingList->pHead->pPrevious = NULL;
  WaitingList->pTail->pNext = NULL;
  WaitingList->pTail->pPrevious = WaitingList->pHead;
  
  /*TIMERLIST*/
  TimerList = (list*) malloc (sizeof(list));
  if(TimerList == NULL)
    return FAIL;
  
  TimerList->pHead = (listobj*) malloc (sizeof(listobj));
  if(TimerList->pHead == NULL)
    return FAIL;
  
  TimerList->pTail = TimerList->pHead;
  if(TimerList->pTail == NULL)
    return FAIL;
  
  TimerList->pHead->pNext = TimerList->pTail;
  TimerList->pHead->pPrevious = NULL;
  TimerList->pTail->pNext = NULL;
  TimerList->pTail->pPrevious = TimerList->pHead;

  return SUCCESS;
}

void insertList(list *List, listobj *Node) {
    listobj *object_temp;
    object_temp = List->pHead->pNext;

    
  //  if (List->pTail == object_temp) {
        List->pHead->pNext = Node;
        List->pTail->pPrevious = Node;
        Node->pPrevious = List->pHead;
        Node->pNext = List->pTail;
        return;
  //  }

    if (&List == &TimerList) {
        while (Node->nTCnt > object_temp->nTCnt) {
            object_temp = object_temp->pNext;
            if (object_temp == List->pTail)
                break;
        }
    } else {
        while (Node->pTask->Deadline > object_temp->pTask->Deadline) {
            object_temp = object_temp->pNext;
            if (object_temp == List->pTail)
                break;
        }
    }

    Node->pNext = object_temp;
    Node->pPrevious = object_temp->pPrevious;
    object_temp->pPrevious->pNext = Node;
    object_temp->pPrevious = Node;
    return;
}

void object_remove_list(listobj *temp){
    temp->pNext->pPrevious = temp->pPrevious;
    temp->pPrevious->pNext = temp->pNext;
    temp->pPrevious = NULL;
    temp->pNext = NULL;
}
/*
listobj *extract_object_ReadyList(){
    listobj *_temp = NULL;
    if (ReadyList->pHead->pNext == ReadyList->pTail)
        return _temp;
    _temp = ReadyList->pHead->pNext;
    object_remove_list(_temp);
    return _temp;
}*/

listobj *extract(listobj *object_temp) {

    listobj *temp = NULL;
    if (ReadyList->pHead->pNext == ReadyList->pTail) {
        return NULL;
    }
    temp = ReadyList->pHead->pNext;
    object_remove_list(temp);
    return temp;

}
