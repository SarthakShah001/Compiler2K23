#ifndef _symbol_table_
#define _symbol_table_
#include "symbol_table_def.h"
symbol_table symbol_table_init();
void insert_symbol(symbol_table t, ast_symbol s);
void delete_symbol(symbol_table t, ast_symbol s);
ast_symbol find_symbol(symbol_table t,char *str);
extern symbol_table global_symbol_table;
#endif