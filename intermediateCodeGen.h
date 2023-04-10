#ifndef _intermediateCodeGen_
#define _intermediateCodeGen_

#include "intermediateCodeGenDef.h"
#include "symbol_table.h"
#include "parseTree.h"

// new label func
// emit function 
// 

qNode createQuadruple() ; 
void generateIR(qNode curr ,parseTreeNode root, char* modname) ; 
void addTemporary() ; 
void newLabel() ; 


#endif