#include "intermediateCodeGen.h"

char *currModuleName;
int childcount = 0;

int tempNum = 0;
int labelNum = 0;
char *addTemporary()
{
    tempNum++;
    char *str = malloc(20 * sizeof(char));
    sprintf(str, "Temp_%d", tempNum);

    return str;
}
char* newLabel(){
    labelNum++;
    char *str = malloc(20 * sizeof(char));
    sprintf(str, "Label_%d", labelNum);

    return str;
}

qNode createQuadruple()
{
    qNode newQnode = (qNode)malloc(sizeof(struct QUADRUPLE));
    newQnode->op = NULL_OP;
    newQnode->arg1 = NULL;
    newQnode->arg2 = NULL;
    newQnode->result = NULL;
    newQnode->arg1Node = NULL;
    newQnode->arg2Node = NULL;
    newQnode->result = NULL;
    newQnode->arg1symbol = NULL;
    newQnode->arg2symbol = NULL;
    newQnode->resultsymbol = NULL;
    newQnode->next = NULL;
    return newQnode;
}

qNode generateIR(qNode curr, parseTreeNode root, char *modname)
{

    if (root == NULL)
    {
        return;
    }
    else if (root->s->is_terminal)
    {
        // if(root->tok->token_type == TK_ID){
        //     qNode temp = createQuadruple();
        //     sprintf(temp->result,"%s",root->tok->lex.value);
        //     temp->resultNode = root;
        //     int x = find_mod_no(modname);
        //     temp->resultsymbol = find_symbol(global_symbol_table[x],temp->result);
        //     curr->next = temp;
        //     curr = curr->next;
        //     // return curr;
        // }
        // else if(root->tok->token_type == TK_INTEGER){

        // }
        // else if(root->tok->token_type == TK_MUL)

        return curr;
    }
    else if (root->tok == NULL && root->ast_name == AST_ID_ASSIGN)
    {
        qNode temp = createQuadruple();
        temp->op = ASSIGN_OP;
        temp->result = root->child->tok->lex.value;
        temp->resultNode = root->child;
        int x = find_mod_no(modname);
        temp->resultsymbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
        if (root->child->sibling->s == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling, modname);
            temp->arg1 = temp2->result;
            temp->arg1Node = temp2->resultNode;
            temp->arg1symbol = temp2->resultsymbol;
            curr = temp2;
        }
        else
        {
            if (root->child->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->sibling->tok->lex.integer);
                temp->arg1Node = root->child->sibling;
            }
            else if (root->child->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->sibling->tok->lex.decimal);
                temp->arg1Node = root->child->sibling;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->sibling->tok->lex.value);
                temp->arg1Node = root->child->sibling;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        // temp->prev = temp2;
        // while(temp->prev!=NULL){
        //     temp = temp->prev;
        // }
        curr->next = temp;
        curr = curr->next;
        // curr = temp2->next;
        // generateIR(curr,root->sibling,modname);
    }

    else if (root->tok == NULL && root->ast_name == AST_PLUS)
    {
        qNode temp = createQuadruple();
        temp->op = PLUS_OP;
        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, modname);
            temp->arg1 = temp1->result;
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, modname);
            temp->arg2 = temp2->result;
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary());
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
    }

    else if (root->tok == NULL && root->ast_name == AST_MINUS)
    {
        qNode temp = createQuadruple();
        temp->op = MINUS_OP;
        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, modname);
            temp->arg1 = temp1->result;
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, modname);
            temp->arg2 = temp2->result;
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary());
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
    }

    else if (root->tok == NULL && root->ast_name == AST_RELATIONAL_OP)
    {
        qNode temp = createQuadruple();
        tkType t1 = root->child->sibling->tok->token_type;
        switch (t1)
        {
        case TK_GT:
        {
            temp->op = GT_OP;
            break;
        }
        case TK_LT:
        {
            temp->op = LT_OP;
            break;
        }
        case TK_GE:
        {
            temp->op = GE_OP;
            break;
        }
        case TK_LE:
        {
            temp->op = LE_OP;
            break;
        }
        case TK_EQ:
        {
            temp->op = EQ_OP;
            break;
        }
        case TK_NE:
        {
            temp->op = NE_OP;
            break;
        }
        default:
        {
            break;
        }
        }

        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, modname);
            temp->arg1 = temp1->result;
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, modname);
            temp->arg2 = temp2->result;
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary());
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
    }

    else if (root->tok == NULL && root->ast_name == AST_LOGICAL_OP)
    {
        qNode temp = createQuadruple();
        tkType t1 = root->child->sibling->tok->token_type;
        switch (t1)
        {
        case TK_AND:
        {
            temp->op = AND_OP;
            break;
        }
        case TK_OR:
        {
            temp->op = OR_OP;
            break;
        }
        default:{
            break;
        }
        }
        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, modname);
            temp->arg1 = temp1->result;
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, modname);
            temp->arg2 = temp2->result;
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary());
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
    }
    else if (root->tok == NULL && root->ast_name == AST_MUL)
    {
        qNode temp = createQuadruple();
        temp->op = MUL_OP;
        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, modname);
            temp->arg1 = temp1->result;
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, modname);
            temp->arg2 = temp2->result;
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary());
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
    }

    else if (root->tok == NULL && root->ast_name == AST_DIV)
    {
        qNode temp = createQuadruple();
        temp->op = DIV_OP;
        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, modname);
            temp->arg1 = temp1->result;
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, modname);
            temp->arg2 = temp2->result;
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary());
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
    }
    else if (root->tok == NULL && root->ast_name ==AST_UNARYEXPR ){
        qNode temp = createQuadruple();
        if(root->child->tok->token_type == TK_PLUS){
            temp->arg1 = PLUS_OP;
        }
        else{
            temp->arg1 = MINUS_OP;
        }
        temp->arg1Node = root->child;
        temp->op = ASSIGN_OP;
        sprintf(temp->result, "%s", addTemporary());

    if (root->child->sibling->s == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling, modname);
            temp->arg2 = temp2->result;
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }
        else
        {
            if (root->child->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling;
            }
            else if (root->child->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling;
                int x = find_mod_no(modname);
                temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        // temp->prev = temp2;
        // while(temp->prev!=NULL){
        //     temp = temp->prev;
        // }
        curr->next = temp;
        curr = curr->next;
}

    else if (root->tok == NULL && root->ast_name == AST_GET_VALUE){

        // yaha getvalue wala quadruple generate hoga
        // assuming get_value will take valid value
        // array ka abhi sochna hai
        qNode temp = createQuadruple() ;
        temp->op = GETVAL_OP ;
        temp->arg1 = root->child->tok->lex.value;
        temp->arg1Node = root->child;
        int x = find_mod_no(modname);
        temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount],temp->arg1 );
        // temp->arg1symbol =
        curr->next = temp;
        curr = curr->next;
    }
    else if (root->tok == NULL && root->ast_name == AST_PRINT){

        // yaha getvalue wala quadruple generate hoga
        // assuming get_value will take valid value
        // array ka abhi sochna hai
        qNode temp = createQuadruple() ;
        temp->op = PRINTVAL_OP ;
        if (root->child->s != NULL){
        temp->arg1 = root->child->tok->lex.value;
        temp->arg1Node = root->child;
        int x = find_mod_no(modname);
        temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount],temp->arg1 );

        }
        else{
            qNode temp2 = generateIR(curr,root->child,modname);
            temp->arg1 = temp2->result;
            temp->arg1Node = temp2->resultNode;
            temp->arg1symbol = temp2->resultsymbol;
            curr = temp2;
        }
        // temp->arg1symbol =
        curr->next = temp;
        curr = curr->next;
    }
    else if(root->tok == NULL && root->ast_name == AST_ARRAY_ACCESS){
        qNode temp = createQuadruple();
        temp->op = ASSIGN_OP;
        temp->arg1 = root->child->tok->lex.value;
        temp->arg1Node = root->child;
        int x = find_mod_no(modname);
        temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount],temp->arg1 );
        qNode temp2 = generateIR(curr,root->child->sibling,modname);
        temp->arg2 = temp2->result;
        // temp->arg2Node = temp2->resultNode;
        curr = temp2;
        sprintf(temp->result,"%s",addTemporary());
        curr->next = temp;
        curr = curr->next;
        // int x = find_mod_no(modname);
        // temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount],temp->arg2 );

    }
    else if(root->tok == NULL && root->ast_name == AST_INDEX_ARR){
        qNode temp = createQuadruple;
        temp->op = ASSIGN_OP;
        sprintf(temp->result,"%s",addTemporary());

        if(root->child->sibling!=NULL){
            temp->arg1 = root->child->tok->lex.value;
            temp->arg1Node = root->child;
            if (root->child->sibling->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg2, "%d", root->child->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling;
            }
            else if (root->child->sibling->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg2);
            }
        }
        else{
                if (root->child->tok->token_type == TK_INTEGER)
            {
                sprintf(temp->arg1, "%d", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_REAL)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                int x = find_mod_no(modname);
                temp->arg2symbol = find_symbol(global_symbol_table[x]->table->child[childcount], temp->arg1);
            }
        }
        curr->next = temp;
        curr = curr->next;
    }


    else if(root->tok == NULL && root->ast_name == AST_MODULEDEFINITIONS){
        if(root->child!=NULL){
        qNode temp = createQuadruple();
        temp->op = LABEL_OP;
        temp->arg1 = root->child->tok->lex.value;
        modname = root->child->tok->lex.value;
        temp->arg1Node = root->child;
        temp->result = newLabel();
        curr->next = temp;
        curr = curr->next;
        }
        
        // yaha pe is module ke corresponding label ek jagah store kr dete hai
        // int x = find_mod_no(modname);
        // temp->arg1


    }
    else if(root->tok ==NULL && root->ast_name == AST_STATEMENTS){
        parseTreeNode temp = root->child;
        while(temp!=NULL){
           qNode temp2 =  generateIR(curr,temp,modname);
           curr = temp2;
           temp = temp->sibling;
        }
    }
    else if(root->tok == NULL && root->ast_name == AST_PROGRAM){

        parseTreeNode temp = root->child;
        while(temp!=NULL){
           qNode temp2 =  generateIR(curr,temp,modname);
           curr = temp2;
           temp = temp->sibling;
        }

    }
    else if(root->tok == NULL && root->ast_name == AST_DRIVER){

        modname = "driver";
        qNode temp2 = generateIR(curr,root->child,modname);

        curr = temp2;

    }


    else{

    }
}