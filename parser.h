#ifndef _parser_
#define _parser_
#include "parserDef.h"
#include "hashTable.h"
hashtable terminals,nonterminals;
void fill_terminals(hashtable terminals);
void fill_nonterminals(hashtable nonterminals);
void fill_grammer();
symbol get_next_symbol(char *str);

#endif