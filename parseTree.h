/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

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
void openparsetreefile(FILE *f);

// outfile will add later 
void printParseTree(parseTreeNode treeNode) ; 
void addTokenTonode(parseTreeNode treeNode,Token currtoken);
#endif

