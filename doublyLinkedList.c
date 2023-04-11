/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include <stdio.h>
#include <stdlib.h>
#include "doublyLinkedList.h"

dlinkedlist createDLinkedList()
{
    struct DLinkedList *list = (dlinkedlist)malloc(sizeof(struct DLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->listSize = 0;
    return list;
}

void insertBegin(struct DLinkedList *list, symbol val)
{
    struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
    if (new_node == NULL)
    {
        printf("COULD NOT ALLOCATE MEMORY TO NODE\n");
    }

    new_node->val = val;

    if (list->head == NULL)
    {
        new_node->next = NULL;
        new_node->prev = NULL;
        list->head = new_node;
        list->tail = new_node;
        list->listSize = 1;
        return;
    }

    new_node->next = list->head;
    list->head->prev = new_node;
    new_node->prev = NULL;
    list->head = new_node;
    list->listSize++;
    printRev(list);
}

void insertEnd(struct DLinkedList *list, symbol val)
{
    struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
    if (new_node == NULL)
    {
        printf("COULD NOT ALLOCATE MEMORY TO NODE\n");
    }

    new_node->val = val;

    if (list->tail == NULL)
    {
        new_node->next = NULL;
        new_node->prev = NULL;
        list->head = new_node;
        list->tail = new_node;
        list->listSize = 1;
        return;
    }

    list->tail->next = new_node;
    new_node->prev = list->tail;
    list->tail = new_node;
    list->listSize++;
}

void print(struct DLinkedList *list)
{
    struct DLLNode *temp = list->head;
    while (temp)
    {
        printf("%d ", temp->val->t);
        temp = temp->next;
    }
    printf("\n");
}
void printRev(struct DLinkedList *list)
{
    struct DLLNode *temp = list->tail;
    while (temp)
    {
        printf("%d ", temp->val->is_terminal);
        temp = temp->prev;
    }

    printf("\n");
}
