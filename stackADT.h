#include "parserDef.h"
#include "parseTree.h"

typedef struct node* stackNode;
typedef struct stack* parse_Stack;

struct node
{
    stackNode next;
    parseTreeNode tree_ptr;
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
void push_on_stack(parse_Stack S, parseTreeNode x);

// function to pop from parseStack
void pop(parse_Stack S);

// function to get the top of pareStack
stackNode top(parse_Stack S);

// function to push symbols of rule acc to stackTop
void push_rule(parse_Stack S, dlinkedlist dll);


