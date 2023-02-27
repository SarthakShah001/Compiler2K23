#ifndef _parseTree_
#define _parseTree_

#include "parserDef.h"
#include "doublyLinkedList.h"


typedef struct PARSETREENODE* parseTreeNode ;

struct PARSETREENODE{
    parseTreeNode parent , sibling , prevSibling ;

    // childrenList is a linked list containing all first child node of the parent
    // X -> ABC
    // X -> DEF 

    // X->childrenList = [A->D]
    int childrenCount ; 
    parseTreeNode childrenList[20] ;
    symbol s ;
}; 


// functions for the parse tree 
// create tree 
// add rule

parseTreeNode createTree() ; 

void addRuleInTree(parseTreeNode treeNode , dlinkedlist node) ; 
#endif

