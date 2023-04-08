#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// mod global_symbol_table[500];
int no_of_modules = 0;
FILE *fs; // ye driver file me lena h
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
    int prob = 0;
    int hash_val = hash(s->var_name);
    while (table->sym[hash_val] != NULL)
    {
        prob++;
        hash_val = (hash_val + prob) % 1000;
    }
    table->sym[hash_val] = s;
}

void add_child_symbol_table(symbol_table parent, symbol_table child)
{
    parent->child[parent->no_child] = child;
    parent->no_child++;
    child->parent = parent;
}

ast_symbol find_symbol(symbol_table table, char *str)
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
        if (global_symbol_table[i].mod_name != NULL)
        {
            if (strcmp(global_symbol_table[i].mod_name, str) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

int generate_symbol_table(parseTreeNode root, symbol_table table, int nesting, int curr_offset, ast_symbol curr_symbol)
{

    parseTreeNode currchild = root->child;
    int offset = curr_offset;
    switch (root->ast_name)
    {

    case AST_PROGRAM:
    {
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol);
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol);
        currchild = currchild->sibling;
        strcpy(global_symbol_table[no_of_modules].mod_name, "driver");
        symbol_table temp = symbol_table_init();
        global_symbol_table[no_of_modules].table = temp;
        global_symbol_table[no_of_modules].table->modulewrapper = global_symbol_table[no_of_modules];
        no_of_modules++;
        generate_symbol_table(currchild, temp, 1, 0, curr_symbol);
        currchild = currchild->sibling;
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol);
        break;
    }
    case AST_MODULEDECLARATIONS:
    {
        while (currchild != NULL)
        {
            if (find_mod_no(currchild->tok->lex.value) != -1)
            {
                // throw error, change colour
                printf("In line no. -> %d module already declared\n", currchild->tok->line_no);
            }
            else
            {
                strcpy(global_symbol_table[no_of_modules].mod_name, currchild->tok->lex.value);
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
            generate_symbol_table(currchild, table, 0, 0, curr_symbol);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_DRIVER:
    {
        generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol);
        break;
    }
    case AST_MODULE:
    {
        symbol_table temp = symbol_table_init();
        int mod_no = find_mod_no(currchild->tok->lex.value);
        if (mod_no == -1)
        {
            strcpy(global_symbol_table[no_of_modules].mod_name, currchild->tok->lex.value);
            global_symbol_table[no_of_modules].table = temp;
            temp->modulewrapper = global_symbol_table[no_of_modules];
            no_of_modules++;
        }
        else
        {
            if (global_symbol_table[mod_no].table != NULL)
            {
                printf("In line no-> %d module already defined before\n", currchild->tok->line_no);
                break;
            }
            else
            {
                temp->modulewrapper = global_symbol_table[mod_no];
                global_symbol_table[mod_no].table = temp;
            }
        }
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 0, 0, curr_symbol);
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 0, offset, curr_symbol);
        currchild = currchild->sibling;
        offset = generate_symbol_table(currchild, temp, 1, offset, curr_symbol);
        break;
    }
    case AST_INPUT_PARAMETER_LIST:
    {
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_OUTPUT_PARAMETER_LIST:
    {
        while (currchild != NULL)
        {
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol);
            currchild = currchild->sibling;
        }
        break;
    }
    case AST_INP_PARAMETER:
    {
        ast_symbol new_symbol = ast_symbol_init();
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
            offset = generate_symbol_table(currchild->sibling, table, 0, offset, new_symbol);
        }
        mod currmodule = table->modulewrapper;
        symbol_list_node child = currmodule.inlist;
        while (child != NULL)
        {
            child = child->next;
        }
        child = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
        child->next = NULL;
        child->curr = new_symbol;
        break;
    }
    case AST_OUT_PARAMETER:
    {
        ast_symbol new_symbol = ast_symbol_init();
        strcpy(new_symbol->var_name, currchild->tok->lex.value);
        new_symbol->type = currchild->sibling->tok->token_type;
        new_symbol->width = find_width(new_symbol->type);
        new_symbol->offset = offset;
        offset += new_symbol->width;
        new_symbol->nesting_level = 0;
        mod currmodule = table->modulewrapper;
        symbol_list_node child = currmodule.inlist;
        while (child != NULL)
        {
            child = child->next;
        }
        child = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
        child->next = NULL;
        child->curr = new_symbol;
        break;
    }
    case AST_ARRAY:
    {
        offset=generate_symbol_table(currchild,table,nesting,curr_offset,curr_symbol);
        curr_symbol->type =currchild->sibling->tok->token_type;
        break;
    }
    case AST_RANGE_ARRAYS:
    {
        // generate_symbol_table(currchild,table,nesting,curr_offset,curr_symbol);
        parseTreeNode temp=currchild->child;
        if(temp->sibling==NULL){
            curr_symbol->array_range[0]=temp->tok->lex;
            if(temp->tok->token_type==TK_ID){
                curr_symbol->is_dynamic=true;
            }
        }
        else{
            
            
        }
        generate_symbol_table(currchild->sibling,table,nesting,curr_offset,curr_symbol);
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
        break;
    }
    case AST_ID_ASSIGN:
    {
        break;
    }
    case AST_ARRAY_ASSIGN:
    {
        break;
    }
    case AST_INDEX_ARR:
    {
        
        break;
    }
    case AST_MODULE_REUSE:
    {
        break;
    }
    case AST_PARAMETER_LIST1:
    {
        break;
    }
    case AST_PARAMETER_LIST2:
    {
        break;
    }
    case AST_ACTUAL_PARA:
    {
        break;
    }
    case AST_UNARYEXPR:
    {
        break;
    }
    case AST_RELATIONAL_OP:
    {
        break;
    }
    case AST_LOGICAL_OP:
    {
        break;
    }
    case AST_PLUS:
    {
        break;
    }
    case AST_MINUS:
    {
        break;
    }
    case AST_MUL:
    {
        break;
    }
    case AST_DIV:
    {
        break;
    }
    case AST_ARRAY_FACTOR:
    {
        break;
    }
    case AST_UNARY_INDEX_EXPR:
    {
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
        break;
    }
    default:
    {
        break;
    }
    }
}

