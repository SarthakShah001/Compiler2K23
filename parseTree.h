#ifndef _parseTree_
#define _parseTree_

#include "parserDef.h"
#include "doublyLinkedList.h"


typedef struct PARSETREENODE* parseTreeNode ;

struct PARSETREENODE{
    parseTreeNode parent , sibling , prevSibling ;

    // childrenList is an array containing all first child node of the parent
    // X -> ABC
    // X -> DEF 

    // X->childrenList = [A->D] 
    TOK tok;
    parseTreeNode child;
    symbol s ;
}; 


// functions for the parse tree 
// create tree 
// add rule

parseTreeNode createTree() ; 

void addRuleInTree(parseTreeNode treeNode , dlinkedlist node) ; 


// outfile will add later 
void printParseTree(parseTreeNode treeNode) ; 
void addTokenTonode(parseTreeNode treeNode,Token currtoken);
#endif

