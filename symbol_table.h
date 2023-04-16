/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _symbol_table_
#define _symbol_table_
#include "symbol_table_def.h"

void global_symbol_table_init();
symbol_table symbol_table_init();
ast_symbol ast_symbol_init();
bool is_same_type(tkType t1, tkType t2);
bool is_boolean(tkType t1);
void insert_symbol_table(symbol_table table, ast_symbol s);
void add_child_symbol_table(symbol_table parent, symbol_table child);
ast_symbol find_symbol(symbol_table table, char *str);
ast_symbol f_s(symbol_table table, char *str);
ast_symbol find_in_list(symbol_list_node l, char *str);
bool is_present_var(symbol_table table, char *str);
int range_cal(ast_symbol curr_symbol);
int find_mod_no(char *str);
int find_width(tkType t);
int generate_symbol_table(parseTreeNode root, symbol_table curr, int nesting, int curr_offset,ast_symbol curr_symbol,int currscope[2]);
// returns offset

// void print_global_symbol_table();

void print_symbol_module(mod sym_module);
void print_symbol_table_list(mod sym_module, symbol_list_node in);
void print_symbol_table(symbol_table tab);

void print_symbol_module_array(mod sym_module);
void print_symbol_table_list_array(mod sym_module, symbol_list_node in);
void print_symbol_table_array(symbol_table tab);
int pass2(parseTreeNode root, int mod_no, symbol_table st, int child_count);
#endif