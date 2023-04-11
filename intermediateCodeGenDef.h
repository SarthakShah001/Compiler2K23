/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _intermediate_codegen_def_
#define _intermediate_codegen_def_

#include "symbol_table_def.h"

typedef struct QUADRUPLE* qNode ; 

// HASHMAP OF MODULE DEF => LABEL

typedef enum {
    ASSIGN_OP, 
    MUL_OP , 
    PLUS_OP,
    MINUS_OP,
    DIV_OP, 
    LT_OP ,
    EQ_OP , 
    NE_OP , 
    GT_OP , 
    GE_OP,
    LE_OP,
    OR_OP,
    AND_OP,
    GETVAL_OP,
    PRINTVAL_OP,
    LABEL_OP,
    NULL_OP

} OPERATOR_LIST; 

struct labelNode  {
    char* labelName ; 
    char* moduleName; 
} ; 

struct QUADRUPLE {
    OPERATOR_LIST op ; 
    char arg1[25] , arg2[25] , result[25] ; 
    parseTreeNode arg1Node , arg2Node , resultNode;
    ast_symbol arg1symbol, arg2symbol , resultsymbol;
    symbol_table arg1symboltable,arg2symboltable, resultsymboltable;

    qNode next ; 
    qNode prev;
    parseTreeNode label;
    
}; 



#endif