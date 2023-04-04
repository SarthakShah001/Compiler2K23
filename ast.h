#ifndef _ast_
#define _ast_
#include "astdef.h"
parseTreeNode generate_ast(parseTreeNode node);
bool is_important_terminal(parseTreeNode node);
parseTreeNode deep_copy(parseTreeNode root);
#endif

