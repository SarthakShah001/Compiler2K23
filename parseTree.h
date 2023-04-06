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
AST_MODULEDEFINITIONS,
AST_DRIVER,
AST_MODULE,
AST_INPUT_PARAMETER_LIST,
AST_OUTPUT_PARAMETER_LIST,
AST_INP_PARAMETER,
AST_OUT_PARAMETER,
AST_ARRAY,
AST_RANGE_ARRAYS,
AST_STATEMENTS,
AST_GET_VALUE,
AST_PRINT,
AST_ARRAY_ACCESS,
AST_ID_ASSIGN,
AST_ARRAY_ASSIGN,
AST_INDEX_ARR,
AST_MODULE_REUSE,
AST_PARAMETER_LIST1,
AST_PARAMETER_LIST2,
AST_ACTUAL_PARA
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
void printAST(parseTreeNode treeNode) ; 

#endif

