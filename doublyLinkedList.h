#ifndef _doublylinkedlist_
#define _doublylinkedlist_
#include <stdbool.h>
#include "parserDef.h"

struct DLLNode
{
    int val;
    struct DLLNode *next, *prev;
};
// typedef struct DLinkedList dlinkedlist;
struct DLinkedList
{
    struct DLLNode *head, *tail;
    int listSize;
};

struct DLinkedList createDLinkedList();
void insertBegin(struct DLinkedList *list, int val);
void insertEnd(struct DLinkedList *list, int val);
void print(struct DLinkedList *list);
void printRev(struct DLinkedList *list);

#endif