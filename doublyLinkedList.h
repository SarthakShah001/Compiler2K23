#ifndef _doublylinkedlist_
#define _doublylinkedlist_

struct DLLNode
{
    int val;
    struct DLLNode *next, *prev;
};

struct DLLNode *head, *tail;

//prints the linked list
void print();
void printrev();
void insertBegin();
void insertEnd();

#endif