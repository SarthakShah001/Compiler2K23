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
#include "astdef.h"

typedef enum {
AST_PROGRAM,
AST_MODULEDECLARATIONS,
AST_MODULEDEFINITIONS
}astToken;

typedef struct PARSETREENODE* parseTreeNode ;

struct PARSETREENODE{
    parseTreeNode parent , sibling , prevSibling ;
    TOK tok;
    parseTreeNode child;
    symbol s ;
    parseTreeNode syn_node;
    parseTreeNode inh_node;
    astToken ast_name;
    bool is_visited;
}; 


// functions for the parse tree 
// create tree 
// add rule

parseTreeNode createTree() ; 
// add rules from grammar
void addRuleInTree(parseTreeNode treeNode , dlinkedlist node) ; 
void openparsetreefile(FILE *f);

// prints parse tree into the file open from previous function
void printParseTree(parseTreeNode treeNode) ; 
// add tokens to terminals in parsetree (logic implemented in parse tree)
void addTokenTonode(parseTreeNode treeNode,Token currtoken);

#endif

