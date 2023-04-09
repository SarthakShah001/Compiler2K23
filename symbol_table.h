#ifndef _symbol_table_
#define _symbol_table_
#include "symbol_table_def.h"

void global_symbol_table_init();
symbol_table symbol_table_init();
ast_symbol ast_symbol_init();
void insert_symbol_table(symbol_table table,ast_symbol s);
void add_child_symbol_table(symbol_table parent, symbol_table child);
ast_symbol find_symbol(symbol_table table, char *str);
bool is_present_var(symbol_table table, char *str);
int find_mod_no(char *str);

int generate_symbol_table(parseTreeNode root, symbol_table curr, int nesting, int curr_offset,ast_symbol curr_symbol);
// returns offset

// void print_global_symbol_table();

void print_symbol_module(FILE* fs, mod sym_module);
void print_symbol_table(FILE* fs, symbol_table tab);


#endif