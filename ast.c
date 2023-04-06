#include "ast.h"

bool is_important_terminal(parseTreeNode node)
{
    parseTreeNode temp = node;
    bool flag = true;

    switch (temp->s->t)
    {
    case TK_ID:
    {
        break;
    }
    case TK_NUM:
    {
        break;
    }
    case TK_RNUM:
    {
        break;
    }
    case TK_PLUS:
    {
        break;
    }
    case TK_MINUS:
    {
        break;
    }
    case TK_MUL:
    {
        break;
    }
    case TK_DIVIDE:
    {
        break;
    }
    case TK_AND:
    {
        break;
    }
    case TK_OR:
    {
        break;
    }
    case TK_LT:
    {
        break;
    }
    case TK_LE:
    {
        break;
    }
    case TK_GT:
    {
        break;
    }
    case TK_GE:
    {
        break;
    }
    case TK_EQ:
    {
        break;
    }
    case TK_NE:
    {
        break;
    }
    case TK_TRUE:
    {
        break;
    }
    case TK_FALSE:
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
        switch (root->s->nt)
        {
        case program:
        {
            // 1
            currnode->syn_node = createTree();
            currnode->syn_node->parent = NULL;
            // printf("currnode = %s\n",nonterminal_str[currnode->s->nt]);
            currnode->syn_node->ast_name = AST_PROGRAM;
            
            
            
            parseTreeNode temp1= createTree();
            add_child(temp1, generate_ast(currchild)->syn_node);
            temp1->ast_name = AST_MODULEDECLARATIONS;
            add_child(currnode->syn_node,temp1);
            currchild=currchild->sibling;
            
            parseTreeNode temp2=createTree();
            add_child(temp2,generate_ast(currchild)->syn_node);
            temp2->ast_name = AST_MODULEDEFINITIONS;
            add_child(currnode->syn_node,temp2);
            
            currchild=currchild->sibling;
            add_child(currnode->syn_node,generate_ast(currchild)->syn_node);
            currchild=currchild->sibling;
            
            parseTreeNode temp3=createTree();
            add_child(temp3,generate_ast(currchild)->syn_node);
            temp3->ast_name = AST_MODULEDEFINITIONS;
            add_child(currnode->syn_node,temp3);
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
            currnode->syn_node = currchild->sibling->sibling;
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
            if(currnode->syn_node != NULL)
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling)->syn_node;
            break;
        }
        case driverModule:
        {
            // 7
            currnode->syn_node = deep_copy(currnode);
            currnode->syn_node->parent = NULL;
            while (currchild != NULL)
            {
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                currchild->is_visited = true;
                currchild = currchild->sibling;
            }
            break;
        }

        case module:
        {
            // 8
            currnode->syn_node = deep_copy(currnode);
            currnode->syn_node->parent = NULL;
            
            while (currchild != NULL)
            {
                add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
                currchild->is_visited = true;
                currchild = currchild->sibling;
            }
            currnode->child->sibling->sibling->sibling = NULL;
            break;
        }
        // case ret:
        // {
        //     // 9,10
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        // case input_plist:
        // {
        //     // 11
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
        //     currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
        //     break;
        // }
        // case N1:
        // {
        //     // 12,13
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = NULL;
        //         break;
        //     }
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
        //     currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
        //     break;
        // }
        // case output_plist:
        // {
        //     // 14
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
        //     currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
        //     break;
        // }

        // case N2:
        // {
        //     // 15,16
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = NULL;
        //         break;
        //     }
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
        //     currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
        //     break;
        // }

        // case dataType:
        // {
        //     // 17 18 19 20
        //     if (currchild->s->t = TK_ARRAY)
        //     {
        //         currnode->syn_node = deep_copy(currnode);
        //         currnode->syn_node->parent = NULL;
        //         while (currchild != NULL)
        //         {
        //             add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //             currchild->is_visited = true;
        //             currchild = currchild->sibling;
        //         }
        //     }
        //     else
        //     {
        //         currnode->syn_node = currchild;
        //     }
        //     break;
        // }
        // case range_arrays:
        // {
        //     // 21
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        // case type:
        // {
        //     // 22,23 ,24
        //     currnode->syn_node = currchild;
        //     break;
        // }

        // case moduleDef:
        // {
        //     // 25
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        // case statements:
        // {
        //     // 26-31
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = NULL;
        //         break;
        //     }
        //     currnode->syn_node = generate_ast(currchild)->syn_node;
        //     currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling)->syn_node;
        //     break;
        // }
        // case ioStmt:
        // {
        //     // 32-33
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        // case boolConstt:
        // {
        //     // 34-35
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case var_print:
        // {
        //     // 36-39
        //     if (currchild->s->is_terminal && currchild->s->t == TK_ID)
        //     {
        //         currchild->sibling->inh_node = currchild;
        //         currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     }
        //     else
        //     {
        //         currnode->syn_node = generate_ast(currchild)->syn_node;
        //     }
        //     break;
        // }
        // case P1:
        // {
        //     // 40-41
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     else
        //     {
        //        currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     add_child(currnode->syn_node,currnode->inh_node);
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     }
        //     break;
        // }
        // case simpleStmt:
        // {
        //     //42-43
        //     currnode->syn_node=generate_ast(currchild)->syn_node;
        //     break;
        // }
        // case assignmentStmt:
        // {
        // //44
        // currchild->sibling->inh_node = currchild;
        // currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        // break;
        // }
        
        // case whichStmt:
        // {
        // // 45-46
        // currchild->inh_node=currnode->inh_node;
        // currnode->syn_node=generate_ast(currchild)->syn_node;
        // break;
        // }
        
        // case lvalueIDStmt:
        // {
        // //47
        //    currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     add_child(currnode->syn_node,currnode->inh_node);
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        
        // case lvalueARRStmt:
        // {
        // //48
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     add_child(currnode->syn_node,currnode->inh_node);
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        
        // case index_arr:
        // {
        // //49
        // currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        // case new_index:
        // {
        // //50-51
        // currnode->syn_node=currchild;
        // break;
        // }
        // case sign:
        // {
        // //52-54
        // currnode->syn_node=generate_ast(currchild)->syn_node;
        // break;
        // }
        // case moduleReuseStmt:
        // {
        // //55
        // currnode->syn_node = deep_copy(currnode);
        // currnode->syn_node->parent = NULL;
        // while (currchild != NULL)
        // {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        // }
        // break;
        // }
        // case actual_para_list:
        // {
        // //56-57
        // if(currchild->s->nt==unary_op){
        // currchild->sibling->sibling->inh_node=currnode->inh_node;
        // parseTreeNode temp=deep_copy(currnode);
        // add_child(temp, generate_ast(currchild)->syn_node);
        // add_child(temp, generate_ast(currchild->sibling)->syn_node);
        // currchild->sibling->sibling->inh_node->sibling=temp;
        // currnode->syn_node=generate_ast(currchild->sibling->sibling)->syn_node;
        // break;
        // }
        // else{
        // currchild->sibling->inh_node=currnode->inh_node;
        // currchild->sibling->inh_node->sibling=generate_ast(currchild)->syn_node;
        // currnode->syn_node=generate_ast(currchild->sibling)->syn_node;
        // break;
        // }
        // }
        
        




        // case N20:{
        // //58-59
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     else{
        //         currchild->sibling->inh_node = currnode->inh_node;
        //         currnode->syn_node = currchild->sibling->syn_node;
        //         break;
        //     }
        // }








        // case optional:{//60,61
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = NULL;
        //         break;
        //     }
        //     currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     break;
        // }
        // case idList:{//62
        //     currchild->sibling->inh_node = currchild;
        //     currnode->syn_node = currchild->sibling->syn_node;
        //     break;
        // }
        // case N3:{
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     parseTreeNode temp = currnode->inh_node;
        //     while(temp->sibling!=NULL){
        //         temp = temp->sibling;
        //     }
        //     temp->sibling = currchild->sibling;
            
        //     currchild->sibling->sibling->inh_node = currnode->inh_node;
        
        //     break;
        // }
        
        //     case expression:
        // {
        //     // 65,66
        //     currnode->syn_node = generate_ast(currchild)->syn_node;
        //     break;
        // }
        // case U:
        // { // 67
        //     currnode->syn_node = deep_copy(currnode);
        //     currnode->syn_node->parent = NULL;
        //     while (currchild != NULL)
        //     {
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         currchild->is_visited = true;
        //         currchild = currchild->sibling;
        //     }
        //     break;
        // }
        // case new_NT:
        // { // 68,69
        //     if (currchild->sibling != NULL)
        //     {
        //         currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     }
        //     else
        //     {
        //         currnode->syn_node = generate_ast(currchild)->syn_node;
        //     }
        //     break;
        // }
        // case var_id_num:
        // { // 70,71,72
        //     currnode->syn_node = currchild;
        //     break;
        // }

        // case unary_op:
        // { // 73,74
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case arithmeticOrBooleanExpr:
        // { // 75
        //     currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
        //     currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     break;
        // }
        // case N7:
        // { // 76,77
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     currchild->sibling->sibling->inh_node = deep_copy(currchild->sibling->sibling);
        //     currchild->sibling->sibling->inh_node->parent = NULL;
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild)->syn_node);
        //     add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
        //     currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
        //     break;
        // }
        // case AnyTerm:
        // { // 78,79
        //     if (currchild->sibling == NULL)
        //     {
        //         currnode->syn_node = generate_ast(currchild)->syn_node;
        //     }
        //     else
        //     {
        //         currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
        //         currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     }
        //     break;
        // }
        // case N8:
        // { // 80,81
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, currnode->inh_node);
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
        //     break;
        // }
        // case arithmeticExpr:
        // { // 82
        //     currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
        //     currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     break;
        // }
        // case N4:
        // { // 83,84
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     currchild->sibling->sibling->inh_node = deep_copy(currchild->sibling->sibling);
        //     currchild->sibling->sibling->inh_node->parent = NULL;
        //     add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild)->syn_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
        //     currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
        //     break;
        // }

        // case term:
        // { // 85
        //     currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
        //     currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     break;
        // }
        // case N5:
        // { // 86,87
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     currchild->sibling->sibling->inh_node = deep_copy(currchild->sibling->sibling);
        //     currchild->sibling->sibling->inh_node->parent = NULL;
        //     add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild)->syn_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
        //     currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
        //     break;
        // }

        // case factor:
        // { // 88,89,90,91,92
        //     if (currchild->sibling != NULL && currchild->sibling->sibling != NULL)
        //     {
        //         currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     }
        //     else if (currchild->sibling != NULL && currchild->sibling->sibling == NULL)
        //     {
        //         currchild->sibling->inh_node = currchild;
        //         currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     }
        //     else if (currchild->s->is_terminal)
        //     {
        //         currnode->syn_node = currchild;
        //     }
        //     else
        //     {
        //         currnode->syn_node = generate_ast(currchild)->syn_node;
        //     }
        //     break;
        // }
        // case N11:
        // { // 93,94
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, currnode->inh_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
        //     break;
        // }
        // case arrExpr:
        // { // 95
        //     currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
        //     currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     break;
        // }
        // case arr_N4:
        // { // 96,97
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     currchild->sibling->sibling->inh_node = deep_copy(currchild->sibling->sibling);
        //     currchild->sibling->sibling->inh_node->parent = NULL;
        //     add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild)->syn_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
        //     currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
        //     break;
        // }
        // case arrTerm:
        // { // 98
        //     currchild->sibling->inh_node = generate_ast(currchild)->syn_node;
        //     currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     break;
        // }
        // case arr_N5:
        // { // 99,100
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = currnode->inh_node;
        //         break;
        //     }
        //     currchild->sibling->sibling->inh_node = deep_copy(currchild->sibling->sibling);
        //     currchild->sibling->sibling->inh_node->parent = NULL;
        //     add_child(currchild->sibling->sibling->inh_node, currnode->inh_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild)->syn_node);
        //     add_child(currchild->sibling->sibling->inh_node, generate_ast(currchild->sibling)->syn_node);
        //     currnode->syn_node = generate_ast(currchild->sibling->sibling)->syn_node;
        //     break;
        // }
        // case arrFactor:
        // { // 101 to 104
        //     if (currchild->s->is_terminal && currchild->sibling == NULL)
        //     {
        //         currnode->syn_node = currchild;
        //     }
        //     else if (!currchild->s->is_terminal)
        //     {
        //         currnode->syn_node = generate_ast(currchild)->syn_node;
        //     }
        //     else
        //     {
        //         currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     }
        //     break;
        // }
        // case element_index_with_expressions:
        // { // 105,106
        //     if (currchild->sibling != NULL)
        //     {
        //         currnode->syn_node = deep_copy(currnode);
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
        //     }
        //     else
        //     {
        //         currnode->syn_node = generate_ast(currchild)->syn_node;
        //     }
        //     break;
        // }
        // case N10:
        // { // 107,108
        //     if (!currchild->s->is_terminal)
        //     {
        //         currnode->syn_node = generate_ast(currchild)->syn_node;
        //     }
        //     else
        //     {
        //         currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
        //     }
        //     break;
        // }
        // case op1:
        // { // 109,110
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case op2:
        // { // 111,112
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case logicalOp:
        // { // 113,114
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case relationalOp:
        // { // 115to120
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case declareStmt:
        // {
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);

        //     break;
        // }
        // case condionalStmt:
        // {
        //     currnode->syn_node = deep_copy(currnode);
        //     currchild = currchild->sibling->sibling;
        //     add_child(currnode->syn_node, currchild);
        //     currchild = currchild->sibling->sibling->sibling;
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);

        //     break;
        // }
        // case caseStmts:
        // {
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
        //     currchild = currchild->sibling->sibling->sibling;
        //     // add_child(currnode->syn_node,generate_ast(currchild->sibling->sibling->sibling)->syn_node);
        //     currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
        //     break;
        // }

        // case N9:
        // {
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = NULL;
        //         break;
        //     }
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
        //     currchild = currchild->sibling->sibling->sibling;
        //     // add_child(currnode->syn_node,generate_ast(currchild->sibling->sibling->sibling)->syn_node);
        //     currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling->sibling)->syn_node;
        //     break;
        // }
        // case value:
        // {
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case DEFAULT:
        // {
        //     if (currchild->s->is_terminal && currchild->s->t == epsilon)
        //     {
        //         currnode->syn_node = NULL;
        //         break;
        //     }
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
        //     break;
        // }
        // case iterativeStmt:
        // {
        //     if (currchild->s->t == TK_FOR)
        //     {
        //         currnode->syn_node = deep_copy(currnode);
        //         add_child(currnode->syn_node, currchild->sibling->sibling);
        //         currchild = currchild->sibling->sibling->sibling->sibling;
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling->sibling)->syn_node);
        //         break;
        //     }
        //     else
        //     {
        //         currnode->syn_node = deep_copy(currnode);
        //         add_child(currchild->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
        //         currchild = currchild->sibling->sibling->sibling->sibling->sibling;
        //         add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //         break;
        //     }
        // }
        // case range_for_loop:
        // {
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling->sibling)->syn_node);
        //     break;
        // }

        // case index_for_loop:
        // {
        //     currnode->syn_node = deep_copy(currnode);
        //     add_child(currnode->syn_node, generate_ast(currchild)->syn_node);
        //     add_child(currnode->syn_node, generate_ast(currchild->sibling)->syn_node);
        //     break;
        // }
        // case new_index_for_loop:
        // {
        //     currnode->syn_node = currchild;
        //     break;
        // }
        // case sign_for_loop:
        // {
        //     currnode->syn_node = currchild;
            
        //     break;
        // }
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
            root->syn_node = root;
            return root;
        }
        else
        {
            root->syn_node =NULL;
            return root;
        }
    }
    return currnode;
}