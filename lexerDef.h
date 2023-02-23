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
    char token_type[20];
} Token;


char reserved[30][20] = {
    "AND",
    "OR",
    "true",
    "false",
    "integer",
    "real",
    "boolean",
    "of",
    "array",
    "start",
    "end",
    "declare",
    "module",
    "driver",
    "program",
    "get_value",
    "print",
    "use",
    "with",
    "parameters",
    "takes",
    "input",
    "returns",
    "for",
    "in",
    "switch",
    "case",
    "break",
    "default",
    "while",
};