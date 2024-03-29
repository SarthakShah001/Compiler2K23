/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _lexer_
#define _lexer_

#include "lexerDef.h"
// #include "parser.h"
// initialising lexer variables
void lexer_init();

//starting lexer by filling buffer 1
void start_lexer();

//handling twin buffer through this
void fill_buffer();

// gives the current next char through forward ptr
char get_next_char();

// retract the forward ptr by n length
void retract(int n);

// returns the lexeme according to current begin and forward_ptr
char *get_lexeme();

// prints the type of error and it's location acc to the state of the dfa it occured
void report_error(int state, char *lexeme);

//tokenises the current lexeme
Token tokenise(tkType tokenincoming, int retract_length, bool is_final_state, int state, bool *istok);

//returns next token
Token get_next_token();

void print_tokens(FILE *fptr, int buffsize);


#endif



