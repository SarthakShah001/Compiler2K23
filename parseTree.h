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
AST_ACTUAL_PARA,
AST_UNARYEXPR,
AST_RELATIONAL_OP,
AST_LOGICAL_OP,
AST_PLUS,
AST_MINUS,
AST_MUL,
AST_DIV,
AST_ARRAY_FACTOR,
AST_UNARY_INDEX_EXPR,
AST_DECLARE_STMT,
AST_SWITCH,
AST_CASES,
AST_CASE,
AST_FORLOOP,
AST_WHILELOOP,
AST_INDEX_FOR_LOOP,
AST_DEFAULT,
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
    int scope[2];
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
void freeParseTree(parseTreeNode treeNode) ;

#endif

