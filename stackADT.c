#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parserDef.h"
#include "stackADT.h"

parse_Stack init_parseStack()
{
    parse_Stack S = (parse_Stack)(malloc(sizeof(struct stack)));
    S->size = 0;
    S->top = NULL;
    return S;
}

bool isEmpty(parse_Stack S)
{
    return S->size == 0;
}

void push(parse_Stack S, stackNode x){

    stackNode temp = (stackNode)(malloc(sizeof(struct node)));

    if (temp == NULL){
        printf("Malloc function failed\n");
        return;
    }

    if (x->sym->is_terminal){
        temp->sym->t = x->sym->t;
        temp->sym->is_terminal = true;
    }

    else{
        temp->sym->nt = x->sym->nt;
        temp->sym->is_terminal = false;
    }

    temp->next = S->top;
    S->top = temp;
    S->size++;

    // temp = NULL;
}

void pop(parse_Stack S){

    if (isEmpty(S)){
        printf("Stack is empty.\n");
    }

    else{
        stackNode temp = S->top;
        S->top = S->top->next;
        temp->next = NULL;
        S->size--;
        free(temp);
    }

    return;
}

stackNode top(parse_Stack S){

    if (isEmpty(S)){
        printf("Stack is empty.\n");
        return NULL;
    }

    else{
        return S->top;
    }
    
}