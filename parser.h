/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _parser_
#define _parser_

#include "parserDef.h"
#include "doublyLinkedList.h"
#include "stackADT.h"


parseTreeNode startParser(FILE *fp,int size); 
void fill_terminals(hashtable terminals);
void fill_nonterminals(hashtable nonterminals);
void fill_grammer();
void fill_hash_tables();
void populateFirstSet(nonterminal nt1);
void populateFollowSet(nonterminal nt1);
extern dlinkedlist parse_table[num_nonterminals][num_terminals];
void fill_parse_table();

#endif