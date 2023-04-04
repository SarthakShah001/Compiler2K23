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
        case moduleDeclarations:
        {
        //2,3
            if(currchild->s->is_terminal&&currchild->s->t==epsilon){
            currnode->syn_node = NULL;
            break;
            }
            currnode->syn_node = generate_ast(currchild)->syn_node;
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling)->syn_node;
            break;
        }
        case moduleDeclaration:
        {
            //4
            currnode->syn_node=currchild->sibling->sibling;
            break;
        }
        case otherModules:
        {
        // 5,6
        if(currchild->s->is_terminal&&currchild->s->t==epsilon){
            currnode->syn_node = NULL;
            break;
        }
            currnode->syn_node = generate_ast(currchild)->syn_node;
            currnode->syn_node->sibling = generate_ast(currchild->sibling->sibling)->syn_node;
        break;
        }
        case driverModule:
        {
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
        case 
        
        
        
        case expression:{
            //65,66
            currnode->syn_node = generate_ast(currchild)->syn_node;
        }
        case U:{
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
        case new_NT:{
            if(currchild->sibling!=NULL)
            currnode->syn_node = generate_ast(currchild->sibling)->syn_node;
            else{
                currnode->syn_node = generate_ast(currchild)->syn_node;
            }

        }
        case var_id_num:{
            currnode->syn_node = currchild;
        }

        case 
        
        default:{
        }
        }
    }
}