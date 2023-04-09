#include "ast.h"

char *ast_strings[] = {
    "AST_PROGRAM",
    "AST_MODULEDECLARATIONS",
    "AST_MODULEDEFINITIONS",
    "AST_DRIVER",
    "AST_MODULE",
    "AST_INPUT_PARAMETER_LIST",
    "AST_OUTPUT_PARAMETER_LIST",
    "AST_INP_PARAMETER",
    "AST_OUT_PARAMETER",
    "AST_ARRAY",
    "AST_RANGE_ARRAYS",
    "AST_STATEMENTS",
    "AST_GET_VALUE",
    "AST_PRINT",
    "AST_ARRAY_ACCESS",
    "AST_ID_ASSIGN",
    "AST_ARRAY_ASSIGN",
    "AST_INDEX_ARR",
    "AST_MODULE_REUSE",
    "AST_PARAMETER_LIST1",
    "AST_PARAMETER_LIST2",
    "AST_ACTUAL_PARA",
    "AST_UNARYEXPR",
    "AST_RELATIONAL_OP",
    "AST_LOGICAL_OP",
    "AST_PLUS",
    "AST_MINUS",
    "AST_MUL",
    "AST_DIV",
    "AST_ARRAY_FACTOR",
    "AST_UNARY_INDEX_EXPR",
    "AST_DECLARE_STMT",
    "AST_SWITCH",
    "AST_CASES",
    "AST_CASE",
    "AST_FORLOOP",
    "AST_WHILELOOP",
    "AST_INDEX_FOR_LOOP",
    "AST_DEFAULT",
};


bool is_important_terminal(parseTreeNode node)
{
    bool flag = true;

    switch (node->s->t)
    {
        case TK_ID:
        case TK_NUM:
        case TK_RNUM:
        case TK_PLUS:
        case TK_MINUS:
        case TK_MUL:
        case TK_DIVIDE:
        case TK_AND:
        case TK_OR:
        case TK_LT:
        case TK_LE:
        case TK_GT:
        case TK_GE:
        case TK_EQ:
        case TK_NE:
        case TK_TRUE:
        case TK_FALSE:
        case TK_BOOLEAN:
        case TK_REAL:
        case TK_INTEGER:
        {
            break;
        }
        default:
        {
            flag = false;
            break;
        }
    }
    return flag;
}

parseTreeNode deep_copy(parseTreeNode root)
{
    parseTreeNode temp = (parseTreeNode)malloc(sizeof(struct PARSETREENODE));
    temp->tok = root->tok;
    temp->s = root->s;
    return temp;
}
parseTreeNode delete_node(parseTreeNode parent, parseTreeNode prev, parseTreeNode currnode)
{
    if (prev == NULL)
    {
        parent->child = currnode->sibling;
    }
    else
    {
        prev->sibling = currnode->sibling;
    }
    currnode->sibling->prevSibling = prev;
    free(currnode);
    return prev->sibling;
}
void add_child(parseTreeNode parent, parseTreeNode child)
{
    if (child == NULL)
    {
        return;
    }
    if (parent->child == NULL)
    {
        parent->child = child;
        child->parent = parent;
    }
    else
    {
        parseTreeNode currnode = parent->child;
        while (currnode->sibling != NULL)
        {
            currnode = currnode->sibling;
        }
        currnode->sibling = child;
        child->parent = parent;
        child->prevSibling = currnode;
    }
}

