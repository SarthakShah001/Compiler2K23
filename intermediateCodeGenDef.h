#ifndef _intermediate_codegen_def_
#define _intermediate_codegen_def_

#include "symbol_table_def.h"

typedef struct QUADRUPLE* qNode ; 

typedef enum {
    NULL_OP , INPUT_OP ,MOD_DEFN_OP
} OPERATOR_LIST; 

struct QUADRUPLE {
    OPERATOR_LIST op ; 
    char* arg1 , arg2 , result ; 
    parseTreeNode arg1Node , arg2Node , resultNode;
    ast_symbol arg1symbol, arg2symbol,resultsymbol;
    qNode next ; 
}; 





#endif