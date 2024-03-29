
/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int no_of_errors=0;
mod global_symbol_table[500];
bool is_same_type(tkType t1, tkType t2)
{
    if (t1 == t2)
        return true;
    else if (t1 == TK_INTEGER && t2 == TK_NUM || t2 == TK_INTEGER && t1 == TK_NUM)
        return true;
    else if (t1 == TK_REAL && t2 == TK_RNUM || t2 == TK_REAL && t1 == TK_RNUM)
        return true;
    else if (t1 == TK_BOOLEAN && (t2 == TK_TRUE || t2 == TK_FALSE) || t2 == TK_BOOLEAN && (t1 == TK_TRUE || t1 == TK_FALSE))
        return true;
    else
        return false;
}

bool is_boolean(tkType t1)
{
    if (t1 == TK_BOOLEAN || t1 == TK_TRUE || t1 == TK_FALSE)
        return true;
    else
        return false;
}

bool is_inBounds(ast_symbol curr_symbol, int n)
{
    int a, b;
    if (curr_symbol->array_range[0].value[0] == '-')
    {
        a = -1 * curr_symbol->array_range[1].integer;
    }
    else
    {
        a = curr_symbol->array_range[1].integer;
    }
    if (curr_symbol->array_range[2].value[0] == '-')
    {
        b = -1 * curr_symbol->array_range[3].integer;
    }
    else
    {
        b = curr_symbol->array_range[3].integer;
    }

    if (a > b)
        return (n >= b && n <= a);
    else
        return (n >= a && n <= b);
}

void global_symbol_table_init()
{
    for (int i = 0; i < 500; i++)
    {
        global_symbol_table[i] = (mod)malloc(sizeof(struct MODULE));
        global_symbol_table[i]->inlist=NULL;
        global_symbol_table[i]->outlist=NULL;
        global_symbol_table[i]->is_declared = false;
        global_symbol_table[i]->is_defined = false;
    }
}
int no_of_modules = 0;
int find_width(tkType t)
{
    if (t == TK_BOOLEAN)
    {
        return 1;
    }
    else if (t == TK_INTEGER)
    {
        return 2;
    }
    else
    {
        return 4;
    }
}
symbol_table symbol_table_init()
{
    symbol_table temp = (symbol_table)malloc(sizeof(struct SYMBOL_TABLE));
    temp->parent = NULL;
    for (int i = 0; i < 1000; i++)
    {
        temp->sym[i] = NULL;
    }
    for (int i = 0; i < 1000; i++)
    {
        temp->child[i] = NULL;
    }
    temp->no_child = 0;
    return temp;
}

ast_symbol ast_symbol_init()
{
    ast_symbol temp = (ast_symbol)malloc(sizeof(struct AST_SYMBOL));
    temp->is_array = false;
    temp->is_dynamic = false;
    temp->scope[0] = 0;
    temp->scope[1] = 0;
    temp->is_assigned = false;
    return temp;
}

void insert_symbol_table(symbol_table table, ast_symbol s)
{
    if (table == NULL)
    {
        // printf("table is NULL\n");
        return;
    }
    int prob = 0;
    // printf("here1\n");
    int hash_val = hash(s->var_name);
    while (table->sym[hash_val] != NULL)
    {
        prob++;
        hash_val = (hash_val + prob) % 1000;
    }
    // printf("hash val=%d\n",hash_val);
    // printf("\n");
    table->sym[hash_val] = s;
}

void add_child_symbol_table(symbol_table parent, symbol_table child)
{
    parent->child[parent->no_child] = child;
    parent->no_child++;
    child->parent = parent;
}

ast_symbol f_s(symbol_table table, char *str)
{
    int prob = 0;
    int hash_val = hash(str);
    while (table->sym[hash_val] != NULL)
    {
        if (strcmp(table->sym[hash_val]->var_name, str) == 0)
        {
            return table->sym[hash_val];
        }
        prob++;
        hash_val = (hash_val + prob) % 1000;
    }
    return NULL;
}
ast_symbol find_in_list(symbol_list_node l, char *str)
{
    if (l == NULL)
        return NULL;
    if (strcmp(l->curr->var_name, str) == 0)
        return l->curr;
    return find_in_list(l->next, str);
}
ast_symbol find_symbol(symbol_table table, char *str)
{
    if (table == NULL)
    {
        // printf("Table is NULL\n");
        return NULL;
    }
    symbol_table temp = table;
    while (temp->parent != NULL)
    {
        if (f_s(temp, str) != NULL)
        {
            return f_s(temp, str);
        }
        temp = temp->parent;
    }

    if (f_s(temp, str) != NULL)
    {
        return f_s(temp, str);
    }
    mod m = temp->modulewrapper;
    if (find_in_list(m->inlist, str) == NULL)
    {
        if (find_in_list(m->outlist, str) == NULL)
        {
            return NULL;
        }
        return find_in_list(m->outlist, str);
    }
    return find_in_list(m->inlist, str);
}
void generate_symbol_list(parseTreeNode root, symbol_list_node curr_sym, symbol_table table)
{
    if (root == NULL)
    {
        return;
    }
    if (root->tok != NULL)
    {
        if (root->tok->token_type == TK_ID)
        {
            ast_symbol s = find_symbol(table, root->tok->lex.value);
            if (s != NULL)
            {
                symbol_list_node n = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
                n->curr = s;
                curr_sym->next = n;
                curr_sym = n;
            }
        }
    }
    generate_symbol_list(root->sibling, curr_sym, table);
    generate_symbol_list(root->child, curr_sym, table);
}
void copy_array(int arr1[2], int arr2[2])
{
    arr1[0] = arr2[0];
    arr1[1] = arr2[1];
}
bool is_present_var(symbol_table table, char *str)
{
    int prob = 0;
    int hash_val = hash(str);
    while (table->child[hash_val] != NULL)
    {
        if (strcmp(table->sym[hash_val]->var_name, str) == 0)
        {
            return table->sym[hash_val];
        }
        prob++;
        hash_val = (hash_val + prob) % 1000;
    }
    return NULL;
}

