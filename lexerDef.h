/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _lexerdef_
#define _lexerdef_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashTable.h"
#define num_terminals 60
#define num_nonterminals 71
#define num_reserved 30
extern FILE *fptr;
typedef enum TK_TYPE{
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_RANGEOP,
    TK_PLUS,
    TK_MINUS, 
    TK_MUL, 
    TK_DIVIDE,    
    TK_LE,
    TK_DRIVERDEF, 
    TK_DEF, 
    TK_LT,
    TK_SQBO, 
    TK_SQBC, 
    TK_BO,
    TK_BC,
    TK_GE, 
    TK_GT, 
    TK_DRIVERENDDEF, 
    TK_ENDDEF, 
    TK_EQ,
    TK_NE,
    TK_ASSIGNOP, 
    TK_COLON,
    TK_SEMICOLON,
    TK_COMMA, 
    // Token_ID for keywords
    TK_AND,
    TK_OR,
    TK_TRUE,
    TK_FALSE, 
    TK_INTEGER, 
    TK_REAL,
    TK_BOOLEAN, 
    TK_OF,
    TK_ARRAY, 
    TK_START,
    TK_END,
    TK_DECLARE,
    TK_MODULE,
    TK_DRIVER,
    TK_PROGRAM,
    TK_GETVALUE,
    TK_PRINT,
    TK_USE,
    TK_WITH,
    TK_PARAMETERS,
    TK_TAKES,
    TK_INPUT,
    TK_RETURNS,
    TK_FOR,
    TK_IN,
    TK_SWITCH,
    TK_CASE,
    TK_BREAK,
    TK_DEFAULT,
    TK_WHILE, 
    $,
    TK_ERROR,
    TK_EOF,
    epsilon
    }tkType;


extern char *terminal_str[];
typedef struct PAIR{
    char *first;
    tkType second;
}pair;

typedef union u
{
    long long integer;
    double decimal;
    char value[20];
} lexeme;

typedef struct token
{
    lexeme lex;
    int line_no;
    tkType token_type;
} Token;
typedef Token* TOK;

// enum for TOKEN_ID
#endif