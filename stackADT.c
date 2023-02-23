#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stackADT.h"

stack createStack(){
    stack S;
    S.size = 0;
    S.top = NULL;
    return S;
}

bool isEmpty(stack S){
    return S.size==0;
}

void push(stack S, int value){
    // int to be changed to element
    StackNode temp = (StackNode) malloc(sizeof(struct stacknode));

    // define which datatype to use
    if(temp==NULL){
        printf("Malloc function failed\n");
        return ;
    }

    temp->val = value;
    temp->next = S.top;
    S.top = temp;
    S.size++;

    temp = NULL;

}

void pop(stack S){
    if(isEmpty(S)){
        printf("Stack is empty.\n");
    }
    else{
        StackNode temp = S.top;
        S.top = S.top->next;
        temp->next = NULL;
        S.size--;

        free(temp);
    }
    return ;
}

void top(stack S){
    if(isEmpty(S)){
        printf("Stack is empty.\n");
        return ;
    }
    else{
        return S.top->val;
    }
}
