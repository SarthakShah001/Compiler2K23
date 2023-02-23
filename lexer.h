#include "lexerDef.h"

// initialising lexer variables
void lexer_init();

void start_lexer();

void fill_buffer();

char get_next_char();

void retract(int n);

char *get_lexeme();

void report_error(int state, char *lexeme);

Token tokenise(char tokenincoming[], int retract_length, bool is_final_state, int state, bool *istok);

Token get_next_token();




