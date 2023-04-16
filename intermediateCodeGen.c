/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include "intermediateCodeGen.h"

// char *currModuleName;
// int childcount = 0;

int tempNum = 0;
int labelNum = 0;
char *operator_string[] = {
    "=",
    "*",
    "+",
    "-",
    "/",
    "<",
    "==",
    "!=",
    ">",
    ">=",
    "<=",
    "OR",
    "AND",
    "GETVAL",
    "PRINTVAL",
    "LABEL",
    "driver_start",
    "NULL",
};
char *addTemporary(symbol_table t, tkType type)
{
    int max_offset=-1;
    int width_offset=0;
    for(int i=0;i<1000;i++){
        if(t->sym[i]!=NULL){
            if(t->sym[i]->offset>max_offset){
            max_offset=t->sym[i]->offset;
            width_offset=t->sym[i]->width;
            }
        }
    }
    ast_symbol temp=ast_symbol_init();
    tempNum++;
    char *str = malloc(20 * sizeof(char));
    sprintf(str, "Temp_%d", tempNum);
    strcpy(temp->var_name,str);
    temp->offset=max_offset+width_offset;
    // printf("%s----%d\n",temp->var_name,type);
    temp->width= 2;
    insert_symbol_table(t,temp);
    return str;
}
char *newLabel()
{
    labelNum++;
    char *str = malloc(20 * sizeof(char));
    sprintf(str, "Label_%d", labelNum);

    return str;
}

qNode createQuadruple()
{
    qNode newQnode = (qNode)malloc(sizeof(struct QUADRUPLE));
    newQnode->op = NULL_OP;
    strcpy(newQnode->arg1, "");
    strcpy(newQnode->arg2, "");
    strcpy(newQnode->result, "");
    newQnode->arg1Node = NULL;
    newQnode->arg2Node = NULL;
    newQnode->resultNode = NULL;
    newQnode->arg1symbol = NULL;
    newQnode->arg2symbol = NULL;
    newQnode->resultsymbol = NULL;
    newQnode->next = NULL;
    return newQnode;
}
void printQuadruple(qNode q)
{
    if (q == NULL)
    {
        return;
    }
    printf("%-20s", operator_string[q->op]);
    if (q->arg1 == NULL)
    {
        printf("  NULL  ");
    }
    else
    {
        printf("%-20s", q->arg1);
    }
    if (q->arg2 == NULL)
    {
        printf("  NULL  ");
    }
    else
    {
        printf("%-20s", q->arg2);
    }
    if (q->result == NULL)
    {
        printf("  NULL  ");
    }
    else
    {
        printf("%-20s", q->result);
    }
    printf("\n");
    printQuadruple(q->next);
}