parseTreeNode generate_ast(parseTreeNode root)
{
    parseTreeNode currnode = root;

    parseTreeNode currchild = root->child;
    parseTreeNode prev = NULL;
    if (!root->s->is_terminal)
    {
        // printf("currnode=%s\n", nonterminal_str[currnode->s->nt]);
        switch (root->s->nt)
        {
        case program:
        {
            // 1
            currnode->syn_node = createTree();
            currnode->syn_node->parent = NULL;
            // printf("currnode = %s\n",nonterminal_str[currnode->s->nt]);
            currnode->syn_node->ast_name = AST_PROGRAM;

            parseTreeNode temp1 = createTree();
            add_child(temp1, generate_ast(currchild)->syn_node);
            temp1->ast_name = AST_MODULEDECLARATIONS;
            add_child(currnode->syn_node, temp1);
            currchild = currchild->sibling;

            parseTreeNode temp2 = createTree();
            add_child(temp2, generate_ast(currchild)->syn_node);
            temp2->ast_name = AST_MODULEDEFINITIONS;
            add_child(currnode->syn_node, temp2);

            currchild = currchild->sibling;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            currchild->syn_node->scope[1]=currchild->sibling->tok->line_no;
            currchild = currchild->sibling->sibling;

            parseTreeNode temp3 = createTree();
            add_child(temp3, generate_ast(currchild)->syn_node);
            temp3->ast_name = AST_MODULEDEFINITIONS;
            add_child(currnode->syn_node, temp3);
            // while (currchild != NULL)
            // {
            //     printf("currchild = %s\n",nonterminal_str[currchild->s->nt]);
            //     printf("check\n");
            //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            //     currchild->is_visited = true;
            //     currchild = currchild->sibling;
            // }
            break;
        }
        case moduleDeclarations:
        {
            // 2,3
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }
            currnode->syn_node = generate_ast(currchild)->syn_node;
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling)->syn_node;
            break;
        }
        case moduleDeclaration:
        {
            // 4
            currnode->syn_node = generate_ast(currchild->sibling->sibling);
            break;
        }
        case otherModules:
        {
            // 5,6
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }
            currnode->syn_node = generate_ast(currchild)->syn_node;
            if (currnode->syn_node != NULL){
                currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling)->syn_node;
                currchild->syn_node->scope[1]=currnode->child->sibling->tok->line_no;
            }
            break;
        }
        case driverModule:
        {
            // 7
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_DRIVER;
            currnode->syn_node->scope[0]=currchild->tok->line_no;
            while (currchild!= NULL)
            {
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                currchild->is_visited = true;
                currchild = currchild->sibling;
            }
            
            // currnode->syn_node->scope[1]=currnode->child->sibling->sibling->sibling->sibling->syn_node->scope[1];
            break;
        }

        case module:
        {
            // 8
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_MODULE;
            currnode->syn_node->scope[0]=currchild->tok->line_no;
            currchild = currchild->sibling->sibling;

            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);

            currchild = currchild->sibling->sibling->sibling->sibling->sibling;

            parseTreeNode temp1 = createTree();
            add_child(temp1, generate_ast(currchild)->syn_node);
            temp1->ast_name = AST_INPUT_PARAMETER_LIST;
            add_child(currnode->syn_node, temp1);
            currchild = currchild->sibling->sibling->sibling;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            currchild = currchild->sibling;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            currnode->syn_node->scope[1]=currchild->syn_node->scope[1];
            // while (currchild != NULL)
            // {
            //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            //     currchild->is_visited = true;
            //     currchild = currchild->sibling;
            // }
            break;
        }
        case ret:
        {
            // 9,10
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_OUTPUT_PARAMETER_LIST;
            if (currchild->s->is_terminal && currchild->s->t != epsilon)
            {
                add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            }

            // while (currchild != NULL)
            // {
            //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            //     currchild->is_visited = true;
            //     currchild = currchild->sibling;
            // }

            break;
        }
        case input_plist:
        {
            // 11
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_INP_PARAMETER;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
            break;
        }
        case N1:
        {
            // 12,13
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_INP_PARAMETER;
            currchild = currchild->sibling;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
            break;
        }
        case output_plist:
        {
            // 14
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_OUT_PARAMETER;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
            break;
        }

        case N2:
        {
            // 15,16
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_OUT_PARAMETER;
            currchild = currchild->sibling;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
            break;
        }

        case dataType:
        {
            // 17 18 19 20
            if (currchild->s->t == TK_ARRAY)
            {
                currnode->syn_node = createTree();
                currnode->syn_node->ast_name = AST_ARRAY;
                currchild = currchild->sibling->sibling;
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                currchild = currchild->sibling->sibling->sibling;
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                // while (currchild != NULL)
                // {
                //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                //     currchild->is_visited = true;
                //     currchild = currchild->sibling;
                // }
            }
            else
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
                // if (currnode->syn_node != NULL && currnode->syn_node->s->is_terminal)
                // {
                //     printf("currchild=%s\n", terminal_str[currnode->syn_node->s->t]);
                // }
            }
            break;
        }
        case range_arrays:
        {
            // 21
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_RANGE_ARRAYS;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            break;
        }
        case type:
        {
            // 22,23 ,24
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }

        case moduleDef:
        {
            // 25
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_STATEMENTS;
            add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
            currnode->syn_node->scope[1]=currchild->sibling->syn_node->scope[1];
            // while (currchild != NULL)
            // {
            //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            //     currchild->is_visited = true;
            //     currchild = currchild->sibling;
            // }
            break;
        }
        case statements:
        {
            // 26-31
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }

            currnode->syn_node = generate_ast(currchild)->syn_node;
            if (currnode->syn_node != NULL){
                currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling)->syn_node;
                currnode->syn_node->scope[1]=currchild->sibling->tok->line_no;
            }
            break;
        }
        case ioStmt:
        {
            // 32-33
            currnode->syn_node = createTree();
            if (currchild->s->is_terminal && currchild->s->t == TK_GETVALUE)
            {
                currnode->syn_node->ast_name = AST_GET_VALUE;
            }
            else
            {
                currnode->syn_node->ast_name = AST_PRINT;
            }
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            // while (currchild != NULL)
            // {
            //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            //     currchild->is_visited = true;
            //     currchild = currchild->sibling;
            // }
            break;
        }
        case boolConstt:
        {
            // 34-35
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case var_print:
        {
            // 36-39
            if (currchild->s->is_terminal && currchild->s->t == TK_ID)
            {
                currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            else
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            break;
        }
        case P1:
        {
            // 40-41
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            else
            {
                currnode->syn_node = createTree();
                currnode->syn_node->ast_name = AST_ARRAY_ACCESS;
                add_child(currnode->syn_node, currnode->inh_node);
                add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
                // while (currchild != NULL)
                // {
                //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                //     currchild->is_visited = true;
                //     currchild = currchild->sibling;
                // }
            }
            break;
        }
        case simpleStmt:
        {
            // 42-43
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case assignmentStmt:
        {
            // 44
            currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            break;
        }

        case whichStmt:
        {
            // 45-46
            currchild->inh_node = currnode->inh_node;
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }

        case lvalueIDStmt:
        {
            // 47
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_ID_ASSIGN;
            add_child(currnode->syn_node, currnode->inh_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
            // while (currchild != NULL)
            // {
            //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            //     currchild->is_visited = true;
            //     currchild = currchild->sibling;
            // }
            break;
        }

        case lvalueARRStmt:
        {
            // 48
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_ARRAY_ASSIGN;
            parseTreeNode temp = createTree();
            temp->ast_name = AST_ARRAY_ACCESS;
            add_child(temp, currnode->inh_node);
            add_child(temp, generate_ast(currchild->sibling)->syn_node);
            add_child(currnode->syn_node, temp);
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling->sibling->sibling)->syn_node);

            break;
        }

        case index_arr:
        {
            // 49
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_INDEX_ARR;
            while (currchild != NULL)
            {
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                currchild = currchild->sibling;
            }
            break;
        }
        case new_index:
        {
            // 50-51
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case sign:
        {
            // 52-54
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case moduleReuseStmt:
        {
            // 55
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_MODULE_REUSE;

            parseTreeNode temp1 = createTree();
            temp1->ast_name = AST_PARAMETER_LIST1;
            add_child(temp1, generate_ast(currchild)->syn_node);

            currchild = currchild->sibling->sibling->sibling;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, temp1);

            currchild = currchild->sibling->sibling->sibling;
            parseTreeNode temp2 = createTree();
            temp2->ast_name = AST_PARAMETER_LIST2;
            add_child(temp2, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, temp2);

            // while (currchild != NULL)
            // {
            //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            //         currchild->is_visited = true;
            //         currchild = currchild->sibling;
            // }
            break;
        }
        case actual_para_list:
        {
            // 56-57
            if (currchild->s->nt == unary_op)
            {
                currnode->syn_node = createTree();
                currnode->syn_node->ast_name = AST_ACTUAL_PARA;
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
                parseTreeNode temp = currnode->inh_node;

                if (temp == NULL)
                {
                    currnode->inh_node = currnode->syn_node;
                }
                else
                {
                    while (temp->sibling != NULL)
                    {
                        temp = temp->sibling;
                    }

                    temp->sibling = currnode->syn_node;
                }
                currchild->sibling->sibling->inh_node = currnode->inh_node;
                currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
            }
            else
            {

                currnode->syn_node = createTree();
                currnode->syn_node->ast_name = AST_ACTUAL_PARA;
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                parseTreeNode temp = currnode->inh_node;
                if (temp == NULL)
                {
                    currnode->inh_node = currnode->syn_node;
                }
                else
                {
                    while (temp->sibling != NULL)
                    {
                        temp = temp->sibling;
                    }

                    temp->sibling = currnode->syn_node;
                }
                currchild->sibling->inh_node = currnode->inh_node;
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            break;
        }

        case N20:
        {
            // 58-59
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            else
            {
                currchild->sibling->inh_node = currnode->inh_node;
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
                break;
            }
        }

        case optional:
        { // 60,61
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            break;
        }
        case idList:
        { // 62
            currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            break;
        }
        case N3:
        {
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            parseTreeNode temp = currnode->inh_node;
            if (temp == NULL)
            {
                currnode->inh_node = generate_ast(currchild->sibling)->syn_node;
            }
            else
            {
                while (temp->sibling != NULL)
                {
                    temp = temp->sibling;
                }
                temp->sibling = generate_ast(currchild->sibling)->syn_node;
            }
            currchild->sibling->sibling->inh_node = currnode->inh_node;

            currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;

            break;
        }

        case expression:
        {
            // 65,66
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case U:
        { // 67
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_UNARYEXPR;
            while (currchild != NULL)
            {
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                currchild = currchild->sibling;
            }
            break;
        }
        case new_NT:
        { // 68,69
            if (currchild->sibling != NULL)
            {
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            else
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            break;
        }
        case var_id_num:
        { // 70,71,72
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }

        case unary_op:
        { // 73,74
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case arithmeticOrBooleanExpr:
        { // 75
            currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            // printf("from pos 2\n");
            break;
        }
        case N7:
        { // 76,77
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                // printf("from pos1\n");
                currnode->syn_node = currnode->inh_node;
                break;
            }
            currchild->sibling->sibling->inh_node = createTree();

            currchild->sibling->sibling->inh_node->ast_name = AST_LOGICAL_OP;
            add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
            add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild)->syn_node);

            add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
            currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
            // printf("from pos3\n");
            break;
        }
        case AnyTerm:
        { // 78,79
            if (currchild->sibling == NULL)
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            else
            {
                currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            break;
        }
        case N8:
        { // 80,81
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_RELATIONAL_OP;
            add_child(currnode->syn_node, currnode->inh_node);
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
            break;
        }
        case arithmeticExpr:
        { // 82
            currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            break;
        }
        case N4:
        { // 83,84
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            parseTreeNode temp1 = generate_ast(currchild)->syn_node;
            parseTreeNode temp2 = createTree();
            if (temp1->s->t == TK_PLUS)
            {
                temp2->ast_name = AST_PLUS;
            }
            else
            {
                temp2->ast_name = AST_MINUS;
            }
            add_child(temp2, currnode->inh_node);
            add_child(temp2, temp1);
            add_child(temp2, generate_ast(currchild->sibling)->syn_node);
            currchild->sibling->sibling->inh_node = temp2;
            currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
            break;
        }

        case term:
        { // 85
            currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            break;
        }
        case N5:
        { // 86,87
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            currchild->sibling->sibling->inh_node = createTree();
            add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
            parseTreeNode temp = generate_ast(currchild)->syn_node;
            if (temp->s->t == TK_MUL)
            {
                currchild->sibling->sibling->inh_node->ast_name = AST_MUL;
            }
            else
            {
                currchild->sibling->sibling->inh_node->ast_name = AST_DIV;
            }
            add_child(currchild->sibling->sibling->inh_node, temp);
            add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
            currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
            break;
        }

        case factor:
        { // 88,89,90,91,92
            if (currchild->sibling != NULL && currchild->sibling->sibling != NULL)
            {
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            else if (currchild->sibling != NULL && currchild->sibling->sibling == NULL)
            {
                currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            else if (currchild->s->is_terminal)
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            else
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            break;
        }
        case N11:
        { // 93,94
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_ARRAY_FACTOR;
            add_child(currnode->syn_node, currnode->inh_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
            break;
        }
        case arrExpr:
        { // 95
            currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            break;
        }
        case arr_N4:
        { // 96,97
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            currchild->sibling->sibling->inh_node = createTree();
            parseTreeNode temp = generate_ast(currchild)->syn_node;
            if (temp->s->t == TK_PLUS)
            {
                currchild->sibling->sibling->inh_node->ast_name = AST_PLUS;
            }
            else
            {
                currchild->sibling->sibling->inh_node->ast_name = AST_MINUS;
            }
            add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
            add_child(currchild->sibling->sibling->inh_node, temp);
            add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
            currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
            break;
        }
        case arrTerm:
        { // 98
            currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            break;
        }
        case arr_N5:
        { // 99,100
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = currnode->inh_node;
                break;
            }
            currchild->sibling->sibling->inh_node = createTree();
            parseTreeNode temp = generate_ast(currchild)->syn_node;
            if (temp->s->t == TK_MUL)
            {
                currchild->sibling->sibling->inh_node->ast_name = AST_MUL;
            }
            else
            {
                currchild->sibling->sibling->inh_node->ast_name = AST_DIV;
            }
            add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
            add_child(currchild->sibling->sibling->inh_node, temp);
            add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
            currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
            break;
        }
        case arrFactor:
        { // 101 to 104
            if (currchild->s->is_terminal && currchild->sibling == NULL)
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            else if (!currchild->s->is_terminal)
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            else
            {
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            break;
        }
        case element_index_with_expressions:
        { // 105,106
            if (currchild->sibling != NULL)
            {
                currnode->syn_node = createTree();
                currnode->syn_node->ast_name = AST_UNARY_INDEX_EXPR;
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
            }
            else
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            break;
        }
        case N10:
        { // 107,108
            if (!currchild->s->is_terminal)
            {
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }
            else
            {
                currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            }
            break;
        }
        case op1:
        { // 109,110
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case op2:
        { // 111,112
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case logicalOp:
        { // 113,114
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case relationalOp:
        { // 115to120
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case declareStmt:
        {
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_DECLARE_STMT;

            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);

            break;
        }
        case condionalStmt:
        {
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_SWITCH;

            currchild = currchild->sibling->sibling;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            parseTreeNode temp = createTree();
            temp->ast_name = AST_CASES;
            currchild = currchild->sibling->sibling->sibling;
            add_child(temp, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, temp);
            add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);

            break;
        }
        case caseStmts:
        {
            parseTreeNode temp = createTree();
            temp->ast_name = AST_CASE;
            temp->scope[0]=currchild->tok->line_no;
            add_child(temp, generate_ast(currchild->sibling)->syn_node);
            parseTreeNode temp2 = createTree();
            temp2->ast_name = AST_STATEMENTS;
            add_child(temp2, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
            add_child(temp, temp2);
            currchild = currchild->sibling->sibling->sibling->sibling->sibling->sibling;
            temp->sibling = generate_ast(currchild)->syn_node;
            currnode->syn_node = temp;
            currnode->syn_node->scope[1]=currnode->child->sibling->sibling->sibling->sibling->tok->line_no;
            break;
        }

        case N9:
        {
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }
            parseTreeNode temp = createTree();
            temp->ast_name = AST_CASE;
            temp->scope[0]=currchild->tok->line_no;
            add_child(temp, generate_ast(currchild->sibling)->syn_node);
            parseTreeNode temp2 = createTree();
            temp2->ast_name = AST_STATEMENTS;
            add_child(temp2, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
            add_child(temp, temp2);
            currchild = currchild->sibling->sibling->sibling->sibling->sibling->sibling;
            temp->sibling = generate_ast(currchild)->syn_node;
            currnode->syn_node = temp;
            currnode->syn_node->scope[1]=currnode->child->sibling->sibling->sibling->sibling->tok->line_no;
            
            break;
        }
        case value:
        {
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case DEFAULT:
        {
            if (currchild->s->is_terminal && currchild->s->t == epsilon)
            {
                currnode->syn_node = NULL;
                break;
            }
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_DEFAULT;
            currnode->syn_node->scope[0]=currchild->tok->line_no;
            currnode->syn_node->scope[1]=currchild->sibling->sibling->sibling->tok->line_no;
            parseTreeNode temp = createTree();
            temp->ast_name = AST_STATEMENTS;
            add_child(temp, generate_ast(currchild->sibling->sibling)->syn_node);
            add_child(currnode->syn_node, temp);
            break;
        }
        case iterativeStmt:
        {
            if (currchild->s->t == TK_FOR)
            {
                currnode->syn_node = createTree();
                currnode->syn_node->ast_name = AST_FORLOOP;
                currnode->syn_node->scope[0]=currchild->tok->line_no;
                add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
                currchild = currchild->sibling->sibling->sibling->sibling;
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                parseTreeNode temp = createTree();
                temp->ast_name = AST_STATEMENTS;
                add_child(temp, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
                add_child(currnode->syn_node, temp);
                currnode->syn_node->scope[1]=currchild->sibling->sibling->sibling->syn_node->scope[1];
                break;
            }
            else
            {
                currnode->syn_node = createTree();
                currnode->syn_node->ast_name = AST_WHILELOOP;
                currnode->syn_node->scope[0]=currchild->tok->line_no;
                add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
                currchild = currchild->sibling->sibling->sibling->sibling->sibling;
                parseTreeNode temp = createTree();
                temp->ast_name = AST_STATEMENTS;
                add_child(temp, generate_ast(currchild)->syn_node);
                add_child(currnode->syn_node, temp);
                currnode->syn_node->scope[1]=currchild->syn_node->scope[1];
                break;
            }
        }
        case range_for_loop:
        {
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_RANGE_ARRAYS;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
            break;
        }

        case index_for_loop:
        {
            currnode->syn_node = createTree();
            currnode->syn_node->ast_name = AST_INDEX_FOR_LOOP;
            add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
            add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
            break;
        }
        case new_index_for_loop:
        {
            currnode->syn_node = generate_ast(currchild)->syn_node;
            break;
        }
        case sign_for_loop:
        {
            currnode->syn_node = generate_ast(currchild)->syn_node;

            break;
        }
        default:
        {
            break;
        }
        }
    }
    else
    {
        if (is_important_terminal(root))
        {
            root->syn_node = deep_copy(root);
            return root;
        }
        else
        {
            root->syn_node = NULL;
            return root;
        }
    }
    return currnode;
}
