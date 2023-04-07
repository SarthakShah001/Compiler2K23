#ifndef _ast_
#define _ast_
#include "astdef.h"

parseTreeNode generate_ast(parseTreeNode node);

bool is_important_terminal(parseTreeNode node);

parseTreeNode deep_copy(parseTreeNode root);

parseTreeNode delete_node(parseTreeNode parent, parseTreeNode prev, parseTreeNode currnode);

void add_child(parseTreeNode parent, parseTreeNode child);

#endif

