/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _doublylinkedlist_
#define _doublylinkedlist_
#include <stdbool.h>
typedef struct DLinkedList* dlinkedlist;
typedef struct DLLNode* dllnode;
#include "parserDef.h"

struct DLLNode
{
    symbol val;
    struct DLLNode *next, *prev;
};

struct DLinkedList
{
    struct DLLNode *head, *tail;
    int listSize;
};

dlinkedlist createDLinkedList();
void insertBegin(struct DLinkedList *list,symbol val);
void insertEnd(struct DLinkedList *list,symbol val);
void print(struct DLinkedList *list);
void printRev(struct DLinkedList *list);

#endif