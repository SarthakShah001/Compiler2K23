#include "intermediateCodeGen.h"

char* currModuleName ; 

qNode createQuadruple(){
    qNode newQnode = (qNode)malloc(sizeof(struct QUADRUPLE)) ; 
    newQnode->op = NULL_OP;
    newQnode->arg1 = NULL;
    newQnode->arg2 = NULL;
    newQnode->result = NULL;
    newQnode->arg1Node = NULL;
    newQnode->arg2Node = NULL;
    newQnode->result = NULL;
    newQnode->arg1symbol=NULL;
    newQnode->arg2symbol = NULL;
    newQnode->resultsymbol= NULL;
    return newQnode;    
}



void generateIR(qNode curr, parseTreeNode root, char* modname){
    
    if(root == NULL){
        return;
        
    }

    if (root->tok == NULL && root->ast_name == AST_GET_VALUE){

        // yaha getvalue wala quadruple generate hoga 
        // assuming get_value will take valid value 
        qNode temp = createQuadruple() ; 
        temp->op = INPUT_OP ;
        temp->arg1 = root->child->tok->lex.value;
        temp->arg1Node = root->child;
        int x = find_mod_no(modname);
        temp->arg1symbol = find_symbol(global_symbol_table[x],temp->arg1 );
        // temp->arg1symbol = 
        curr->next = temp;
        generateIR(curr->next, root->sibling,modname);
    }
    if(root->tok == NULL && root->ast_name == AST_MODULEDEFINITIONS){
        genertateIR(curr,root->child,modname);
    }
    if(root->tok == NULL && root->ast_name == AST_MODULE){
        currModuleName = root->child->tok->lex.value;
        qNode temp = createQuadruple() ;
        temp->op = MOD_DEFN_OP;
        temp->arg1 = root->child->tok->lex.value;
        temp->arg1Node = root->child;
        
    }
}
