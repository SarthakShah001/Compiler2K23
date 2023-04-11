// ek global symbol table jisme har module ke symbol table ka pointer input plidt or output plist + ek symbol table jisme jo jo mam ne dia hao
#ifndef _symbol_table_def_
#define _symbol_table_def_

#include "lexerDef.h"
#include "hashTable.h"
#include "parseTree.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct SYMBOL_TABLE* symbol_table;
typedef struct AST_SYMBOL* ast_symbol;
typedef struct Symbol_List_Node* symbol_list_node;
typedef struct MODULE* mod;
struct Symbol_List_Node{
    ast_symbol curr;
    symbol_list_node next;
};

struct MODULE{
    char mod_name[25];
    symbol_table table;
    symbol_list_node inlist;
    symbol_list_node outlist;
    bool is_declared;
    bool is_defined;
};

extern mod global_symbol_table[500];
extern int no_of_modules;
struct AST_SYMBOL{
    char var_name[25];
    int scope[2];
    tkType type;
    bool is_array;
    bool is_dynamic;
    lexeme array_range[4];
    int width;
    int offset;
    int nesting_level;
};

struct SYMBOL_TABLE{
    char mod_name[25];
    symbol_table parent;
    ast_symbol sym[1000];
    int no_child;
    symbol_table child[1000];
    mod modulewrapper;
};


// struct GLOBAL_SYMBOL_TABLE{
//     SYMBOL_TABLE
// } ; 

// struct SYMBOL_ELEMENT{
//     ast_symbol s;
//     bool is_present;
// };
// typedef struct SYMBOL_ELEMENT symbol_element;
// typedef symbol_element s_hashtable[HASHSIZE];
// // typedef struct node{
// // symbol_table curr;
// // symbol_table next;

// // }llst;


//  struct NORMAL_VAR
// {
//     TOK t1;
// } ;
//  struct ARRAY_VAR
// {
//     TOK t1;
//     TOK lindex;
//     TOK rindex;
//     bool isDynamic;
// } ;

// struct FUNCTION_VAR
// {
//     TOK t1;
//     parseTreeNode iplist;
//     parseTreeNode opList;
// };
// union NODE_INFO
// {
//     normal_var n1;
//     array_var a1;
//     function_var f1;
// } ;
// struct AST_SYMBOL
// {
//     node_info ninfo;
//     // lexeme l;
//     ast_symbol next;
//     char name[25];
//     int offset;
//     symbol_table arrayst[100];
// };

// struct SYMBOL_TABLE
// {
//     symbol_table parent;
//     ast_symbol head;
//     s_hashtable t;
// };

#endif