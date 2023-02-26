#include "parserDef.h"

typedef struct node stackNode;
struct node
{
    symbol sym;
    stackNode *next;
};

typedef struct
{
    int size;
    stackNode *top;
} parseStack;

// function to create parseStack
parseStack init_parseStack();

// function to check if parseStack is empty
bool isEmpty(parseStack S);

// function to push on parseStack
void push(parseStack S, stackNode x);

// function to pop from parseStack
void pop(parseStack S);

// function to get the top of pareStack
symbol top(parseStack S);
