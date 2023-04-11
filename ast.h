/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#ifndef _ast_
#define _ast_
#include "astdef.h"

parseTreeNode generate_ast(parseTreeNode node);

bool is_important_terminal(parseTreeNode node);

parseTreeNode deep_copy(parseTreeNode root);

parseTreeNode delete_node(parseTreeNode parent, parseTreeNode prev, parseTreeNode currnode);

void add_child(parseTreeNode parent, parseTreeNode child);

#endif

