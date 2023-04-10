#ifndef _intermediate_codegen_def_
#define _intermediate_codegen_def_

#include "symbol_table_def.h"

typedef struct QUADRUPLE* qNode ; 

// HASHMAP OF MODULE DEF => LABEL

typedef enum {
    ASSIGN_OP, 
    MUL_OP , 
    ADDN_OP,
    SUBT_OP,
    DIV_OP, 
    LT_OP ,
    EQ_OP , 
    NE_OP , 
    GT_OP , 
    GTE_OP,
    LTE_OP,
    FUN_DEF_OP,

} OPERATOR_LIST; 

struct QUADRUPLE {
    OPERATOR_LIST op ; 
    char* arg1 , arg2 , result ; 
    parseTreeNode arg1Node , arg2Node , resultNode;
    ast_symbol arg1symbol, arg2symbol , resultsymbol;
    qNode next ; 
}; 



#endif

// archaj god