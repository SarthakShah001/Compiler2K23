#ifndef _parser_
#define _parser_

#include "parserDef.h"
#include "hashTable.h"
#include "doublyLinkedList.h"

void fill_terminals(hashtable terminals);
void fill_nonterminals(hashtable nonterminals);
void fill_grammer();
void fill_hash_tables();
void populateFirstSet(nonterminal nt1);
void populateFollowSet(nonterminal nt1);
extern dllnode parse_table[num_nonterminals][num_terminals];
void fill_parse_table();

#endif