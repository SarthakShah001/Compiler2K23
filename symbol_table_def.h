#ifndef _symbol_table_def_
#define _symbol_table_def_
typedef struct SYMBOL_TABLE *symbol_table;
typedef struct AST_SYMBOL *ast_symbol;
typedef struct NORMAL_VAR normal_var;
typedef struct ARRAY_VAR array_var;
typedef struct FUNCTION_VAR function_var;
typedef union NODE_INFO node_info;
#include "lexerDef.h"
#include "hashTable.h"
#include "parseTree.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


struct SYMBOL_ELEMENT{
    ast_symbol s;
    bool is_present;
};
typedef struct SYMBOL_ELEMENT symbol_element;
typedef symbol_element s_hashtable[HASHSIZE];
// typedef struct node{
// symbol_table curr;
// symbol_table next;

// }llst;


 struct NORMAL_VAR
{
    TOK t1;
} ;
 struct ARRAY_VAR
{
    TOK t1;
    TOK lindex;
    TOK rindex;
    bool isDynamic;
} ;

struct FUNCTION_VAR
{
    TOK t1;
    parseTreeNode iplist;
    parseTreeNode opList;
};
union NODE_INFO
{
    normal_var n1;
    array_var a1;
    function_var f1;
} ;
struct AST_SYMBOL
{
    node_info ninfo;
    // lexeme l;
    ast_symbol next;
    char name[25];
    int offset;
    symbol_table arrayst[100];
};

struct SYMBOL_TABLE
{
    symbol_table parent;
    ast_symbol head;
    s_hashtable t;
};

#endif