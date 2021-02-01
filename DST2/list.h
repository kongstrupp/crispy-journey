#ifndef lists_H
#define lists_H
//// LIST INITIATION

exception List_initaion(void);


exception configList();
void insertList(list *List, listobj *Node);
void object_remove_list(listobj *temp);
listobj *extract(listobj *object_temp);
//listobj *extract_object_ReadyList();



#endif