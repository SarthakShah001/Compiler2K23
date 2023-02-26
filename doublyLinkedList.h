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