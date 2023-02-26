#include "parserDef.h"

typedef struct node* stackNode;
typedef struct stack* parse_Stack;

struct node
{
    symbol sym;
    stackNode next;
};

struct stack
{
    int size;
    stackNode top;
};

// function to create parseStack
parse_Stack init_parseStack();

// function to check if parseStack is empty
bool isEmpty(parse_Stack S);

// function to push on parseStack

void push(parse_Stack S, stackNode x);

// function to pop from parseStack
void pop(parse_Stack S);

// function to get the top of pareStack
stackNode top(parse_Stack S);
