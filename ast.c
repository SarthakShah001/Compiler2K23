#include "ast.h"
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
    free(currnode);
    return prev->sibling;
}
parseTreeNode generate_ast(parseTreeNode root)
{
    parseTreeNode temp = root;
    parseTreeNode currchild = root->child;
    parseTreeNode prev = NULL;
    // handle base case

    // non terminal
    if (temp->s->is_terminal == false)
    {
        switch (temp->s->nt)
        {
        case program:
        {
            while (currchild != NULL)
            {
                if (prev == NULL)
                {
                    parseTreeNode new_node = createTree();
                    new_node->child = generate_ast(currchild);
                    prev = new_node;
                    currchild = currchild->sibling;
                    free(root->child);
                    root->child = new_node;
                }
                else
                {
                    if (currchild->s->is_terminal == true)
                    {
                        currchild = delete_node(root, prev, currchild);
                    }
                    else
                    {
                        parseTreeNode new_node = createTree();
                        new_node->child = generate_ast(currchild);
                        prev->sibling = new_node;
                        prev = currchild;
                        currchild = currchild->sibling;
                        free(prev);
                        prev = new_node;
                    }
                }
            }
        }
        case moduleDeclarations:
        {
            
        }
        }
    }
}