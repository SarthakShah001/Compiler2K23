#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parserDef.h"
#include "stackADT.h"

parse_Stack init_parseStack()
{
    parse_Stack S = (parse_Stack)(malloc(sizeof(struct stack)));
    stackNode n = (stackNode)malloc(sizeof(struct node));
    symbol s1 = (symbol)malloc(sizeof(struct SYMBOL));
    s1->is_terminal = true;
    s1->t = $;
    n->tree_ptr->s = s1;
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
    symbol s1 = (symbol)malloc(sizeof(struct SYMBOL));

    if (temp == NULL){
        printf("Malloc function failed\n");
        return;
    }

    if (x->s->is_terminal){
        s1->is_terminal = true;
        s1->t = x->s->t;
       
    }

    else{
        s1->is_terminal = false;
        s1->nt = x->s->nt;
        
    }
    temp->tree_ptr->s = s1;
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

void push_rule(parse_Stack S, dlinkedlist node){
    stackNode t = S->top;
    parseTreeNode p1 = t->tree_ptr;
    parseTreeNode temp = NULL;
    for(int i = 0; i<p1->childrenCount; i++){
        if(p1->childrenList[i]->s == node->head->val){
            temp = p1->childrenList[i];
            break;
        }
    }
    // parseTreeNode temp2 = temp;
    while(temp && temp->sibling!=NULL){
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