qNode generateIR(qNode curr, parseTreeNode root, symbol_table st, int childcount)
{
    if (root == NULL)
    {
        return curr;
    }
    // printf("Currently in %s \n", ast_strings[root->ast_name]);
    // parseTreeNode currchild = root->child;
    switch (root->ast_name)
    {

    case AST_PROGRAM:
    {
        parseTreeNode temp = root->child;
        while (temp != NULL)
        {
            qNode temp2 = generateIR(curr, temp, st, childcount);
            curr = temp2;
            temp = temp->sibling;
        }
        break;
    }
    case AST_MODULEDECLARATIONS:
    {
        // do nothing
        break;
    }
    case AST_MODULEDEFINITIONS:
    {
        parseTreeNode temp = root->child;
        while (temp != NULL)
        {
            qNode temp2 = generateIR(curr, temp, st, childcount);
            curr = temp2;
            temp = temp->sibling;
        }

        break;
    }
    case AST_DRIVER:
    {
        // currModuleName = "driver";
         qNode temp = createQuadruple();
        temp->op = DRIVERDEFN_OP;
        strcpy(temp->arg1, "driver");
        curr->next = temp;
        curr = curr->next;
        int x = find_mod_no("driver");
        childcount = 0;
        qNode temp2 = generateIR(curr, root->child, global_symbol_table[x]->table, childcount);
        // currModuleName = "driver";
        curr = temp2;
        break;
    }
    case AST_MODULE:
    {
        int x = find_mod_no(root->child->tok->lex.value);
        st = global_symbol_table[x]->table;
        childcount = 0;
        // currModuleName = root->child->tok->lex.value;
        qNode temp = createQuadruple();
        temp->op = LABEL_OP;
        strcpy(temp->arg1, root->child->tok->lex.value);
        temp->arg1Node = root->child;
        sprintf(temp->result, "%s", newLabel());
        curr->next = temp;
        curr = curr->next;
        qNode temp2 = generateIR(curr, root->child->sibling->sibling->sibling, st, childcount);
        curr = temp2; // ab yaha
        break;
    }
    case AST_INPUT_PARAMETER_LIST:
    {

        break;
    }
    case AST_OUTPUT_PARAMETER_LIST:
    {

        break;
    }
    case AST_INP_PARAMETER:
    {

        break;
    }
    case AST_OUT_PARAMETER:
    {

        break;
    }
    case AST_ARRAY:
    {

        break;
    }
    case AST_RANGE_ARRAYS:
    {

        break;
    }
    case AST_STATEMENTS:
    {
        parseTreeNode temp = root->child;
        while (temp != NULL)
        {
            qNode temp2 = generateIR(curr, temp, st, childcount);
            curr = temp2;
            temp = temp->sibling;
        }
        break;
    }
    case AST_GET_VALUE:
    {

        // yaha getvalue wala quadruple generate hoga
        // assuming get_value will take valid value
        // array ka abhi sochna hai
        qNode temp = createQuadruple();
        temp->op = GETVAL_OP;
        strcpy(temp->arg1, root->child->tok->lex.value);
        temp->arg1Node = root->child;
        // int x = find_mod_no(currModuleName);
        // if (childcount == 0)
        // {
        //     temp->arg1symbol = find_symbol(st, temp->arg1);
        // }
        // else
        // {
        //     temp->arg1symbol = find_symbol(st->child[childcount - 1], temp->arg1);
        // }
        temp->arg1symbol = find_symbol(st, temp->arg1);
        // temp->arg1symbol =
        curr->next = temp;
        curr = curr->next;

        break;
    }
    case AST_PRINT:
    {
        // yaha getvalue wala quadruple generate hoga
        // assuming get_value will take valid value
        // array ka abhi sochna hai
        qNode temp = createQuadruple();
        temp->op = PRINTVAL_OP;
        if (root->child->s != NULL)
        {
            strcpy(temp->arg1, root->child->tok->lex.value);
            temp->arg1Node = root->child;
            // if (childcount == 0)
            // {
            //     temp->arg1symbol = find_symbol(st, temp->arg1);
            // }
            // else
            // {
            //     temp->arg1symbol = find_symbol(st->child[childcount - 1], temp->arg1);
            // }
            temp->arg1symbol = find_symbol(st, temp->arg1);
        }
        else
        {
            qNode temp2 = generateIR(curr, root->child, st, childcount);
            strcpy(temp->arg1, temp2->result);
            temp->arg1Node = temp2->resultNode;
            temp->arg1symbol = temp2->resultsymbol;
            curr = temp2;
        }
        // temp->arg1symbol =
        curr->next = temp;
        curr = curr->next;
        break;
    }
    case AST_ARRAY_ACCESS:
    {

        qNode temp = createQuadruple();
        temp->op = ASSIGN_OP;
        strcpy(temp->arg1, root->child->tok->lex.value);
        temp->arg1Node = root->child;
        // if(childcount == 0){
        // temp->arg1symbol = find_symbol(st, temp->arg1);
        // }
        // else{
        //     temp->arg1symbol = find_symbol(st->child[childcount-1],temp->arg1);
        // }
        temp->arg1symbol = find_symbol(st, temp->arg1);
        qNode temp2 = generateIR(curr, root->child->sibling, st, childcount);
        strcpy(temp->arg2, temp2->result);
        // temp->arg2Node = temp2->resultNode;
        curr = temp2;
        sprintf(temp->result, "%s", addTemporary(st, temp->arg1symbol->type));
        
        curr->next = temp;
        curr = curr->next;

        break;
    }
    case AST_ID_ASSIGN:
    {
        qNode temp = createQuadruple();
        temp->op = ASSIGN_OP;
        strcpy(temp->result, root->child->tok->lex.value);
        temp->resultNode = root->child;

        // {
        // if (childcount == 0)
        // {
        //     temp->resultsymbol = find_symbol(st, temp->result);
        // }
        // else
        // {

        //     temp->resultsymbol = find_symbol(st->child[childcount - 1], temp->result);
        // }
        temp->resultsymbol = find_symbol(st, temp->result);
        // }
        // else
        // {
        //     int x = find_mod_no(currModuleName);
        //     st = global_symbol_table[x]->table;
        //     childcount = 0;
        // }

        if (root->child->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling, st, childcount);
            strcpy(temp->arg1, temp2->result);
            temp->arg1Node = temp2->resultNode;
            temp->arg1symbol = temp2->resultsymbol;
            curr = temp2;
        }
        else
        {
            if (root->child->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lld", root->child->sibling->tok->lex.integer);
                temp->arg1Node = root->child->sibling;
            }
            else if (root->child->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lf", root->child->sibling->tok->lex.decimal);
                temp->arg1Node = root->child->sibling;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->sibling->tok->lex.value);
                temp->arg1Node = root->child->sibling;
                // if (childcount != 0)
                // {
                //     temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount - 1], temp->arg1);
                // }
                // else
                // {
                //     temp->arg1symbol = find_symbol(global_symbol_table[x]->table, temp->arg1);
                // }
                temp->arg1symbol = find_symbol(st, temp->arg1);
            }
        }
        // // temp->prev = temp2;
        // // while(temp->prev!=NULL){
        // //     temp = temp->prev;
        // // }
        curr->next = temp;
        curr = curr->next;
        // // curr = temp2->next;
        // // generateIR(curr,root->sibling,modname);

        break;
    }
    case AST_ARRAY_ASSIGN:
    {

        break;
    }
    case AST_INDEX_ARR:
    {
        qNode temp = createQuadruple();
        temp->op = ASSIGN_OP;
        if (root->child->sibling != NULL)
        {
            strcpy(temp->arg1, root->child->tok->lex.value);
            temp->arg1Node = root->child;
            if (root->child->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_INTEGER));
                sprintf(temp->arg2, "%lld", root->child->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling;
            }
            else if (root->child->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_REAL));
                sprintf(temp->arg2, "%lf", root->child->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
                sprintf(temp->result, "%s", addTemporary(st, temp->arg2symbol->type));
            }
        }
        else
        {
            if (root->child->tok->token_type == TK_NUM)
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_INTEGER));
                sprintf(temp->arg1, "%lld", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_RNUM)
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_REAL));
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                temp->arg1symbol = find_symbol(st, temp->arg1);
                sprintf(temp->result, "%s", addTemporary(st, temp->arg1symbol->type));
                
            }
        }
        temp->resultsymbol = find_symbol(st,temp->result);
        curr->next = temp;
        curr = curr->next;
        break;
    }
    case AST_MODULE_REUSE:
    {

        break;
    }
    case AST_PARAMETER_LIST1:
    {

        break;
    }
    case AST_PARAMETER_LIST2:
    {
        break;
    }
    case AST_ACTUAL_PARA:
    {

        break;
    }
    case AST_UNARYEXPR:
    {

        qNode temp = createQuadruple();

        strcpy(temp->arg1, root->child->tok->lex.value);

        temp->arg1Node = root->child;
        temp->op = ASSIGN_OP;

        if (root->child->sibling->s == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            sprintf(temp->result, "%s", addTemporary(st, temp->arg2symbol->type));
            curr = temp2;
        }
        else
        {
            if (root->child->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling;
            }
            else if (root->child->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
            sprintf(temp->result,"%s",addTemporary(st,temp->arg2Node->tok->token_type));
        }
        // temp->prev = temp2;
        // while(temp->prev!=NULL){
        //     temp = temp->prev;
        // }
        temp->resultsymbol = find_symbol(st,temp->result);
        curr->next = temp;
        curr = curr->next;

        break;
    }
    case AST_RELATIONAL_OP:
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
            if (root->child->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lld", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                temp->arg1symbol = find_symbol(st, temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, st, childcount);
            strcpy(temp->arg1, temp1->result);
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
        }

        sprintf(temp->result, "%s", addTemporary(st, TK_BOOLEAN));
        temp->resultsymbol = find_symbol(st,temp->result);
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);

        break;
    }
    case AST_LOGICAL_OP:
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
        default:
        {
            break;
        }
        }
        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lld", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                temp->arg1symbol = find_symbol(st, temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, st, childcount);
            strcpy(temp->arg1, temp1->result);
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary(st, TK_BOOLEAN));
        temp->resultsymbol = find_symbol(st,temp->result);
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);

        break;
    }
    case AST_PLUS:
    {
        qNode temp = createQuadruple();
        temp->op = PLUS_OP;
        if (root->child->tok != NULL)
        {
            // printf("i am child terminal of plus\n");
            if (root->child->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lld", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                // printf("%s\n", root->child->tok->lex.value);

                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;

                // // // int x = find_mod_no(currModuleName);
                // // // if (childcount != 0)
                // // // {
                // // //     temp->arg1symbol = find_symbol(global_symbol_table[x]->table->child[childcount - 1], temp->arg1);
                // // // }
                // // // else
                // // // {
                // // //     temp->arg1symbol = find_symbol(global_symbol_table[x]->table, temp->arg1);
                // // // }
                temp->arg1symbol = find_symbol(st, temp->arg1);
            }
        }
        else
        {
            // printf("i am child astnode of plus\n");
            qNode temp1 = generateIR(curr, root->child, st, childcount);
            strcpy(temp->arg1, temp1->result);
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            // printf("i am 3rd child ast of plus\n");

            qNode temp2 = generateIR(curr, root->child->sibling->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            // printf("i am 3rd child terminal of plus\n");
            if (root->child->sibling->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
        }
        if (temp->arg1symbol == NULL)
        {
            if (temp->arg1Node->tok->token_type == TK_NUM)
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_INTEGER));
                
            }
            else
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_REAL));
            }
        }
        else
        {
            sprintf(temp->result, "%s", addTemporary(st, temp->arg1symbol->type));
            
        }
        temp->resultsymbol = find_symbol(st,temp->result);
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);

        break;
    }
    case AST_MINUS:
    {
        qNode temp = createQuadruple();
        temp->op = MINUS_OP;
        if (root->child->tok != NULL)
        {
            // printf("i am child terminal of plus\n");
            if (root->child->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lld", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                temp->arg1symbol = find_symbol(st, temp->arg1);
            }
        }
        else
        {
            // printf("i am child astnode of plus\n");
            qNode temp1 = generateIR(curr, root->child, st, childcount);
            strcpy(temp->arg1, temp1->result);
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            //  printf("i am 3rd child ast of plus\n");
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            // printf("i am 3rd child terminal of plus\n");
            if (root->child->sibling->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
        }
        // printf("I am done\n");
if (temp->arg1symbol == NULL)
        {
            if (temp->arg1Node->tok->token_type == TK_NUM)
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_INTEGER));
            }
            else
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_REAL));
            }
        }
        else
        {
            sprintf(temp->result, "%s", addTemporary(st, temp->arg1symbol->type));
        }    
        temp->resultsymbol = find_symbol(st,temp->result);
        
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
        break;
    }
    case AST_MUL:
    {
        qNode temp = createQuadruple();
        temp->op = MUL_OP;

        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lld", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                temp->arg1symbol = find_symbol(st, temp->arg1);
            }
        }
        else
        {
           
            qNode temp1 = generateIR(curr, root->child, st, childcount);
            strcpy(temp->arg1, temp1->result);
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;

            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
        }
        if (temp->arg1symbol == NULL)
        {
            if (temp->arg1Node->tok->token_type == TK_NUM)
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_INTEGER));
            }
            else
            {
                sprintf(temp->result, "%s", addTemporary(st, TK_REAL));
            }
        }
        else
        {
            sprintf(temp->result, "%s", addTemporary(st, temp->arg1symbol->type));
            
        }
        temp->resultsymbol = find_symbol(st,temp->result);
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
        //  do nothing
        break;
    }
    case AST_DIV:
    {
        qNode temp = createQuadruple();
        temp->op = DIV_OP;
        if (root->child->tok != NULL)
        {
            if (root->child->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg1, "%lld", root->child->tok->lex.integer);
                temp->arg1Node = root->child;
            }
            else if (root->child->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg1, "%lf", root->child->tok->lex.decimal);
                temp->arg1Node = root->child;
            }
            else
            {
                sprintf(temp->arg1, "%s", root->child->tok->lex.value);
                temp->arg1Node = root->child;
                temp->arg1symbol = find_symbol(st, temp->arg1);
            }
        }
        else
        {
            qNode temp1 = generateIR(curr, root->child, st, childcount);
            strcpy(temp->arg1, temp1->result);
            temp->arg1Node = temp1->resultNode;
            temp->arg1symbol = temp1->resultsymbol;
            curr = temp1;
        }
        if (root->child->sibling->sibling->tok == NULL)
        {
            qNode temp2 = generateIR(curr, root->child->sibling->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            curr = temp2;
        }

        else
        {
            if (root->child->sibling->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else if (root->child->sibling->sibling->tok->token_type == TK_RNUM)
            {
                sprintf(temp->arg2, "%lf", root->child->sibling->sibling->tok->lex.decimal);
                temp->arg2Node = root->child->sibling->sibling;
            }
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
        }
        sprintf(temp->result, "%s", addTemporary(st,TK_REAL));
        temp->resultsymbol = find_symbol(st,temp->result);
        
        curr->next = temp;
        curr = curr->next;
        // generateIR(curr,root->sibling,modname);
        break;
    }
    case AST_ARRAY_FACTOR:
    {
        qNode temp = createQuadruple();
        temp->op = ASSIGN_OP;
        strcpy(temp->arg1,root->child->tok->lex.value);
        temp->arg1Node = root->child;
        temp->arg1symbol = find_symbol(st,temp->arg1);
        if(root->child->sibling->tok==NULL){
                qNode temp2 = generateIR(curr, root->child->sibling, st, childcount);
            strcpy(temp->arg2, temp2->result);
            temp->arg2Node = temp2->resultNode;
            temp->arg2symbol = temp2->resultsymbol;
            
            curr = temp2;
        }
        else{
                if (root->child->sibling->tok->token_type == TK_NUM)
            {
                sprintf(temp->arg2, "%lld", root->child->sibling->tok->lex.integer);
                temp->arg2Node = root->child->sibling;
            }
            
            else
            {
                sprintf(temp->arg2, "%s", root->child->sibling->tok->lex.value);
                temp->arg2Node = root->child->sibling;
                temp->arg2symbol = find_symbol(st, temp->arg2);
            }
        
        }

        
            sprintf(temp->result, "%s", addTemporary(st, temp->arg1symbol->type));
            temp->resultsymbol = find_symbol(st,temp->result);
            
            curr->next = temp;
            curr = curr->next;
    
        break;
    }
    case AST_RANGE_FOR_LOOP:
    {

        break;
    }
    case AST_DECLARE_STMT:
    {

        break;
    }
    case AST_SWITCH:
    {

        break;
    }
    case AST_CASES:
    {

        break;
    }
    case AST_CASE:
    {

        break;
    }
    case AST_FORLOOP:
    {

        break;
    }
    case AST_WHILELOOP:
    {

        break;
    }
    case AST_INDEX_FOR_LOOP:
    {

        break;
    }
    case AST_DEFAULT:
    {

        break;
    }
    default:
    {
        break;
    }
    }
    return curr;
}