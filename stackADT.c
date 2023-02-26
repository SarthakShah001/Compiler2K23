#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parserDef.h"
#include "stackADT.h"

parse_Stack* init_parseStack()
{
    parse_Stack* S = (parse_Stack*)(malloc(sizeof(parse_Stack)));
    S->size = 0;
    S->top = NULL;
    return S;
}

bool isEmpty(parseStack* S)
{
    return S->size == 0;
}

void push(parseStack* S, stackNode* x)
{
    // int to be changed to element
    stackNode *temp = (stackNode *)(malloc(sizeof(stackNode)));

    // define which datatype to use
    if (temp == NULL)
    {
        printf("Malloc function failed\n");
        return;
    }

    if (x->sym.is_terminal)
    {
        temp->symbol->t = x->symbol->t;
        temp->symbol->is_terminal = true;
    }
    else
    {
        temp->sym.nt = x->sym.nt;
        temp->sym.is_terminal = false;
    }
    temp->next = S->top;
    S->top = temp;
    S->size++;

    temp = NULL;
}

void pop(parseStack* St)
{
    if (isEmpty(S))
    {
        printf("Stack is empty.\n");
    }
    else
    {
        stackNode *temp = St->top;
        St->top = St->top->next;
        temp->next = NULL;
        St->size--;

        free(temp);
    }
    return;
}

symbol(parseStack* St)
{
    if (isEmpty(St))
    {
        printf("Stack is empty.\n");
        return -1;
    }
    else
    {
        return St->top->sym;
    }
}