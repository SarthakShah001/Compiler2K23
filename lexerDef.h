#ifndef _lexerdef_
#define _lexerdef_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define buff_size 43

char buffer1[buff_size + 1];
char buffer2[buff_size + 1];
bool is_buffer1_filled, is_buffer2_filled;
char *begin_ptr, *forward_ptr;
FILE *fptr;
int curr_line_no;
int char_count;
bool where_begin, where_forward; // 0 indicates buff1, 1 indicates buff2
bool retract_case;
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
    TK_EQ ,
    TK_NE ,
    TK_ASSIGNOP, 
    TK_COLON ,
    TK_SEMICOLON ,
    TK_COMMA, 
    // Token_ID for keywords
    TK_AND ,
    TK_OR ,
    TK_TRUE ,
    TK_FALSE, 
    TK_INTEGER, 
    TK_REAL ,
    TK_BOOLEAN, 
    TK_OF ,
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


char *enum_to_str[]={
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_RANGEOP",
    "TK_PLUS",
    "TK_MINUS", 
    "TK_MUL", 
    "TK_DIVIDE",    
    "TK_LE",
    "TK_DRIVERDEF", 
    "TK_DEF", 
    "TK_LT",
    "TK_SQBO", 
    "TK_SQBC", 
    "TK_BO",
    "TK_BC",
    "TK_GE", 
    "TK_GT", 
    "TK_DRIVERENDDEF", 
    "TK_ENDDEF", 
    "TK_EQ ",
    "TK_NE ",
    "TK_ASSIGNOP", 
    "TK_COLON ",
    "TK_SEMICOLON ",
    "TK_COMMA", 
    // Token_ID for keywords
    "TK_AND ",
    "TK_OR ",
    "TK_TRUE ",
    "TK_FALSE", 
    "TK_INTEGER", 
    "TK_REAL ",
    "TK_BOOLEAN", 
    "TK_OF ",
    "TK_ARRAY", 
    "TK_START",
    "TK_END",
    "TK_DECLARE",
    "TK_MODULE",
    "TK_DRIVER",
    "TK_PROGRAM",
    "TK_GETVALUE",
    "TK_PRINT",
    "TK_USE",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_TAKES",
    "TK_INPUT",
    "TK_RETURNS",
    "TK_FOR",
    "TK_IN",
    "TK_SWITCH",
    "TK_CASE",
    "TK_BREAK",
    "TK_DEFAULT",
    "TK_WHILE", 
    "$",
    "TK_ERROR",
    "TK_EOF"
};
typedef struct PAIR{
    char *first;
    tkType second;
}pair;

typedef union u
{
    int integer;
    float decimal;
    char value[20];
} lexeme;

typedef struct token
{
    lexeme lex;
    int line_no;
    tkType token_type;
} Token;
// enum for TOKEN_ID

pair reserved[30] = {
    {"AND",TK_AND},
    {"OR",TK_OR},
    {"true",TK_TRUE},
    {"false",TK_FALSE},
    {"integer",TK_INTEGER},
    {"real",TK_REAL},
    {"boolean",TK_BOOLEAN},
    {"of",TK_OF},
    {"array",TK_ARRAY},
    {"start",TK_START},
    {"end",TK_END},
    {"declare",TK_DECLARE},
    {"module",TK_MODULE},
    {"driver",TK_DRIVER},
    {"program",TK_PROGRAM},
    {"get_value",TK_GETVALUE},
    {"print",TK_PRINT},
    {"use",TK_USE},
    {"with",TK_WITH},
    {"parameters",TK_PARAMETERS},
    {"takes",TK_TAKES},
    {"input",TK_INPUT},
    {"returns",TK_RETURNS},
    {"for",TK_FOR},
    {"in",TK_IN},
    {"switch",TK_SWITCH},
    {"case",TK_CASE},
    {"break",TK_BREAK},
    {"default",TK_DEFAULT},
    {"while",TK_WHILE},
};
#endif