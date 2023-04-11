/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _doublylinkedlist_
#define _doublylinkedlist_
#include <stdbool.h>
typedef struct DLinkedList *dlinkedlist;
typedef struct DLLNode *dllnode;
#include "parserDef.h"

// Substructure to represent grammer rule RHS
struct DLLNode
{
    symbol val;
    struct DLLNode *next, *prev;
};

// Complete Structure to represent grammer rule RHS
struct DLinkedList
{
    struct DLLNode *head, *tail;
    int listSize;
};
// creates the Linked list and return the pointer to it
dlinkedlist createDLinkedList();
// inserting at begin in list
void insertBegin(struct DLinkedList *list, symbol val);
// inserting at end in list
void insertEnd(struct DLinkedList *list, symbol val);
// print function to debug our code
void print(struct DLinkedList *list);
// same print rev as we used linked list in reverse direction like while pushing rule into stack
void printRev(struct DLinkedList *list);

#endif