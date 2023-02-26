#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parserDef.h"
#include "stackADT.h"

parseStack init_parseStack()
{
    parseStack S;
    S.size = 0;
    S.top = NULL;
    return S;
}

bool isEmpty(parseStack S)
{
    return S.size == 0;
}

void push(parseStack S, stackNode x)
{
    // int to be changed to element
    stackNode *temp = (stackNode *)(malloc(sizeof(stackNode)));

    // define which datatype to use
    if (temp == NULL)
    {
        printf("Malloc function failed\n");
        return;
    }

    if (x.sym.is_terminal)
    {
        temp->sym.t = x.sym.t;
        temp->sym.is_terminal = true;
    }
    else
    {
        temp->sym.nt = x.sym.nt;
        temp->sym.is_terminal = false;
    }
    temp->next = S.top;
    S.top = temp;
    S.size++;

    temp = NULL;
}

void pop(parseStack S)
{
    if (isEmpty(S))
    {
        printf("Stack is empty.\n");
    }
    else
    {
        stackNode *temp = S.top;
        S.top = S.top->next;
        temp->next = NULL;
        S.size--;

        free(temp);
    }
    return;
}

symbol(parseStack S)
{
    if (isEmpty(S))
    {
        printf("Stack is empty.\n");
        return -1;
    }
    else
    {
        return S.top->sym;
    }
}