#ifndef _setADT_
#define _setADT_
#include <stdbool.h>
#include "parserDef.h"

typedef struct SETNODE* setNode;

struct SETNODE{
    bool arr[num_terminals];
    bool is_filled;
}; 



// typedef struct {
//     SetNode s;
//     nonterminal nt;
// }set;

extern setNode first_set[num_nonterminals];
extern setNode follow_set[num_nonterminals];

// initialising Set
setNode initSet() ; 

// insert terminal in set
void insertElement(setNode set,terminal t) ;

// deleting element 
void deleteElement(setNode set, terminal t) ;
bool findInSet(setNode s, terminal t) ;
void setUnion(setNode s1, setNode s2, setNode dest); 
void setIntersection(setNode s1, setNode s2, setNode dest);
bool isSuperset(setNode s1,setNode s2);

#endif