int find_mod_no(char *str)
{
    for (int i = 0; i < no_of_modules; i++)
    {
        if (global_symbol_table[i]->mod_name != NULL)
        {
            if (strcmp(global_symbol_table[i]->mod_name, str) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}
int range_cal(ast_symbol curr_symbol)
{
    int len_array;
    int a, b;
    if (curr_symbol->array_range[0].value[0] == '-')
    {
        a = -1 * curr_symbol->array_range[1].integer;
    }
    else
    {
        a = curr_symbol->array_range[1].integer;
    }
    if (curr_symbol->array_range[2].value[0] == '-')
    {
        b = -1 * curr_symbol->array_range[3].integer;
    }
    else
    {
        b = curr_symbol->array_range[3].integer;
    }

    if (a > b)
        len_array = a - b + 1;
    else
        len_array = b - a + 1;
    return len_array;
}

int generate_symbol_table(parseTreeNode root, symbol_table table, int nesting, int curr_offset, ast_symbol curr_symbol, int currscope[2])
{

    // printf("Currently in %s with scope -> %d to %d\n", ast_strings[root->ast_name], root->scope[0], root->scope[1]);
    parseTreeNode currchild = root->child;
    int offset = curr_offset;
    switch (root->ast_name)
    {

    case AST_PROGRAM:
    {
        global_symbol_table_init();
        no_of_errors=0;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        currchild = currchild->sibling;
        strcpy(global_symbol_table[no_of_modules]->mod_name, "driver");
        symbol_table temp = symbol_table_init();
        strcpy(temp->mod_name, "driver");
        global_symbol_table[no_of_modules]->table = temp;
        global_symbol_table[no_of_modules]->table->modulewrapper = global_symbol_table[no_of_modules];
        no_of_modules++;
        generate_symbol_table(currchild, temp, 1, 0, curr_symbol, currchild->scope);
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        break;
    }
    case AST_MODULEDECLARATIONS:
    {
        while (currchild != NULL)
        {
            if (find_mod_no(currchild->tok->lex.value) != -1)
            {
                // throw error, change colour
                printf("\033[31m");
                printf("ERROR in line %d: module %s cannot be redeclared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
            else
            {
                strcpy(global_symbol_table[no_of_modules]->mod_name, currchild->tok->lex.value);
                global_symbol_table[no_of_modules]->is_declared = true;
                no_of_modules++;
            }
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_MODULEDEFINITIONS:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        while (currchild != NULL)
        {

            generate_symbol_table(currchild, table, 0, 0, curr_symbol, currscope);
            copy_array(currscope, temp_array);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_DRIVER:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        offset=generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        int mod_no=find_mod_no("driver");
        global_symbol_table[mod_no]->width=offset;
        copy_array(currscope, temp_array);
        break;
    }
    case AST_MODULE:
    {
        int temp_array[2];
        symbol_table temp = symbol_table_init();
        int mod_no = find_mod_no(currchild->tok->lex.value);
        currscope[0] = currchild->tok->line_no;
        copy_array(temp_array, currscope);
        if (mod_no == -1)
        {
            strcpy(global_symbol_table[no_of_modules]->mod_name, currchild->tok->lex.value);
            global_symbol_table[no_of_modules]->is_defined = true;
            global_symbol_table[no_of_modules]->table = temp;
            temp->modulewrapper = global_symbol_table[no_of_modules];
            strcpy(temp->mod_name, global_symbol_table[no_of_modules]->mod_name);
            no_of_modules++;
        }
        else
        {
            if(global_symbol_table[mod_no]->is_declared){
                printf("\033[31m");
                printf("ERROR in line %d: module %s both declared and defined before use\n", currchild->tok->line_no, global_symbol_table[mod_no]->mod_name);
                printf("\033[0m");
                no_of_errors++;
            }
            if (global_symbol_table[mod_no]->table != NULL)
            {
                // throw error change colour
                printf("\033[31m");
                printf("ERROR in line %d: module %s cannot be overloaded\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else
            {
                global_symbol_table[mod_no]->is_defined = true;
                temp->modulewrapper = global_symbol_table[mod_no];
                strcpy(temp->mod_name, global_symbol_table[mod_no]->mod_name);
                global_symbol_table[mod_no]->table = temp;
            }
        }
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 0, 0, curr_symbol, root->scope);
        copy_array(currscope, temp_array);
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 0, offset, curr_symbol, root->scope);
        copy_array(currscope, temp_array);
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 1, offset, curr_symbol, root->scope);
        copy_array(currscope, temp_array);
        mod_no = find_mod_no(root->child->tok->lex.value);
        symbol_list_node outlist = global_symbol_table[mod_no]->outlist;
        global_symbol_table[mod_no]->width = offset;
        while (outlist != NULL)
        {
            currscope[1] = outlist->curr->scope[1];
            if (!outlist->curr->is_assigned)
            {
                printf("\033[31m");
                printf("ERROR in line scope [%d-%d]: output_parameter %s not assigned\n", currscope[0], currscope[1], outlist->curr->var_name);
                printf("\033[0m");
                no_of_errors++;
            }
            outlist = outlist->next;
        }
        break;
    }
    case AST_INPUT_PARAMETER_LIST:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_OUTPUT_PARAMETER_LIST:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_INP_PARAMETER:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        ast_symbol new_symbol = ast_symbol_init();
        new_symbol->scope[0] = currscope[0];
        new_symbol->scope[1] = currscope[1];
        strcpy(new_symbol->var_name, currchild->tok->lex.value);
        if (currchild->sibling->tok != NULL)
        {
            new_symbol->type = currchild->sibling->tok->token_type;
            new_symbol->width = find_width(new_symbol->type);
            new_symbol->offset = offset;
            offset += new_symbol->width;
            new_symbol->nesting_level = 0;
        }
        else
        {

            new_symbol->is_array = true;
            new_symbol->offset = offset;
            offset = generate_symbol_table(currchild->sibling, table, 0, offset, new_symbol, currscope);
            copy_array(currscope, temp_array);
        }
        mod currmodule = table->modulewrapper;

        // printf("\ncurrmodule=%s\n",currmodule->mod_name);
        symbol_list_node child = currmodule->inlist;
        if (child == NULL)
        {
            // printf("\ninlist was null\n");
            currmodule->inlist = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
            currmodule->inlist->next = NULL;
            currmodule->inlist->curr = new_symbol;
        }
        else
        {
            while (child->next != NULL)
            {
                child = child->next;
            }
            child->next = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
            child->next->next = NULL;
            child->next->curr = new_symbol;
        }
        break;
    }
    case AST_OUT_PARAMETER:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        ast_symbol new_symbol = ast_symbol_init();
        new_symbol->scope[0] = currscope[0];
        new_symbol->scope[1] = currscope[1];
        strcpy(new_symbol->var_name, currchild->tok->lex.value);
        new_symbol->type = currchild->sibling->tok->token_type;
        new_symbol->width = find_width(new_symbol->type);
        new_symbol->offset = offset;
        offset += new_symbol->width;
        new_symbol->nesting_level = 0;
        mod currmodule = table->modulewrapper;
        symbol_list_node child = currmodule->outlist;
        if (child == NULL)
        {
            // printf("\noutlist was null\n");
            currmodule->outlist = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
            currmodule->outlist->next = NULL;
            currmodule->outlist->curr = new_symbol;
        }
        else
        {
            while (child->next != NULL)
            {
                child = child->next;
            }
            child->next = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
            child->next->next = NULL;
            child->next->curr = new_symbol;
        }
        // while (child != NULL)
        // {
        //     child = child->next;
        // }
        // child = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
        // child->next = NULL;
        // child->curr = new_symbol;
        break;
    }
    case AST_ARRAY:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        curr_symbol->type = currchild->sibling->tok->token_type;
        offset = generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        copy_array(currscope, temp_array);
        if (!curr_symbol->is_dynamic)
        {
            int len_array = range_cal(curr_symbol);
            if (nesting == 0)
            {
                curr_symbol->width = 1 + 2 * find_width(TK_INTEGER);
            }
            else
            {
                curr_symbol->width = len_array * find_width(curr_symbol->type) + 1;
            }
        }
        if (curr_symbol->is_dynamic)
        {
            curr_symbol->width = 1;
        }
        offset = offset + curr_symbol->width;
        break;
    }
    case AST_RANGE_ARRAYS:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        // generate_symbol_table(currchild,table,nesting,curr_offset,curr_symbol);
        parseTreeNode temp = currchild->child;
        if (temp->sibling == NULL)
        {
            strcpy(curr_symbol->array_range[0].value, "");
            curr_symbol->array_range[1] = temp->tok->lex;
            if (temp->tok->token_type == TK_ID)
            {
                if (find_symbol(table, temp->tok->lex.value) == NULL)
                {
                    // var_not_declared error
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is not declared\n", temp->tok->line_no, temp->tok->lex.value);
                    printf("\033[0m");
                    no_of_errors++;
                    break;
                }
                else if (find_symbol(table, temp->tok->lex.value)->type != TK_INTEGER)
                {
                    // range should be integer
                    printf("\033[31m");
                    printf("ERROR in line %d: array range must be of type INTEGER\n", temp->tok->line_no);
                    printf("\033[0m");
                    no_of_errors++;
                }
                curr_symbol->is_dynamic = true;
            }
        }
        else
        {
            curr_symbol->array_range[0] = temp->tok->lex;
            if (temp->sibling->tok->token_type = TK_ID)
            {
                if (find_symbol(table, temp->tok->lex.value) == NULL)
                {
                    // var_not_declared error
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is not declared\n", temp->tok->line_no, temp->tok->lex.value);
                    printf("\033[0m");
                    no_of_errors++;
                    break;
                }
                else if (find_symbol(table, temp->tok->lex.value)->type != TK_INTEGER)
                {
                    // range should be integer
                    printf("\033[31m");
                    printf("ERROR in line %d: array range must be of type INTEGER\n", temp->tok->line_no);
                    printf("\033[0m");
                    no_of_errors++;
                }
                curr_symbol->is_dynamic = true;
            }
            curr_symbol->array_range[1] = temp->sibling->tok->lex;
        }
        currchild = currchild->sibling;
        temp = currchild->child;
        if (temp->sibling == NULL)
        {
            strcpy(curr_symbol->array_range[2].value, "");
            curr_symbol->array_range[3] = temp->tok->lex;
            if (temp->tok->token_type == TK_ID)
            {
                if (find_symbol(table, temp->tok->lex.value) == NULL)
                {
                    // var_not_declared error
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is not declared\n", temp->tok->line_no, temp->tok->lex.value);
                    printf("\033[0m");
                    no_of_errors++;
                    break;
                }
                else if (find_symbol(table, temp->tok->lex.value)->type != TK_INTEGER)
                {
                    // range should be integer
                    printf("\033[31m");
                    printf("ERROR in line %d: array range must be of type INTEGER\n", temp->tok->line_no);
                    printf("\033[0m");
                    no_of_errors++;
                }
                curr_symbol->is_dynamic = true;
            }
        }
        else
        {
            curr_symbol->array_range[2] = temp->tok->lex;
            if (temp->sibling->tok->token_type = TK_ID)
            {
                if (find_symbol(table, temp->tok->lex.value) == NULL)
                {
                    // var_not_declared error
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is not declared\n", temp->tok->line_no, temp->tok->lex.value);
                    printf("\033[0m");
                    no_of_errors++;
                    break;
                }
                else if (find_symbol(table, temp->tok->lex.value)->type != TK_INTEGER)
                {
                    // range should be integer
                    printf("\033[31m");
                    printf("ERROR in line no %d: array range must be of type INTEGER\n", temp->tok->line_no);
                    printf("\033[0m");
                    no_of_errors++;
                }
                curr_symbol->is_dynamic = true;
            }
            curr_symbol->array_range[3] = temp->sibling->tok->lex;
        }
        break;
    }
    case AST_STATEMENTS:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_GET_VALUE:
    {

        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            printf("\033[31m");
            printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
        }
        break;
    }
    case AST_PRINT:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        if (currchild->tok == NULL)
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
        }
        else if (currchild->tok->token_type == TK_ID)
        {
            if (find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // throw error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
        }
        break;
    }
    case AST_ARRAY_ACCESS:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        // //     // id->sign(canbeNULL)->numberorid

        // //     // array also be declared
        // //     // array index should be integer and in range
        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            printf("\033[31m");
            printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        else
        {
            ast_symbol t = find_symbol(table, currchild->tok->lex.value);
            if (!t->is_array)
            {
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not of type ARRAY\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            currchild = currchild->sibling->child;
            // if(currchild==NULL){
            //         printf("currchild is NULL\n");
            //     }
            if (currchild->sibling == NULL)
            {
                if (currchild == NULL)
                {
                    printf("currchild is NULL\n");
                }
                // printf("tok=%s\n", terminal_str[currchild->tok->token_type]);
                if (currchild->tok->token_type == TK_NUM)
                {
                    if (t->is_dynamic)
                    {
                        break;
                    }
                    else
                    {
                        int a, b;
                        if (t->array_range[0].value[0] == '-')
                        {
                            a = -1 * t->array_range[1].integer;
                        }
                        else
                        {
                            a = t->array_range[1].integer;
                        }
                        if (t->array_range[2].value[0] == '-')
                        {
                            b = -1 * t->array_range[3].integer;
                        }
                        else
                        {
                            b = t->array_range[3].integer;
                        }
                        if (a > b)
                        {
                            if (currchild->tok->lex.integer > a || currchild->tok->lex.integer < b)
                            {
                                // throw error
                                printf("\033[31m");
                                printf("ERROR in line %d: index of Array %s is out of bounds\n", root->child->tok->line_no, root->child->tok->lex.value);
                                printf("\033[0m");
                                no_of_errors++;
                            }
                        }
                        else
                        {
                            if (currchild->tok->lex.integer < a || currchild->tok->lex.integer > b)
                            {
                                // throw error
                                printf("\033[31m");
                                printf("ERROR in line %d: index of Array %s is out of bounds\n", root->child->tok->line_no, root->child->tok->lex.value);
                                printf("\033[0m");
                                no_of_errors++;
                            }
                        }
                    }
                }
                else if (currchild->tok->token_type == TK_ID)
                {
                    ast_symbol t = find_symbol(table, currchild->tok->lex.value);
                    if (t == NULL)
                    {
                        // throw error var not declared
                        printf("\033[31m");
                        printf("ERROR in line %d: variable %s is not declared\n", root->child->tok->line_no, root->child->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                    }
                    else if (t->is_array)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR in line %d: array variable %s is not allowed as index of an array\n", root->child->tok->line_no, root->child->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                    }
                    else if (t->type != TK_NUM)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR in line %d: array index %s should be of type INTEGER\n", root->child->tok->line_no, root->child->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                    }
                }
                else
                {
                    // throw error
                    printf("\033[31m");
                    printf("ERROR in line %d: array index %s should be of type INTEGER\n", root->child->tok->line_no, root->child->tok->lex.value);
                    printf("\033[0m");
                    no_of_errors++;
                }
            }
            else
            {
                if (currchild->sibling->tok->token_type == TK_NUM)
                {
                    if (t->is_dynamic)
                    {
                        break;
                    }
                    else
                    {
                        int a, b;
                        int x = currchild->sibling->tok->lex.integer;
                        if (currchild->tok->lex.value[0] == '-')
                        {
                            x = x * -1;
                        }
                        if (t->array_range[0].value[0] == '-')
                        {
                            a = -1 * t->array_range[1].integer;
                        }
                        else
                        {
                            a = t->array_range[1].integer;
                        }
                        if (t->array_range[2].value[0] == '-')
                        {
                            b = -1 * t->array_range[3].integer;
                        }
                        else
                        {
                            b = t->array_range[3].integer;
                        }
                        if (a > b)
                        {
                            if (x > a || x < b)
                            {
                                // throw error
                                printf("\033[31m");
                                printf("ERROR in line %d: index of array %s is out of bounds\n", root->child->tok->line_no, root->child->tok->lex.value);
                                printf("\033[0m");
                                no_of_errors++;
                            }
                        }
                        else
                        {
                            if (x < a || x > b)
                            {
                                // throw error
                                printf("\033[31m");
                                printf("ERROR in line %d: index of array %s is out of bounds\n", root->child->tok->line_no, root->child->tok->lex.value);
                                printf("\033[0m");
                                no_of_errors++;
                            }
                        }
                    }
                }
                else if (currchild->sibling->tok->token_type == TK_ID)
                {
                    ast_symbol t = find_symbol(table, currchild->sibling->tok->lex.value);
                    if (t == NULL)
                    {
                        // throw error var not declared
                        printf("\033[31m");
                        printf("ERROR in line %d: variable %s is not declared\n", root->child->tok->line_no, root->child->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                    }
                    else if (t->is_array)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR in line %d: array variable %s is not allowed as index\n", root->child->tok->line_no, root->child->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                    }
                    else if (t->type != TK_NUM)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR in line %d: array index %s is not of type INTEGER\n", root->child->tok->line_no, root->child->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                    }
                }
                else
                {
                    // throw error
                    printf("\033[31m");
                    printf("ERROR in line %d: array index %s is not of type INTEGER\n", root->child->tok->line_no, root->child->tok->lex.value);
                    printf("\033[0m");
                    no_of_errors++;
                }
            }
        }
        //     // variable should be declared and is int
        break;
    }
    case AST_ID_ASSIGN:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        // if (currchild == NULL)
        // {
        //     printf("currchild is NULL\n");
        //     break;
        // }
        // else if (currchild->tok == NULL)
        // {
        //     printf("tok is NULL\n");
        //     printf("%s\n", ast_strings[currchild->ast_name]);
        //     break;
        // }
        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            printf("\033[31m");
            printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        find_symbol(table, currchild->tok->lex.value)->is_assigned = true;
        currchild = currchild->sibling;
        if (currchild->tok != NULL)
        {
            ast_symbol t1;
            if (currchild->tok->token_type == TK_ID)
            {
                t1 = find_symbol(table, currchild->tok->lex.value);
            }
            ast_symbol t2 = find_symbol(table, root->child->tok->lex.value);
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->type != find_symbol(table, root->child->tok->lex.value)->type))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: Type mismatch error\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
            else if (currchild->tok->token_type == TK_ID && t1->is_array && t2->is_array)
            {
                int a = range_cal(t1);
                int b = range_cal(t2);
                if (a != b)
                {
                    printf("\033[31m");
                    printf("ERROR in line %d: arrays are structurally inequivalent\n", currchild->tok->line_no);
                    printf("\033[0m");
                    no_of_errors++;
                }
            }
            else if (currchild->tok->token_type == TK_ID && (t1->is_array || t2->is_array))
            {
                printf("\033[31m");
                printf("ERROR in line %d: Type mismatch error\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
            else if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value)->is_array)
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: Type mismatch error\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
            else if (currchild->tok->token_type != TK_ID && !is_same_type(find_symbol(table, root->child->tok->lex.value)->type, currchild->tok->token_type))
            {
                // type not matching error
                printf("\033[31m");
                printf("ERROR in line %d: Mismatch type assign error\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
        }
        else
        {
            if (find_symbol(table, root->child->tok->lex.value)->is_array)
            {
                printf("\033[31m");
                printf("ERROR in line %d: array can be assigned only with an array\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
            if (currchild->type_syn == epsilon)
            {
                break;
            }
            if (!is_same_type(find_symbol(table, root->child->tok->lex.value)->type, currchild->type_syn))
            {
                printf("\033[31m");
                printf("ERROR in line %d: Mismatch type assign error\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
        }
        // do nothing
        // all variable calculation at rhs should be compatible to each other
        // end result type of rhs compatible to lhs
        break;
    }
    case AST_ARRAY_ASSIGN:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        // if(currchild==NULL){
        //     printf("currchild is NULL\n");
        //     break;
        // }
        // else if(currchild->tok==NULL){
        //     printf("tok is NULL\n");
        //     printf("%s\n",ast_strings[currchild->ast_name]);
        //     break;
        // }
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        copy_array(currscope, temp_array);
        currchild = currchild->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
        }
        else
        {
            
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
        }
        // do nothing
        // index should be integer and in range
        // type of rhs same as array element type
        break;
    }
    case AST_INDEX_ARR:
    {
        // do nothing
        break;
    }
    case AST_MODULE_REUSE:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        if (find_mod_no(currchild->tok->lex.value) == -1)
        {
            // throw error change colour
            printf("\033[31m");
            printf("ERROR in line %d: Module %s needs to be declared first before use\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        int i = find_mod_no(currchild->tok->lex.value);
        symbol_table t = table;
        while (t->parent != NULL)
        {
            t = t->parent;
        }
        if (strcmp(t->modulewrapper->mod_name, global_symbol_table[i]->mod_name) == 0)
        {
            // recursion not allowed error
            printf("\033[31m");
            printf("ERROR in line %d: Recursion is not allowed\n", currchild->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        if (global_symbol_table[i]->is_declared && !global_symbol_table[i]->is_defined)
        {
            global_symbol_table[i]->is_declared = false;
        }
        if (global_symbol_table[i]->is_declared && global_symbol_table[i]->is_defined)
        {
            // both declaration and definition
            printf("\033[31m");
            printf("ERROR in line %d: module %s both declared and defined before use\n", currchild->tok->line_no, global_symbol_table[i]->mod_name);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        copy_array(currscope, temp_array);
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        copy_array(currscope, temp_array);
        break;
    }
    case AST_PARAMETER_LIST1:
    {
        while (currchild != NULL)
        {
            if (find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
            else
            {
                find_symbol(table, currchild->tok->lex.value)->is_assigned = true;
            }
            currchild = currchild->sibling;
        }
        // do nothing
        break;
    }
    case AST_PARAMETER_LIST2:
    {

        int temp_array[2];
        copy_array(temp_array, currscope);
        while (currchild != NULL)
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
            currchild = currchild->sibling;
        }
        // do nothing
        break;
    }
    case AST_ACTUAL_PARA:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        if (currchild->tok != NULL && (currchild->tok->token_type == TK_PLUS || currchild->tok->token_type == TK_MINUS))
        {
            currchild = currchild->sibling;
        }
        if (currchild->tok != NULL && (currchild->tok->token_type == TK_ID) && (find_symbol(table, currchild->tok->lex.value) == NULL))
        {
            printf("\033[31m");
            printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
        }
        else if (currchild->tok == NULL)
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
        }
        // do nothing
        break;
    }
    case AST_UNARYEXPR:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        currchild = currchild->sibling;
        if (currchild->tok == NULL)
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope, temp_array);
                root->type_syn = currchild->type_syn;
        }
        else
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // throw not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
            else if (currchild->tok->token_type == TK_ID)
            {
                root->type_syn = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                root->type_syn = currchild->tok->token_type;
            }
        }
        // do nothing
        break;
    }
    case AST_RELATIONAL_OP:
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: Type mismatch error\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t1 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t1 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: Type mismatch error\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t2 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t2 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR in line %d: Type mismatch error\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        root->type_syn = TK_BOOLEAN;
        // do nothing
        break;
    }
    case AST_LOGICAL_OP:
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: Type mismatch error\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t1 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t1 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: Type mismatch error\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t2 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t2 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || !is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR in line %d: Type mismatch error\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        root->type_syn = TK_BOOLEAN;
        // do nothing
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        // currchild = currchild->sibling->sibling;
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        // // do nothing
        break;
    }
    case AST_PLUS:
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t1 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t1 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t2 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t2 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }
        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR in line %d: Type mismatch error\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        root->type_syn = t1;
        // do nothing
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        // currchild = currchild->sibling->sibling;
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        // // do nothing
        break;
    }
    case AST_MINUS:
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR  in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t1 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t1 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t2 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t2 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR in line %d: Type mismatch error\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        root->type_syn = t1;
        // do nothing
        //     if (currchild->tok != NULL)
        //     {
        //         if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //         {
        //             // var not declared error
        //             printf("\033[31m");
        //             printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //             printf("\033[0m");
        //         }
        //     }
        //     else
        //     {
        //         generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        //     }
        //     currchild = currchild->sibling->sibling;
        //     if (currchild->tok != NULL)
        //     {
        //         if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //         {
        //             // var not declared error
        //             printf("\033[31m");
        //             printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //             printf("\033[0m");
        //         }
        //     }
        //     else
        //     {
        //         generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        //     }
        // do nothing
        break;
    }
    case AST_MUL:
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t1 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t1 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t2 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t2 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR in line %d: Type mismatch error\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        root->type_syn = t1;
        // do nothing
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        // currchild = currchild->sibling->sibling;
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        //  do nothing
        break;
    }
    case AST_DIV:
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t1 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t1 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value) == NULL))
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->is_array))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR in line %d: array variable cannot be in arithmetic operation\n", root->child->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
            if (currchild->tok->token_type == TK_ID)
            {
                t2 = find_symbol(table, currchild->tok->lex.value)->type;
            }
            else
            {
                t2 = currchild->tok->token_type;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR in line %d: Type mismatch error\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        root->type_syn = TK_REAL;
        // do nothing
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        // currchild = currchild->sibling->sibling;
        // if (currchild->tok != NULL)
        // {
        //     if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
        //     {
        //         // var not declared error
        //         printf("\033[31m");
        //         printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
        //         printf("\033[0m");
        //     }
        // }
        // else
        // {
        //     generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        // }
        // do nothing
        break;
    }
    case AST_ARRAY_FACTOR:
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        ast_symbol t = find_symbol(table, currchild->tok->lex.value);
        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            printf("\033[31m");
            printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        else if (!find_symbol(table, currchild->tok->lex.value)->is_array)
        {
            printf("\033[31m");
            printf("ERROR in line %d: variable %s is not of type ARRAY\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        if(root->parent->ast_name==AST_ARRAY_ASSIGN){
        t->is_assigned = true;
        }
        currchild = currchild->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->lex.value[0] == '+' || currchild->tok->lex.value[0] == '-')
            {
                currchild = currchild->sibling;
            }
            if (currchild->tok != NULL)
            {
                if (currchild->tok->token_type == TK_ID)
                {
                    if (find_symbol(table, currchild->tok->lex.value) == NULL)
                    {
                        // var not declared error
                        printf("\033[31m");
                        printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                        break;
                    }
                    else if (find_symbol(table, currchild->tok->lex.value)->type != TK_INTEGER)
                    {
                        // var should be integer
                        printf("\033[31m");
                        printf("ERROR in line %d: variable %s should be of type INTEGER\n", currchild->tok->line_no, currchild->tok->lex.value);
                        printf("\033[0m");
                        no_of_errors++;
                        break;
                    }
                }
                else if (currchild->tok->token_type != TK_NUM)
                {
                    // only integer access possible in array
                    printf("\033[31m");
                    printf("ERROR in line %d: array index should be type INTEGER\n", currchild->tok->line_no);
                    printf("\033[0m");
                    no_of_errors++;
                    break;
                }
                else
                {
                    if (!is_inBounds(t, currchild->tok->lex.integer))
                    {
                        printf("\033[31m");
                        printf("ERROR in line %d: array index out of bounds\n", currchild->tok->line_no);
                        printf("\033[0m");
                        no_of_errors++;
                        break;
                    }
                }
            }
            else
            {
                generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
                copy_array(currscope,temp_array);
                if (currchild->type_syn != TK_NUM && currchild->type_syn != TK_INTEGER)
                {
                    // throw error
                    printf("\033[31m");
                    printf("ERROR in line %d: array index variable %s should be of type INTEGER\n", currchild->tok->line_no, currchild->tok->lex.value);
                    printf("\033[0m");
                    no_of_errors++;
                    break;
                }
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            copy_array(currscope,temp_array);
            if (currchild->type_syn != TK_NUM && currchild->type_syn != TK_INTEGER)
            {
                // throw error
                printf("\033[31m");
                printf("ERROR in line %d: array index variable %s should be of type INTEGER\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
        }
        root->type_syn = t->type;
        // do nothing
        break;
    }
    case AST_RANGE_FOR_LOOP:
    {

        // do nothing
        break;
    }
    case AST_DECLARE_STMT:
    
    {
        int temp_array[2];
        copy_array(temp_array,currscope);
        if (currchild->tok != NULL)
        {
            // non-array declarations
            tkType t = currchild->tok->token_type;
            currchild = currchild->sibling;
            while (currchild != NULL)
            {
                ast_symbol new_symbol = ast_symbol_init();
                new_symbol->nesting_level = nesting;
                if (find_symbol(table, currchild->tok->lex.value) != NULL && find_symbol(table, currchild->tok->lex.value)->nesting_level == nesting)
                {
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is redeclared\n", currchild->tok->line_no, currchild->tok->lex.value);
                    printf("\033[0m");
                    currchild = currchild->sibling;
                    no_of_errors++;
                    continue;
                }
                if (nesting == 1 && find_in_list(table->modulewrapper->outlist, currchild->tok->lex.value) != NULL)
                {
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is already declared in output parameter list and cannot be redeclared\n", currchild->tok->line_no, currchild->tok->lex.value);
                    printf("\033[0m");
                    currchild = currchild->sibling;
                    no_of_errors++;
                    continue;
                }
                strcpy(new_symbol->var_name, currchild->tok->lex.value);
                new_symbol->offset = offset;
                new_symbol->width = find_width(t);
                offset += new_symbol->width;
                new_symbol->type = t;
                new_symbol->scope[0] = currscope[0];
                new_symbol->scope[1] = currscope[1];
                insert_symbol_table(table, new_symbol);
                currchild = currchild->sibling;
            }
        }
        else
        {
            currchild = currchild->sibling;
            while (currchild != NULL)
            {
                int temp_array[2];
                copy_array(temp_array, currscope);
                ast_symbol new_symbol = ast_symbol_init();
                new_symbol->nesting_level = nesting;
                if (find_symbol(table, currchild->tok->lex.value) != NULL && find_symbol(table, currchild->tok->lex.value)->nesting_level == nesting)
                {
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is redeclared\n", currchild->tok->line_no, currchild->tok->lex.value);
                    printf("\033[0m");
                    currchild = currchild->sibling;
                    no_of_errors++;
                    continue;
                }
                if (nesting == 1 && find_in_list(table->modulewrapper->outlist, currchild->tok->lex.value) != NULL)
                {
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %s is declared in output parameter list and cannot be redeclared\n", currchild->tok->line_no, currchild->tok->lex.value);
                    printf("\033[0m");
                    currchild = currchild->sibling;
                    no_of_errors++;
                    continue;
                }
                strcpy(new_symbol->var_name, currchild->tok->lex.value);
                new_symbol->offset = offset;
                new_symbol->is_array = true;
                offset = generate_symbol_table(root->child, table, nesting, offset, new_symbol, currscope);
                copy_array(currscope, temp_array);
                new_symbol->scope[0] = currscope[0];
                new_symbol->scope[1] = currscope[1];
                insert_symbol_table(table, new_symbol);
                currchild = currchild->sibling;
            }
        }
        break;
    }
    case AST_SWITCH:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        ast_symbol t = find_symbol(table, currchild->tok->lex.value);
        currscope[0] = currchild->tok->line_no;
        currscope[1] = root->scope[1];
        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            // var not declared error
            printf("\033[31m");
            printf("ERROR in line %d: variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        else
        {
            if (t->type != TK_INTEGER && t->type != TK_BOOLEAN)
            {
                // switch variable should be integer or boolean
                printf("\033[31m");
                printf("ERROR in line %d: switch variable %s should be of type INTEGER or BOOLEAN\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
           
        }
        currchild = currchild->sibling;
        currchild->type_inh = t->type;
        offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
        currchild = currchild->sibling;
        if (t->type == TK_INTEGER)
        {
            if (currchild != NULL)
            {
                generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);

                copy_array(currscope, temp_array);
            }
            else
            {
                // throw error no default
                printf("\033[31m");
                printf("ERROR in scope[%d-%d] : default case absent for switch case\n",currscope[0],currscope[1]);
                printf("\033[0m");
                no_of_errors++;
            }
        }
        else
        {
            if (currchild != NULL)
            {
                // throw error default should not be there
                printf("\033[31m");
                printf("ERROR in scope[%d-%d]: default case should not be present \n", currscope[0], currscope[1]);
                printf("\033[0m");
                no_of_errors++;
            }
        }
        copy_array(currscope, temp_array);
        break;
    }
    case AST_CASES:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        while (currchild != NULL)
        {
            symbol_table new_table = symbol_table_init();
            new_table->parent = table;
            strcpy(new_table->mod_name, table->mod_name);
            table->child[table->no_child] = new_table;
            table->no_child++;
            currchild->type_inh = root->type_inh;
            offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
            copy_array(currscope, temp_array);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_CASE:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        if (is_boolean(currchild->tok->token_type) && (root->type_inh != TK_BOOLEAN))
        {
            // type mismatch error
            printf("\033[31m");
            printf("ERROR in line %d: type mismatch b/w switch and case variable\n", currchild->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        else if (!is_boolean(currchild->tok->token_type) && (root->type_inh != TK_INTEGER))
        {
            // type mismatch error
            printf("\033[31m");
            printf("ERROR in line %d: type mismatch b/w switch and case variable\n", currchild->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, table, nesting, offset, NULL, root->scope);
        copy_array(currscope, temp_array);
        break;
    }
    case AST_FORLOOP:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            // var not declared error
            printf("\033[31m");
            printf("ERROR in line %d: for_loop variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        else if (find_symbol(table, currchild->tok->lex.value)->type != TK_INTEGER)
        {
            // for loop variable should be integer
            // throw error
            printf("\033[31m");
            printf("ERROR in line %d: for_loop variable %s is not declared\n", currchild->tok->line_no, currchild->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
            break;
        }
        ast_symbol t = find_symbol(table, currchild->tok->lex.value);
        bool b = t->is_assigned;
        t->is_assigned = false;
        currscope[0] = currchild->tok->line_no;
        currscope[1] = root->scope[1];
        currchild = currchild->sibling;
        // generate_symbol_table(currchild,table,nesting,offset,curr_symbol,currscope);
        currchild = currchild->sibling;
        symbol_table new_table = symbol_table_init();
        new_table->parent = table;
        strcpy(new_table->mod_name, table->mod_name);
        table->child[table->no_child] = new_table;
        table->no_child++;
        offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
        if (t->is_assigned)
        {
            // for loop var is assigned
            printf("\033[31m");
            printf("ERROR in scope [%d-%d]: for_loop variable %s is being assigned\n", currscope[0], currscope[1], root->child->tok->lex.value);
            printf("\033[0m");
            no_of_errors++;
        }
        t->is_assigned = b;
        copy_array(currscope, temp_array);
        break;
    }
    case AST_WHILELOOP:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        symbol_list_node l = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type != TK_ID&&currchild->tok->token_type != TK_TRUE&&currchild->tok->token_type != TK_FALSE)
            {
                // while loop expression should not be a constant
                printf("\033[31m");
                printf("ERROR in line %d: while_loop variable cannot be a constant non boolean\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
            else if (find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var_not_declared error
                printf("\033[31m");
                printf("ERROR in line %d: while_loop variable is not declared\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
                break;
            }
            else if (find_symbol(table, currchild->tok->lex.value)->type != TK_BOOLEAN)
            {
                // while loop expression should be of boolean type
                printf("\033[31m");
                printf("ERROR in line %d: while_loop variable should be of type BOOLEAN\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
            if(currchild->type_syn==epsilon){
                break;
            }
            if (currchild->type_syn != TK_BOOLEAN)
            {
                // while loop expression should be of boolean type
                printf("\033[31m");
                printf("ERROR in line %d: while_loop variable should be of type BOOLEAN\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
            generate_symbol_list(currchild, l, table);
        }
        currchild = currchild->sibling;
        symbol_table new_table = symbol_table_init();
        new_table->parent = table;
        strcpy(new_table->mod_name, table->mod_name);
        table->child[table->no_child] = new_table;
        table->no_child++;
        symbol_list_node temp = l;
        temp = temp->next;
        bool arr[1000];
        for (int i = 0; i < 1000; i++)
        {
            if (temp != NULL)
            {
                arr[i] = temp->curr->is_assigned;
                temp->curr->is_assigned = false;
                temp = temp->next;
            }
            else
            {
                arr[i] = false;
            }
        }
        temp = l->next;
        while (temp != NULL)
        {
            // printf("%s ",temp->curr->var_name);
            // printf("\n");
            temp->curr->is_assigned=false;
            temp = temp->next;
        }
        offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
        temp = l->next;
        bool var_assigned = false;
        while (temp != NULL)
        {
            // printf("%s ",temp->curr->var_name);
            // printf("\n");
            if (temp->curr->is_assigned)
            {
            var_assigned = true;
            }
            temp = temp->next;
        }
        if (!var_assigned)
        {
            // while loop var not assigned
            printf("\033[31m");
            printf("ERROR in scope[%d-%d]: while_loop variable is not assigned\n", root->scope[0], root->scope[1]);
            printf("\033[0m");
            no_of_errors++;
        }
        temp = l->next;
        int i = 0;
        while (temp != NULL)
        {
            temp->curr->is_assigned = arr[i];
            i++;
            temp = temp->next;
        }
            copy_array(currscope, temp_array);
        break;
    }
    case AST_INDEX_FOR_LOOP:
    {
        // do nothing
        break;
    }
    case AST_DEFAULT:
    {
        int temp_array[2];
        copy_array(temp_array, currscope);
        symbol_table new_table = symbol_table_init();
        new_table->parent = table;
        strcpy(new_table->mod_name, table->mod_name);
        table->child[table->no_child] = new_table;
        table->no_child++;
        offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
        copy_array(currscope, temp_array);
        break;
    }
    default:
    {
        break;
    }
    }
    return offset;
}
int pass2(parseTreeNode root, int mod_no, symbol_table st, int child_count)
{
    // printf("Currently in %s with scope [%d-%d]\n", ast_strings[root->ast_name],root->scope[0],root->scope[1]);
    parseTreeNode currchild = root->child;
    switch (root->ast_name)
    {

    case AST_PROGRAM:
    {
        while (currchild != NULL)
        {
            pass2(currchild, mod_no, st, child_count);
            currchild = currchild->sibling;
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
        while (currchild != NULL)
        {
            pass2(currchild, mod_no, st, child_count);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_DRIVER:
    {
        int i = find_mod_no("driver");
        mod m = global_symbol_table[i];
        pass2(currchild, i, m->table, 0);
        break;
    }
    case AST_MODULE:
    {
        int t = find_mod_no(currchild->tok->lex.value);
        mod m = global_symbol_table[t];

        while (currchild->sibling != NULL)
        {
            currchild = currchild->sibling;
        }

        pass2(currchild, t, m->table, 0);

        break;
    }
    case AST_INPUT_PARAMETER_LIST:
    {
        // do nothing
        break;
    }
    case AST_OUTPUT_PARAMETER_LIST:
    {
        // do nothing
        break;
    }
    case AST_INP_PARAMETER:
    {
        // do nothing
        break;
    }
    case AST_OUT_PARAMETER:
    {
        // do nothing
        break;
    }
    case AST_ARRAY:
    {
        // do nothing

        break;
    }
    case AST_RANGE_ARRAYS:
    {
        // do nothing

        break;
    }
    case AST_STATEMENTS:
    {
        while (currchild != NULL)
        {
            child_count= pass2(currchild, mod_no, st, child_count);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_GET_VALUE:
    {

        break;
    }
    case AST_PRINT:
    {

        break;
    }
    case AST_ARRAY_ACCESS:
    {

        // variable should be declared and is int
        break;
    }
    case AST_ID_ASSIGN:
    {

        // do nothing
        // all variable calculation at rhs should be compatible to each other
        // end result type of rhs compatible to lhs
        break;
    }
    case AST_ARRAY_ASSIGN:
    {

        break;
    }
    case AST_INDEX_ARR:
    {
        // do nothing
        break;
    }
    case AST_MODULE_REUSE:
    {
        int i = find_mod_no(currchild->tok->lex.value);
        currchild = currchild->sibling;
        pass2(currchild, i, st, child_count);
        currchild = currchild->sibling;
        pass2(currchild,i, st, child_count);
        break;
    }
    case AST_PARAMETER_LIST1:
    {
        
        // printf("mod_name=%s\n", global_symbol_table[mod_no]->mod_name);
    //    int m= find_mod_no(root->parent->child->tok->lex.value);
        symbol_list_node outlist = global_symbol_table[mod_no]->outlist;
        while (currchild != NULL && outlist != NULL)
        {
            if(currchild->tok->token_type!=TK_ID){
            // output parameter should be a variable
                printf("\033[31m");
                printf("ERROR in line %d: only variable can be assigned\n", currchild->tok->line_no);
                printf("\033[0m");
                no_of_errors++;
            }
            ast_symbol a = find_symbol(st, currchild->tok->lex.value);
            if (a == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared in output parameters\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
            else if (a->type != outlist->curr->type)
            {
                // type mismatch error outlist
                printf("\033[31m");
                printf("ERROR in line %d: variable %s does not match with outlist parameter type\n", currchild->tok->line_no, currchild->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
            else{
                if(a->is_array){
                // array can't be assigned a non-array var
                    printf("\033[31m");
                    printf("ERROR in line %d: array %s can't be assigned non-array var fron return list \n",currchild->tok->line_no,a->var_name); 
                    printf("\033[0m");
                    no_of_errors++;
                }
            
            }
            
            currchild = currchild->sibling;
            outlist = outlist->next;
        }
        if (currchild != NULL || outlist != NULL)
        {
            // // no of parameters don't match error
            // if(currchild!=NULL){
            // // printf("currchild not empty\n");
            // }
            // else{
            // // printf("outlist not empty\n");
            // // printf("%s\n", outlist->curr->var_name);
            
            // }
            printf("\033[31m");
            printf("ERROR in line %d: no. of variables do not match with outlist parameters\n", root->parent->child->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
        }

        break;
    }
    case AST_PARAMETER_LIST2:
    {
        symbol_list_node inlist = global_symbol_table[mod_no]->inlist;
        // printf("mod_name=%s\n", global_symbol_table[mod_no]->mod_name);
        while (currchild != NULL && inlist != NULL)
        {
            parseTreeNode t=currchild->child;
            if(t->sibling!=NULL){
                t=t->sibling;
            }
            if(t->tok->token_type==TK_ID){
            ast_symbol a = find_symbol(st, t->tok->lex.value);
            if (a == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR in line %d: variable %s is not declared in input parameters\n", t->tok->line_no, t->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
            else if (a->type != inlist->curr->type)
            {
                // type mismatch error inlist
                printf("\033[31m");
                printf("ERROR in line %d: variable %s does not match with inlist parameter type\n", t->tok->line_no, t->tok->lex.value);
                printf("\033[0m");
                no_of_errors++;
            }
            else {
                if(a->is_array&&inlist->curr->is_array){
                    int t1= range_cal(a);
                    int t2= range_cal(inlist->curr);
                    if(t1!=t2){
                    // arrays are structurally inequivalent
                        printf("\033[31m");
                        printf("ERROR in line %d: array %s and %s are structurally inequivalent\n", t->tok->line_no, t->tok->lex.value,inlist->curr->var_name);
                        printf("\033[0m");
                        no_of_errors++;
                    }
                
                }
                else if(a->is_array||inlist->curr->is_array){
                if(a->is_array){
                // array can't be assigned to non-array var
                        printf("\033[31m");
                        printf("ERROR in line %d: array %s cannot be assigned to non-array %s variable \n",t->tok->line_no,a->var_name,inlist->curr->var_name); 
                        printf("\033[0m");
                        no_of_errors++;
                
                }
                else{
                    // non array var can't be assigned to array
                        printf("\033[31m");
                        printf("ERROR in line %d: non-array variable %s cannot be assigned to array %s\n", t->tok->line_no,a->var_name,inlist->curr->var_name );
                        printf("\033[0m");
                        no_of_errors++;
                }
                }
            }
            }
            else{
                if(is_same_type(t->tok->token_type, inlist->curr->type)){
                    if(t->tok->token_type ==TK_NUM){
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %lld does not match with inlist parameter type\n", t->tok->line_no, t->tok->lex.integer);
                    printf("\033[0m");
                    }
                    if(t->tok->token_type ==TK_RNUM){
                    printf("\033[31m");
                    printf("ERROR in line %d: variable %lf does not match with inlist parameter type\n", t->tok->line_no, t->tok->lex.decimal);
                    printf("\033[0m");
                    }
                }
                
            }
            currchild = currchild->sibling;
            inlist = inlist->next;
        }
        if (currchild != NULL || inlist != NULL)
        {
            // no of parameters don't match error
            printf("\033[31m");
            printf("ERROR in line %d: no. of variables do not match with inlist parameters\n", root->parent->child->tok->line_no);
            printf("\033[0m");
            no_of_errors++;
        }
        // do nothing
        break;
    }
    case AST_ACTUAL_PARA:
    {
        // do nothing
        break;
    }
    case AST_UNARYEXPR:
    {
        // do nothing
        break;
    }
    case AST_RELATIONAL_OP:
    {

        // do nothing
        break;
    }
    case AST_LOGICAL_OP:
    {
        // do nothing
        break;
    }
    case AST_PLUS:
    {
        // do nothing
        break;
    }
    case AST_MINUS:
    {
        // do nothing
        break;
    }
    case AST_MUL:
    {
        //  do nothing
        break;
    }
    case AST_DIV:
    {
        // do nothing
        break;
    }
    case AST_ARRAY_FACTOR:
    {
        // do nothing
        break;
    }
    case AST_RANGE_FOR_LOOP:
    {
        // do nothing
        break;
    }
    case AST_DECLARE_STMT:
    {

        break;
    }
    case AST_SWITCH:
    {
        ast_symbol t = find_symbol(st, currchild->tok->lex.value);
        if (find_symbol(st, currchild->tok->lex.value) == NULL)
        {
            
            break;
        }
        else
        {
            if (t->type != TK_INTEGER && t->type != TK_BOOLEAN)
            {
               
                break;
            }
            
            
        }
        currchild = currchild->sibling;
        while (currchild != NULL)
        {
            child_count= pass2(currchild, mod_no, st, child_count);
            currchild = currchild->sibling;
        }
        child_count++;
        break;
    }
    case AST_CASES:
    {
        while (currchild != NULL)
        {
            // printf("childcount=%d\n", child_count);
            pass2(currchild, mod_no, st->child[child_count], child_count);
            child_count++;
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_CASE:
    {
        currchild = currchild->sibling;
        pass2(currchild, mod_no,st,0);
        break;
    }
    case AST_FORLOOP:
    {
        currchild= currchild->sibling->sibling;
        pass2(currchild, mod_no,st->child[child_count],0);
        child_count++;
        break;
    }
    case AST_WHILELOOP:
    {
        currchild=currchild->sibling;
        pass2(currchild, mod_no,st->child[child_count],0);
        child_count++;
        break;
    }
    case AST_INDEX_FOR_LOOP:
    {
        // do nothing
        break;
    }
    case AST_DEFAULT:
    {
        pass2(currchild, mod_no,st->child[child_count],0);
        break;
    }
    default:
    {
        break;
    }
    }
    return child_count;
}

void print_symbol_table_list(mod sym_module, symbol_list_node in)
{
    // if (in == NULL) printf("\nList is NULL\n");

    while (in != NULL)
    {
        // print variables
        if (in->curr == NULL)
        {
            // printf("curr was null\n");
            continue;
        }
        printf("%-20s", in->curr->var_name);
        // printf("%-25s", sym_module->table->mod_name);
        printf("%-20s", sym_module->table->mod_name);
        printf("   %3d-%-3d     ", in->curr->scope[0], in->curr->scope[1]);
        printf("   %-15s", terminal_str[in->curr->type]);
        if (in->curr->is_array)
        {
            printf("   YES    ");
            if (in->curr->is_dynamic)
            {
                printf("     YES     ");
                printf("    %s%s..%s%s   ", in->curr->array_range[0].value, in->curr->array_range[1].value, in->curr->array_range[2].value, in->curr->array_range[3].value);
            }
            else
            {
                printf("     NO    ");
                printf("   %s%3lld..%s%-3lld   ", in->curr->array_range[0].value, in->curr->array_range[1].integer, in->curr->array_range[2].value, in->curr->array_range[3].integer);
            }
        }
        else
        {
            printf("    NO    ");
            printf("     **     ");
            printf("     **      ");
        }

        printf("   %4d   ", in->curr->width);
        printf("   %5d   ", in->curr->offset);
        printf("    %5d  ", in->curr->nesting_level);
        printf("\n");
        in = in->next;
    }
    return;
}

void print_symbol_table(symbol_table tab)
{
    for (int i = 0; i < 1000; i++)
    {
        if (tab->sym[i] != NULL)
        {
            // print symbols
            printf("%-20s", tab->sym[i]->var_name);
            printf("%-20s", tab->mod_name);
            printf("   %3d-%-3d     ", tab->sym[i]->scope[0], tab->sym[i]->scope[1]);
            printf("   %-15s", terminal_str[tab->sym[i]->type]);
            if (tab->sym[i]->is_array)
            {
                printf("   YES    ");
                if (tab->sym[i]->is_dynamic)
                {
                    printf("     YES     ");
                    printf("   %s%s..%s%s    ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].value, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].value);
                }
                else
                {
                    printf("     NO    ");
                    printf("   %s%3lld..%s%-3lld   ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].integer, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].integer);
                }
            }
            else
            {
                printf("    NO    ");
                printf("     **     ");
                printf("     **      ");
            }

            printf("   %4d   ", tab->sym[i]->width);
            printf("   %5d   ", tab->sym[i]->offset);
            printf("    %5d  ", tab->sym[i]->nesting_level);
            printf("\n");
        }
    }

    // print child tables
    for (int i = 0; i < tab->no_child; i++)
    {
        print_symbol_table(tab->child[i]);
    }

    return;
}

void print_symbol_table_list_array(mod sym_module, symbol_list_node in)
{
    // if (in == NULL) printf("\nList is NULL\n");

    while (in != NULL)
    {
        // print variables
        if (in->curr == NULL)
        {
            printf("curr was null\n");
            continue;
        }

        if (in->curr->is_array)
        {
            printf("%-20s", sym_module->table->mod_name);
            printf("     %3d-%-3d        ", in->curr->scope[0], in->curr->scope[1]);
            printf("   %-20s", in->curr->var_name);

            if (in->curr->is_dynamic)
            {
                printf("      YES     ");
                printf("      %s%s..%s%s    ", in->curr->array_range[0].value, in->curr->array_range[1].value, in->curr->array_range[2].value, in->curr->array_range[3].value);
            }
            else
            {
                printf("     NO    ");
                printf("     %s%3lld..%s%-3lld    ", in->curr->array_range[0].value, in->curr->array_range[1].integer, in->curr->array_range[2].value, in->curr->array_range[3].integer);
            }

            printf("    %-15s", terminal_str[in->curr->type]);
            printf("\n");
        }
        in = in->next;
    }
    return;
}

void print_symbol_table_array(symbol_table tab)
{
    // if (in == NULL) printf("\nList is NULL\n");
    for (int i = 0; i < 1000; i++)
    {
        if (tab->sym[i] != NULL)
        {
            if (tab->sym[i]->is_array)
            {
                printf("%-20s", tab->mod_name);
                printf("     %3d-%-3d        ", tab->sym[i]->scope[0], tab->sym[i]->scope[1]);
                printf("   %-20s", tab->sym[i]->var_name);

                if (tab->sym[i]->is_dynamic)
                {
                    printf("      YES     ");
                    printf("     %s%s..%s%s     ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].value, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].value);
                }
                else
                {
                    printf("     NO    ");
                    printf("     %s%3lld..%s%-3lld    ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].integer, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].integer);
                }

                printf("    %-15s", terminal_str[tab->sym[i]->type]);
                printf("\n");
            }
        }
    }

    for (int i = 0; i < tab->no_child; i++)
    {
        print_symbol_table_array(tab->child[i]);
    }

    return;
}

void print_symbol_module_array(mod sym_module)
{
    // print inlist var
    if (sym_module == NULL)
    {
        // printf("module not present\n");
        return;
    }
    // printf("current module=%s\n",sym_module->mod_name);
    // print inlist array variables
    print_symbol_table_list_array(sym_module, sym_module->inlist);

    // print outlist array variables
    print_symbol_table_list_array(sym_module, sym_module->outlist);

    // symbol table array variables
    print_symbol_table_array(sym_module->table);
    return;
}

void print_symbol_module(mod sym_module)
{
    // print inlist var
    if (sym_module == NULL)
    {
        // printf("module not present\n");
        return;
    }
    // printf("current module=%s\n",sym_module->mod_name);
    // print inlist variables
    print_symbol_table_list(sym_module, sym_module->inlist);

    // print outlist variables
    print_symbol_table_list(sym_module, sym_module->outlist);

    // symbol table variables
    print_symbol_table(sym_module->table);

    return;
}