#include <stdio.h>
#include <stdlib.h>
#include "doublyLinkedList.h"

void print()
{
    struct DLLNode *temp = head;
    while (temp)
    {
        printf("%d ", temp->val);
        temp = temp->next;
    }
    printf("\n");
}

void printrev()
{
    struct DLLNode *temp = tail;
    while (temp)
    {
        printf("%d ", temp->val);
        temp = temp->prev;
    }

    printf("\n");
}

void insertBegin()
{
    int val;
    printf("Enter the value to be inserted \n");
    scanf("%d", &val);

    if (head == NULL)
    {
        struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
        new_node->val = val;
        new_node->prev = NULL;
        new_node->next = NULL;
        head = new_node;
        tail = new_node;
    }
    else
    {
        struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
        new_node->val = val;
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
    }

    print();
    printrev();
}

void insertEnd()
{
    int val;
    printf("Enter the value to be inserted \n");
    scanf("%d", &val);

    if (tail == NULL)
    {
        struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
        new_node->val = val;
        new_node->prev = NULL;
        new_node->next = NULL;
        head = new_node;
        tail = new_node;
    }
    else
    {
        struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));
        new_node->val = val;
        new_node->prev = tail;
        tail->next = new_node;
        tail = new_node;
    }

    print();
    printrev();
}

int main()
{

    printf("Enter the number of nodes\n");
    int n, val;
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("INVALID NODES\n");
    }
    else
    {
        struct DLLNode *node = (struct DLLNode *)malloc(sizeof(struct DLLNode));

        printf("Enter the value of the node\n");
        scanf("%d", &val);

        node->val = val;

        node->next = NULL;
        node->prev = NULL;

        head = node;
        tail = node;
        // head->next = NULL;
        // tail->next = NULL;
        // head->prev = NULL;
        // tail->prev = NULL;

        struct DLLNode *temp = node;

        for (int i = 2; i <= n; i++)
        {
            struct DLLNode *new_node = (struct DLLNode *)malloc(sizeof(struct DLLNode));

            printf("Enter the value of the node\n");
            scanf("%d", &val);

            new_node->val = val;
            new_node->prev = temp;
            temp->next = new_node;
            tail = new_node;
            temp = temp->next;
            temp->next = NULL;
        }
    }

    print();
    printrev();
    return 0;
}