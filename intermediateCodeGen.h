/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _intermediateCodeGen_
#define _intermediateCodeGen_

#include "intermediateCodeGenDef.h"
#include "symbol_table.h"
#include "parseTree.h"

// new label func
// emit function 
// 
extern char *operator_string[];
void printQuadruple(qNode q);
qNode createQuadruple(); 
qNode generateIR(qNode curr ,parseTreeNode root, symbol_table st, int childcount) ; 
char* addTemporary(); 
char* newLabel();



#endif