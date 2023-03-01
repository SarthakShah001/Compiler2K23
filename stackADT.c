/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parserDef.h"
#include "stackADT.h"

parse_Stack init_parseStack()
{
    parse_Stack S = (parse_Stack)(malloc(sizeof(struct stack)));
    stackNode n = (stackNode)malloc(sizeof(struct node));
    parseTreeNode tnode=createTree();
    symbol s1 = (symbol)malloc(sizeof(struct SYMBOL));
    s1->is_terminal = true;
    s1->t = $;
    tnode->s = s1;
    // tnode->childrenCount = 0;
    // tnode->parent = NULL;
    // tnode->prevSibling = NULL;
    // tnode->sibling = NULL;
    // memset(tnode->childrenList,NULL,20);
    n->tree_ptr = tnode;
    n->next = NULL;
    S->size = 1;
    S->top = n;
    return S;
}

bool isEmpty(parse_Stack S)
{
    return S->size==0;
}

void push_on_stack(parse_Stack S, parseTreeNode x){

    stackNode temp = (stackNode)(malloc(sizeof(struct node)));

    if (temp == NULL){
        printf("temp Malloc function failed\n");
        return;
    }
    
    temp->tree_ptr = x;
    temp->next = S->top;
    S->top = temp;
    S->size++;

    return ;
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

void push_rule(parse_Stack S, dlinkedlist dlist){
    stackNode t = S->top;
    parseTreeNode p1 = t->tree_ptr;
    parseTreeNode temp = NULL;
    if(p1==NULL){
        printf("p1 null hogya\n");
        return ;
    }
    
    temp = p1->child;
    // parseTreeNode temp2 = temp;
    if(temp==NULL){
        printf("No matching rule found\n");
        return ;
    }
    while(temp->sibling!=NULL){
        temp = temp->sibling;
    }
    pop(S);
    while(temp!=NULL){
        push_on_stack(S,temp);
        temp = temp->prevSibling;
    }
    return ;
}
// struct node
// {
//     stackNode next;
//     parseTreeNode tree_ptr;
// };

// struct stack
// {
//     int size;
//     stackNode top;
// };

// struct PARSETREENODE{
//     parseTreeNode parent , sibling , prevSibling ;
//     int childrenCount ; 
//     parseTreeNode* childrenList ;
//     symbol s ;
// }; 