void print_symbol_table(mod sym_module)
{
    // print inlist var
    symbol_list_node in = sym_module.inlist;
    while (in != NULL)
    {
        // print variables
        fprintf(fs, "%-25s", in->curr->var_name);
        fprintf(fs, "%-25s", sym_module.table->mod_name);
        fprintf(fs, "     [%d-%d]     ", in->curr->scope[0], in->curr->scope[1]);
        fprintf(fs, "%-18s", in->curr->type);
        if (in->curr->is_array)
        {
            fprintf(fs, "  ARRAY   ");
            if (in->curr->is_dynamic)
            {
                fprintf(fs, " DYNAMIC    ");
                fprintf(fs, "   [%s-%s]   ", in->curr->array_range[0].value, in->curr->array_range[1].value);
            }
            else
            {
                fprintf(fs, "  STATIC   ");
                fprintf(fs, "  [%d-%d]  ", in->curr->array_range[0].integer, in->curr->array_range[1].integer);
            }
        }
        else
        {
            fprintf(fs, "    **    ");
            fprintf(fs, "    **    ");
            fprintf(fs, "    **    ");
        }
        

        fprintf(fs, "%7d", in->curr->width);
        fprintf(fs, "%8d", in->curr->offset);
        fprintf(fs, "%9d", in->curr->nesting_level);
        in = in->next;
    }

    // print outlist var
    symbol_list_node out = sym_module.outlist;
    while (out != NULL)
    {
        // print variables
        fprintf(fs, "%-25s", out->curr->var_name);
        fprintf(fs, "%-25s", sym_module.table->mod_name);
        fprintf(fs, "     [%d-%d]     ", out->curr->scope[0], out->curr->scope[1]);
        fprintf(fs, "%-18s", out->curr->type);
        if (out->curr->is_array)
        {
            fprintf(fs, "  ARRAY   ");
            if (out->curr->is_dynamic)
            {
                fprintf(fs, " DYNAMIC    ");
                fprintf(fs, "   [%s-%s]   ", out->curr->array_range[0].value, out->curr->array_range[1].value);
            }
            else
            {
                fprintf(fs, "  STATIC   ");
                fprintf(fs, "  [%d-%d]  ", out->curr->array_range[0].integer, out->curr->array_range[1].integer);
            }
        }
        else
        {
            fprintf(fs, "    **    ");
            fprintf(fs, "    **    ");
            fprintf(fs, "    **    ");
        }

        fprintf(fs, "%7d", out->curr->width);
        fprintf(fs, "%8d", out->curr->offset);
        fprintf(fs, "%9d", out->curr->nesting_level);
        out = out->next;
    }
}

void print_global_symbol_table()
{
    // ye sab driver file me
    fs = fopen("./symbol.txt", "w");
    if (fs == NULL)
    {
        printf("File opening failed.\n");
        return;
    }
    fprintf(fs, "    VARIABLE NAME       ");
    fprintf(fs, "   SCOPE/MODULE NAME    ");
    fprintf(fs, " SCOPE(LINE NO) ");
    fprintf(fs, "  VARIABLE TYPE ");
    fprintf(fs, " IS_ARRAY ");
    fprintf(fs, "  IS_DYNAMIC ");
    fprintf(fs, "  RANGE_IF_ARR ");
    fprintf(fs, " WIDTH ");
    fprintf(fs, " OFFSET ");
    fprintf(fs, " NESTING ");
    fprintf(fs, "\n\n");
    // ye sab driver file me

    for (int i = 0; i < no_of_modules; i++)
    {
        if (global_symbol_table[i].table == NULL)
        {
            // table is not present
            fprintf(fs, "\n\nSymbol table not present for module <<%s>>\n\n", global_symbol_table[i].mod_name);
        }
        else
        {
            print_symbol_table(global_symbol_table[i]);
        }
    }
    fclose(fs);
    return;
}
