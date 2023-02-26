#ifndef _setADT_
#define _setADT_

#include <stdbool.h>
#include "parserDef.h"

// typedef struct setnode* SetNode;

// typedef struct SetNode{
//     bool arr[num_terminals];
// }setnode; 

// typedef struct {
//     SetNode s;
//     nonterminal nt;
// }set;

bool first_set[num_nonterminals][num_terminals];
bool follow_set[num_nonterminals][num_terminals];

void initSet(bool set[]) ; 
void insertElement(bool set[],terminal t) ; 
void deleteElement(bool set[], terminal t) ;
bool findInSet(bool s[], terminal t) ;
void setUnion(bool s1[], bool s2[], bool dest[]); 
void setIntersection(bool s1[], bool s2[], bool dest[]); 

#endif