#ifndef _intermediateCodeGen_
#define _intermediateCodeGen_

#include "intermediateCodeGenDef.h"
#include "symbol_table.h"
#include "parseTree.h"

// new label func
// emit function 
// 
void printQuadruple(qNode q);
qNode createQuadruple(); 
qNode generateIR(qNode curr ,parseTreeNode root, symbol_table st, int childcount) ; 
char* addTemporary(); 
char* newLabel();



#endif