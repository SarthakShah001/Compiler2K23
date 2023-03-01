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

// starts parsing 
parseTreeNode startParser(FILE *fp,int size); 
// creates hashtable for terminals and fill terminals in it
void fill_terminals(hashtable terminals);
// same as terminals
void fill_nonterminals(hashtable nonterminals);
// fills grammer in the matrix of doubly linked list
void fill_grammer();
// fills both terminals and non terminals function
void fill_hash_tables();
// populates the first set from grammer
void populateFirstSet(nonterminal nt1);

// populates follow set from grammar
void populateFollowSet(nonterminal nt1);
// extern as used in parsetree.c
extern dlinkedlist parse_table[num_nonterminals][num_terminals];
// fills parse table from first and follow sets and grammar
void fill_parse_table();

#endif