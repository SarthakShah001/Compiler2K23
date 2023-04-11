#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

    if (a > b) return (n>=b && n<=a);
    else return (n>=a && n<=b);
}

void global_symbol_table_init()
{
    for (int i = 0; i < 500; i++)
    {
        global_symbol_table[i] = (mod)malloc(sizeof(struct MODULE));
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
    return temp;
}

void insert_symbol_table(symbol_table table, ast_symbol s)
{
    if (table == NULL)
    {
        printf("table is NULL\n");
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
        printf("Table is NULL\n");
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
                printf("ERROR: module %s cannot be redeclared in line no %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
            }
            else
            {
                strcpy(global_symbol_table[no_of_modules]->mod_name, currchild->tok->lex.value);
                no_of_modules++;
            }
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_MODULEDEFINITIONS:
    {
        while (currchild != NULL)
        {
            generate_symbol_table(currchild, table, 0, 0, curr_symbol, currscope);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_DRIVER:
    {
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        break;
    }
    case AST_MODULE:
    {
        symbol_table temp = symbol_table_init();
        int mod_no = find_mod_no(currchild->tok->lex.value);
        if (mod_no == -1)
        {
            strcpy(global_symbol_table[no_of_modules]->mod_name, currchild->tok->lex.value);
            global_symbol_table[no_of_modules]->table = temp;
            temp->modulewrapper = global_symbol_table[no_of_modules];
            strcpy(temp->mod_name, global_symbol_table[no_of_modules]->mod_name);
            no_of_modules++;
        }
        else
        {
            if (global_symbol_table[mod_no]->table != NULL)
            {
                // throw error change colour
                printf("\033[31m");
                printf("ERROR: module %s cannot be overloaded in line no. %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
                break;
            }
            else
            {
                temp->modulewrapper = global_symbol_table[mod_no];
                strcpy(temp->mod_name, global_symbol_table[mod_no]->mod_name);
                global_symbol_table[mod_no]->table = temp;
            }
        }
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 0, 0, curr_symbol, root->scope);
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 0, offset, curr_symbol, root->scope);
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 1, offset, curr_symbol, root->scope);
        break;
    }
    case AST_INPUT_PARAMETER_LIST:
    {
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_OUTPUT_PARAMETER_LIST:
    {
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_INP_PARAMETER:
    {
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
        curr_symbol->type = currchild->sibling->tok->token_type;
        offset = generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
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
        // generate_symbol_table(currchild,table,nesting,curr_offset,curr_symbol);
        parseTreeNode temp = currchild->child;
        if (temp->sibling == NULL)
        {
            strcpy(curr_symbol->array_range[0].value, "");
            curr_symbol->array_range[1] = temp->tok->lex;
            if (temp->tok->token_type == TK_ID)
            {
                curr_symbol->is_dynamic = true;
            }
        }
        else
        {
            curr_symbol->array_range[0] = temp->tok->lex;
            if (temp->sibling->tok->token_type = TK_ID)
            {
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
                curr_symbol->is_dynamic = true;
            }
        }
        else
        {
            curr_symbol->array_range[2] = temp->tok->lex;
            if (temp->sibling->tok->token_type = TK_ID)
            {
                curr_symbol->is_dynamic = true;
            }

            curr_symbol->array_range[3] = temp->sibling->tok->lex;
        }
        break;
    }
    case AST_STATEMENTS:
    {
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_GET_VALUE:
    {

        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            printf("\033[31m");
            printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
            printf("\033[0m");
        }
        break;
    }
    case AST_PRINT:
    {
        if (currchild->tok == NULL)
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        }
        else if (currchild->tok->token_type == TK_ID)
        {
            if (find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // throw error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
            }
        }
        break;
    }
    case AST_ARRAY_ACCESS:
    {
        // //     // id->sign(canbeNULL)->numberorid

        // //     // array also be declared
        // //     // array index should be integer and in range
        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            printf("\033[31m");
            printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
            printf("\033[0m");
            break;
        }
        else
        {
            ast_symbol t = find_symbol(table, currchild->tok->lex.value);
            if (!t->is_array)
            {
                printf("\033[31m");
                printf("ERROR: variable %s is not of type ARRAY in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
                                printf("ERROR: Index of Array %s is out of bounds in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                                printf("\033[0m");
                            }
                        }
                        else
                        {
                            if (currchild->tok->lex.integer < a || currchild->tok->lex.integer > b)
                            {
                                // throw error
                                printf("\033[31m");
                                printf("ERROR: Index of Array %s is out of bounds in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                                printf("\033[0m");
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
                        printf("ERROR: variable %s is not declared in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                        printf("\033[0m");
                    }
                    else if (t->is_array)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR: Array variable %s is not allowed as index of an Array in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                        printf("\033[0m");
                    }
                    else if (t->type != TK_NUM)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR: Array index %s should be of type INTEGER in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                        printf("\033[0m");
                    }
                }
                else
                {
                    // throw error
                    printf("\033[31m");
                    printf("ERROR: Array index %s not of type INTEGER in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                    printf("\033[0m");
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
                                printf("ERROR: Index of Array %s is out of bounds in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                                printf("\033[0m");
                            }
                        }
                        else
                        {
                            if (x < a || x > b)
                            {
                                // throw error
                                printf("\033[31m");
                                printf("ERROR: Index of Array %s is out of bounds in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                                printf("\033[0m");
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
                        printf("ERROR: variable %s is not declared in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                        printf("\033[0m");
                    }
                    else if (t->is_array)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR: Array variable %s is not allowed as index in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                        printf("\033[0m");
                    }
                    else if (t->type != TK_NUM)
                    {
                        // throw error
                        printf("\033[31m");
                        printf("ERROR: Array index %s not of type INTEGER in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                        printf("\033[0m");
                    }
                }
                else
                {
                    // throw error
                    printf("\033[31m");
                    printf("ERROR: Array index %s not of type INTEGER in line %d\n", root->child->tok->lex.value, root->child->tok->line_no);
                    printf("\033[0m");
                }
            }
        }
        //     // variable should be declared and is int
        break;
    }
    case AST_ID_ASSIGN:
    {
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
            printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
            printf("\033[0m");
        }
        currchild = currchild->sibling;
        if (currchild->tok != NULL)
        {
            ast_symbol t1 = find_symbol(table, currchild->tok->lex.value);
            ast_symbol t2 = find_symbol(table, root->child->tok->lex.value);
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
                break;
            }
            else if (currchild->tok->token_type == TK_ID && (find_symbol(table, currchild->tok->lex.value)->type != find_symbol(table, root->child->tok->lex.value)->type))
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR: Type mismatch error in line no %d\n", currchild->tok->line_no);
                printf("\033[0m");
            }
            else if (currchild->tok->token_type == TK_ID && t1->is_array && t2->is_array)
            {
                int a = range_cal(t1);
                int b = range_cal(t2);
                if (a != b)
                {
                    printf("\033[31m");
                    printf("ERROR: Type mismatch error in line no %d\n", currchild->tok->line_no);
                    printf("\033[0m");
                }
            }
            else if (currchild->tok->token_type == TK_ID && (t1->is_array || t2->is_array))
            {
                printf("\033[31m");
                printf("ERROR: Type mismatch error in line no %d\n", currchild->tok->line_no);
                printf("\033[0m");
            }
            else if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value)->is_array)
            {
                // type mismatch error
                printf("\033[31m");
                printf("ERROR: Type mismatch error in line no %d\n", currchild->tok->line_no);
                printf("\033[0m");
            }
            else if (!is_same_type(find_symbol(table, root->child->tok->lex.value)->type, currchild->tok->token_type))
            {
                // type not matching error
                printf("\033[31m");
                printf("ERROR: Type mismatch error in line no %d\n", currchild->tok->line_no);
                printf("\033[0m");
            }
        }
        else
        {
            if (find_symbol(table, root->child->tok->lex.value)->is_array)
            {
                printf("\033[31m");
                printf("ERROR: Type mismatch error in line no %d\n", root->child->tok->line_no);
                printf("\033[0m");
                break;
            }
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            if (currchild->type_syn == epsilon)
            {
                break;
            }
            if (!is_same_type(find_symbol(table, root->child->tok->lex.value)->type, currchild->type_syn))
            {
                printf("\033[31m");
                printf("ERROR: Type mismatch error in line no %d\n", root->child->tok->line_no);
                printf("\033[0m");
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
        currchild = currchild->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
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
        if (find_mod_no(currchild->tok->lex.value) == -1)
        {
            // throw error change colour
            printf("\033[31m");
            printf("ERROR: module  needs to be declared first before use in line no -> %d \n", currchild->tok->line_no);
            printf("\033[0m");
        }
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        break;
    }
    case AST_PARAMETER_LIST1:
    {
        while (currchild != NULL)
        {
            if (find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
            }
            currchild = currchild->sibling;
        }
        // do nothing
        break;
    }
    case AST_PARAMETER_LIST2:
    {
        while (currchild != NULL)
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            currchild = currchild->sibling;
        }
        // do nothing
        break;
    }
    case AST_ACTUAL_PARA:
    {
        if (currchild->tok != NULL && (currchild->tok->token_type == TK_PLUS || currchild->tok->token_type == TK_MINUS))
        {
            currchild = currchild->sibling;
        }
        if (currchild->tok != NULL && (currchild->tok->token_type == TK_ID) && (find_symbol(table, currchild->tok->lex.value) == NULL))
        {
            printf("\033[31m");
            printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
            printf("\033[0m");
        }
        else if (currchild->tok == NULL)
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
        }
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
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR: Type mismatch error in line no %d\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
            break;
        }
        root->type_syn = TK_BOOLEAN;
        // do nothing
        break;
    }
    case AST_LOGICAL_OP:
    {
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || !is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR: Type mismatch error in line no %d\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
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
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }
        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR: Type mismatch error in line no %d\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
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
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR: Type mismatch error in line no %d\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
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
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR: Type mismatch error in line no %d\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
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
        tkType t1, t2;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t1 = currchild->type_syn;
            if (t1 == epsilon)
            {
                break;
            }
        }

        currchild = currchild->sibling->sibling;
        if (currchild->tok != NULL)
        {
            if (currchild->tok->token_type == TK_ID && find_symbol(table, currchild->tok->lex.value) == NULL)
            {
                // var not declared error
                printf("\033[31m");
                printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
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
            t2 = currchild->type_syn;
            if (t2 == epsilon)
            {
                break;
            }
        }

        if (!is_same_type(t1, t2) || is_boolean(t1))
        {
            printf("\033[31m");
            printf("ERROR: Type mismatch error in line no %d\n", root->child->sibling->tok->line_no);
            printf("\033[0m");
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
        // do nothing
        break;
    }
    case AST_ARRAY_FACTOR:
    {
        ast_symbol t = find_symbol(table, currchild->tok->lex.value);
        if (find_symbol(table, currchild->tok->lex.value) == NULL)
        {
            printf("\033[31m");
            printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
            printf("\033[0m");
            break;
        }
        else if (!find_symbol(table, currchild->tok->lex.value)->is_array)
        {
            printf("\033[31m");
            printf("ERROR: variable %s is not an error in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
            printf("\033[0m");
            break;
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
                        printf("ERROR: variable %s is not declared in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                        printf("\033[0m");
                        break;
                    }
                    else if (find_symbol(table, currchild->tok->lex.value)->type != TK_INTEGER)
                    {
                        // var should be integer
                        printf("\033[31m");
                        printf("ERROR: variable %s should be of type INTEGER in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                        printf("\033[0m");
                        break;
                    }
                }
                else if (currchild->tok->token_type != TK_NUM)
                {
                    // only integer access possible in array
                    printf("\033[31m");
                    printf("ERROR: array index should be type INTEGER in line %d\n",  currchild->tok->line_no);
                    printf("\033[0m");
                    break;
                }
                else{
                    if(!is_inBounds(t,currchild->tok->lex.integer)){
                    printf("\033[31m");
                    printf("ERROR: array index not in bounds in line %d\n",  currchild->tok->line_no);
                    printf("\033[0m");
                    break;
                    }
                }
            }
            else
            {
                generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
                if (currchild->type_syn != TK_NUM && currchild->type_syn != TK_INTEGER)
                {
                    // throw error
                    printf("\033[31m");
                    printf("ERROR: variable %s should be of type INTEGER for array access in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                    printf("\033[0m");
                    break;
                }
            }
        }
        else
        {
            generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol, currscope);
            if (currchild->type_syn != TK_NUM && currchild->type_syn != TK_INTEGER)
            {
                // throw error
                printf("\033[31m");
                printf("ERROR: variable %s should be of type INTEGER for array access in line %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                printf("\033[0m");
                break;
            }
        }
        root->type_syn=t->type;
        // do nothing
        break;
    }
    case AST_UNARY_INDEX_EXPR:
    {
        // do nothing
        break;
    }
    case AST_DECLARE_STMT:
    {
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
                    printf("ERROR: variable %s is redeclared in line no %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                    printf("\033[0m");
                    currchild = currchild->sibling;
                    continue;
                }
                if (nesting == 1 && find_in_list(table->modulewrapper->outlist, currchild->tok->lex.value) != NULL)
                {
                    printf("\033[31m");
                    printf("ERROR: variable %s is declared in output parameter list and cannot be redeclared in line no %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                    printf("\033[0m");
                    currchild = currchild->sibling;
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
                ast_symbol new_symbol = ast_symbol_init();
                new_symbol->nesting_level = nesting;
                if (find_symbol(table, currchild->tok->lex.value) != NULL && find_symbol(table, currchild->tok->lex.value)->nesting_level == nesting)
                {
                    printf("\033[31m");
                    printf("ERROR: variable %s is redeclared in line no %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                    printf("\033[0m");
                    currchild = currchild->sibling;
                    continue;
                }
                if (nesting == 1 && find_in_list(table->modulewrapper->outlist, currchild->tok->lex.value) != NULL)
                {
                    printf("\033[31m");
                    printf("ERROR: variable %s is declared in output parameter list and cannot be redeclared in line no %d\n", currchild->tok->lex.value, currchild->tok->line_no);
                    printf("\033[0m");
                    currchild = currchild->sibling;
                    continue;
                }
                strcpy(new_symbol->var_name, currchild->tok->lex.value);
                new_symbol->offset = offset;
                new_symbol->is_array = true;
                offset = generate_symbol_table(root->child, table, nesting, offset, new_symbol, currscope);
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
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
        currchild = currchild->sibling;
        if (currchild != NULL)
        {
            generate_symbol_table(currchild, table, nesting, offset, curr_symbol, currscope);
        }
        break;
    }
    case AST_CASES:
    {
        while (currchild != NULL)
        {
            symbol_table new_table = symbol_table_init();
            new_table->parent = table;
            strcpy(new_table->mod_name, table->mod_name);
            table->child[table->no_child] = new_table;
            table->no_child++;
            offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_CASE:
    {
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, table, nesting, offset, NULL, root->scope);
        break;
    }
    case AST_FORLOOP:
    {
        currchild = currchild->sibling->sibling;
        symbol_table new_table = symbol_table_init();
        new_table->parent = table;
        strcpy(new_table->mod_name, table->mod_name);
        table->child[table->no_child] = new_table;
        table->no_child++;
        offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
        break;
    }
    case AST_WHILELOOP:
    {
        currchild = currchild->sibling;
        symbol_table new_table = symbol_table_init();
        new_table->parent = table;
        strcpy(new_table->mod_name, table->mod_name);
        table->child[table->no_child] = new_table;
        table->no_child++;
        offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
        break;
    }
    case AST_INDEX_FOR_LOOP:
    {
        // do nothing
        break;
    }
    case AST_DEFAULT:
    {
        symbol_table new_table = symbol_table_init();
        new_table->parent = table;
        strcpy(new_table->mod_name, table->mod_name);
        table->child[table->no_child] = new_table;
        table->no_child++;
        offset = generate_symbol_table(currchild, new_table, nesting + 1, offset, NULL, root->scope);
        break;
    }
    default:
    {
        break;
    }
    }
    return offset;
}
void type_checking(parseTreeNode root, tkType syn, tkType inh)
{
    parseTreeNode currchild = root->child;
    switch (root->ast_name)
    {

    case AST_PROGRAM:
    {

        break;
    }
    case AST_MODULEDECLARATIONS:
    {

        break;
    }
    case AST_MODULEDEFINITIONS:
    {

        break;
    }
    case AST_DRIVER:
    {
        break;
    }
    case AST_MODULE:
    {

        break;
    }
    case AST_INPUT_PARAMETER_LIST:
    {

        break;
    }
    case AST_OUTPUT_PARAMETER_LIST:
    {

        break;
    }
    case AST_INP_PARAMETER:
    {

        break;
    }
    case AST_OUT_PARAMETER:
    {

        break;
    }
    case AST_ARRAY:
    {

        break;
    }
    case AST_RANGE_ARRAYS:
    {

        break;
    }
    case AST_STATEMENTS:
    {

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

        //     // variable should be declared and is int
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

        break;
    }
    case AST_PARAMETER_LIST1:
    {
        // do nothing
        break;
    }
    case AST_PARAMETER_LIST2:
    {
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
    case AST_UNARY_INDEX_EXPR:
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

        break;
    }
    case AST_CASES:
    {

        break;
    }
    case AST_CASE:
    {

        break;
    }
    case AST_FORLOOP:
    {

        break;
    }
    case AST_WHILELOOP:
    {

        break;
    }
    case AST_INDEX_FOR_LOOP:
    {
        // do nothing
        break;
    }
    case AST_DEFAULT:
    {

        break;
    }
    default:
    {
        break;
    }
    }
    return;
}

void print_symbol_table_list(mod sym_module, symbol_list_node in)
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
        printf("%-20s", in->curr->var_name);
        // printf("%-25s", sym_module->table->mod_name);
        printf("%-20s", sym_module->table->mod_name);
        printf(" [%d-%d]       ", in->curr->scope[0], in->curr->scope[1]);
        printf("%-18s", terminal_str[in->curr->type]);
        if (in->curr->is_array)
        {
            printf("   YES    ");
            if (in->curr->is_dynamic)
            {
                printf("     YES     ");
                printf("   [%s%s..%s%s]    ", in->curr->array_range[0].value, in->curr->array_range[1].value, in->curr->array_range[2].value, in->curr->array_range[3].value);
            }
            else
            {
                printf("     NO    ");
                printf("   [%s%lld..%s%lld]   ", in->curr->array_range[0].value, in->curr->array_range[1].integer, in->curr->array_range[2].value, in->curr->array_range[3].integer);
            }
        }
        else
        {
            printf("    **    ");
            printf("     **     ");
            printf("     **     ");
        }

        printf("   %4d   ", in->curr->width);
        printf("   %5d   ", in->curr->offset);
        printf("    %5d", in->curr->nesting_level);
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
            printf(" [%d-%d]       ", tab->sym[i]->scope[0], tab->sym[i]->scope[1]);
            printf("%-18s", terminal_str[tab->sym[i]->type]);
            if (tab->sym[i]->is_array)
            {
                printf("   YES    ");
                if (tab->sym[i]->is_dynamic)
                {
                    printf("     YES     ");
                    printf("   [%s%s..%s%s]    ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].value, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].value);
                }
                else
                {
                    printf("     NO    ");
                    printf("   [%s%lld..%s%lld]   ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].integer, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].integer);
                }
            }
            else
            {
                printf("    **    ");
                printf("     **     ");
                printf("     **     ");
            }

            printf("   %4d   ", tab->sym[i]->width);
            printf("   %5d   ", tab->sym[i]->offset);
            printf("    %5d", tab->sym[i]->nesting_level);
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
            printf("     [%d-%d]        ", in->curr->scope[0], in->curr->scope[1]);
            printf("%-20s", in->curr->var_name);

            if (in->curr->is_dynamic)
            {
                printf("    DYNAMIC   ");
                printf("    [%s%s..%s%s]    ", in->curr->array_range[0].value, in->curr->array_range[1].value, in->curr->array_range[2].value, in->curr->array_range[3].value);
            }
            else
            {
                printf("   STATIC  ");
                printf("   [%s%lld..%s%lld]   ", in->curr->array_range[0].value, in->curr->array_range[1].integer, in->curr->array_range[2].value, in->curr->array_range[3].integer);
            }

            printf("%-15s", terminal_str[in->curr->type]);
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
                printf("     [%d-%d]        ", tab->sym[i]->scope[0], tab->sym[i]->scope[1]);
                printf("%-20s", tab->sym[i]->var_name);

                if (tab->sym[i]->is_dynamic)
                {
                    printf("    DYNAMIC   ");
                    printf("   [%s%s..%s%s]   ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].value, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].value);
                }
                else
                {
                    printf("   STATIC  ");
                    printf("   [%s%lld..%s%lld]  ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].integer, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].integer);
                }

                printf("%-15s", terminal_str[tab->sym[i]->type]);
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
        printf("module not present\n");
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
        printf("module not present\n");
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