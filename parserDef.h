/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _parserdef_
#define _parserdef_
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
typedef struct SYMBOL* symbol;
#include "hashTable.h"
#include "lexer.h"
#include "doublyLinkedList.h"
#define terminal tkType
#define num_rules 138
#define max_rule_length 200
typedef enum NONTERMINAL{
program,
moduleDeclarations,
moduleDeclaration,
otherModules,
driverModule,
module,
ret,
input_plist,
N1,
output_plist,
N2,
dataType,
range_arrays,
type,
moduleDef,
statements,
ioStmt,
boolConstt,
var_print,
P1,
simpleStmt,
assignmentStmt,
whichStmt,
lvalueIDStmt,
lvalueARRStmt,
index_arr,
new_index,
sign,
moduleReuseStmt,
actual_para_list,
N20,
optional,
idList,
N3,
expression,
U,
new_NT,
var_id_num,
unary_op,
arithmeticOrBooleanExpr,
N7,
AnyTerm,
N8,
arithmeticExpr,
N4,
term,
N5,
factor,
N11,
element_index_with_expressions,
arrExpr,
arr_N4,
arrTerm,
arr_N5,
arrFactor,
N10,
op1,
op2,
logicalOp,
relationalOp,
declareStmt,
condionalStmt,
caseStmts,
N9,
value,
DEFAULT,//default not working
iterativeStmt,
range_for_loop,
index_for_loop,
new_index_for_loop,
sign_for_loop
} nonterminal;

extern char *nonterminal_str[];
extern hashtable terminals,nonterminals;

struct SYMBOL{
    union
    {
        terminal t;
        nonterminal nt;
    };
    bool is_terminal;
};

typedef struct RULES{
    nonterminal LHS;
    dlinkedlist RHS;
}rule;

extern rule grep[num_rules];
#endif