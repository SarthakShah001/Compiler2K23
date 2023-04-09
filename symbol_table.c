#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

mod global_symbol_table[500];
void global_symbol_table_init(){
    for(int i=0;i<500;i++){
    global_symbol_table[i]=(mod)malloc(sizeof(struct MODULE));
    }
}
int no_of_modules = 0; // ye driver file me lena h
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

int generate_symbol_table(parseTreeNode root, symbol_table table, int nesting, int curr_offset, ast_symbol curr_symbol)
{
    printf("Currently in %s\n",ast_strings[root->ast_name]);
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
        strcpy(global_symbol_table[no_of_modules]->mod_name, "driver");
        symbol_table temp = symbol_table_init();
        strcpy(temp->mod_name,"driver");
        global_symbol_table[no_of_modules]->table = temp;
        global_symbol_table[no_of_modules]->table->modulewrapper = global_symbol_table[no_of_modules];
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
            strcpy(global_symbol_table[no_of_modules]->mod_name, currchild->tok->lex.value);
            global_symbol_table[no_of_modules]->table = temp;
            temp->modulewrapper = global_symbol_table[no_of_modules];
            strcpy(temp->mod_name,global_symbol_table[no_of_modules]->mod_name);
            no_of_modules++;
        }
        else
        {
            if (global_symbol_table[mod_no]->table != NULL)
            {
                printf("In line no-> %d module already defined before\n", currchild->tok->line_no);
                break;
            }
            else
            {
                temp->modulewrapper = global_symbol_table[mod_no];
                strcpy(temp->mod_name,global_symbol_table[mod_no]->mod_name);
                global_symbol_table[mod_no]->table = temp;
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
        
        printf("\ncurrmodule=%s\n",currmodule->mod_name);
        symbol_list_node child = currmodule->inlist;
        if(child==NULL){
        printf("\ninlist was null\n");
        currmodule->inlist = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
        currmodule->inlist->next = NULL;
        currmodule->inlist->curr = new_symbol;
        }
        else{
        while (child ->next!= NULL)
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
        strcpy(new_symbol->var_name, currchild->tok->lex.value);
        new_symbol->type = currchild->sibling->tok->token_type;
        new_symbol->width = find_width(new_symbol->type);
        new_symbol->offset = offset;
        offset += new_symbol->width;
        new_symbol->nesting_level = 0;
        mod currmodule = table->modulewrapper;
        symbol_list_node child = currmodule->outlist;
        if(child==NULL){
        printf("\noutlist was null\n");
        currmodule->outlist = (symbol_list_node)malloc(sizeof(struct Symbol_List_Node));
        currmodule->outlist->next = NULL;
        currmodule->outlist->curr = new_symbol;
        }
        else{
        while (child ->next!= NULL)
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
        offset = generate_symbol_table(currchild, table, nesting, curr_offset, curr_symbol);
        if (!curr_symbol->is_dynamic)
        {
            int len_array = curr_symbol->array_range[3].integer - curr_symbol->array_range[1].integer;
            if (len_array < 0)
            {
                len_array = -1 * len_array;
                
            }
            len_array+=1;
            curr_symbol->width = len_array * find_width(curr_symbol->type) + 1;
        }
        offset=offset + curr_symbol->width;
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
            offset = generate_symbol_table(currchild, table, nesting, offset, curr_symbol);
            currchild=currchild->sibling;
        }
        break;
    }
    case AST_GET_VALUE:
    {
        // do nothing
        break;
    }
    case AST_PRINT:
    {
        // do nothing
        break;
    }
    case AST_ARRAY_ACCESS:
    {
        // do nothing
        break;
    }
    case AST_ID_ASSIGN:
    {
        // do nothing
        break;
    }
    case AST_ARRAY_ASSIGN:
    {
        // do nothing
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
            printf("In Line no.-> %d Module needs to be declared first before use\n", currchild->tok->line_no);
        }
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
        if (currchild->tok != NULL)
        {
            tkType t = currchild->tok->token_type;
            currchild = currchild->sibling;
            while (currchild != NULL)
            {
                ast_symbol new_symbol = ast_symbol_init();
                new_symbol->nesting_level=nesting;
                strcpy(new_symbol->var_name,currchild->tok->lex.value);
                new_symbol->offset=offset;
                new_symbol->width=find_width(t);
                new_symbol->type=t;
                insert_symbol_table(table,new_symbol);
                currchild=currchild->sibling;
            }
        }
        else
        {
            currchild=currchild->sibling;
            while(currchild!=NULL){
                ast_symbol new_symbol = ast_symbol_init();
                new_symbol->nesting_level=nesting;
                strcpy(new_symbol->var_name,currchild->tok->lex.value);
                new_symbol->offset=offset;
                offset=generate_symbol_table(root->child,table,nesting,offset,new_symbol);
                insert_symbol_table(table,new_symbol);
                currchild=currchild->sibling;
            }
        }
        break;
    }
    case AST_SWITCH:
    {
        currchild=currchild->sibling;
        generate_symbol_table(currchild,table,nesting,offset,curr_symbol);
        currchild=currchild->sibling;
        if(currchild!=NULL){
        generate_symbol_table(currchild,table,nesting,offset,curr_symbol);
        }
        break;
    }
    case AST_CASES:
    {
        while(currchild!=NULL){
            symbol_table new_table=symbol_table_init();
            new_table->parent=table;
            strcpy(new_table->mod_name,table->mod_name);
            table->child[table->no_child]=new_table;
            table->no_child++;
            generate_symbol_table(currchild,new_table,nesting+1,0, NULL);
            currchild=currchild->sibling;
        }
        break;
    }
    case AST_CASE:
    {
        currchild=currchild->sibling;
        generate_symbol_table(currchild,table,nesting,0, NULL);
        break;
    }
    case AST_FORLOOP:
    {
        currchild=currchild->sibling->sibling;
        symbol_table new_table=symbol_table_init();
        new_table->parent=table;
        strcpy(new_table->mod_name,table->mod_name);
        table->child[table->no_child]=new_table;
        table->no_child++;
        generate_symbol_table(currchild,new_table,nesting+1,0, NULL);
        break;
    }
    case AST_WHILELOOP:
    {
        currchild=currchild->sibling;
        symbol_table new_table=symbol_table_init();
        new_table->parent=table;
        strcpy(new_table->mod_name,table->mod_name);
        table->child[table->no_child]=new_table;
        table->no_child++;
        generate_symbol_table(currchild,new_table,nesting+1,0, NULL);
        break;
    }
    case AST_INDEX_FOR_LOOP:
    {
        //do nothing
        break;
    }
    case AST_DEFAULT:
    {
        symbol_table new_table=symbol_table_init();
        new_table->parent=table;
        strcpy(new_table->mod_name,table->mod_name);
        table->child[table->no_child]=new_table;
        table->no_child++;
        generate_symbol_table(currchild,new_table,nesting+1,0, NULL);
    break;
    }
    default:
    {
        break;
    }
    }
    return offset;
}

void print_symbol_table(FILE* fs, symbol_table tab){
    for(int i=0;i<1000;i++){
        if(tab->sym[i]!=NULL){
            // print symbols
            fprintf(fs, "%-25s", tab->sym[i]->var_name);
            fprintf(fs, "%-25s", tab->mod_name);
            fprintf(fs, "     [%d-%d]     ", tab->sym[i]->scope[0], tab->sym[i]->scope[1]);
            fprintf(fs, "%-18s", terminal_str[tab->sym[i]->type]);
            if (tab->sym[i]->is_array)
            {
                fprintf(fs, "  ARRAY   ");
                if (tab->sym[i]->is_dynamic)
                {
                    fprintf(fs, "   DYNAMIC   ");
                    fprintf(fs, "   [%s%s-%s%s]   ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].value, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].value);
                }
                else
                {
                    fprintf(fs, "   STATIC  ");
                    fprintf(fs, "  [%s%lld-%s%lld]  ", tab->sym[i]->array_range[0].value, tab->sym[i]->array_range[1].integer, tab->sym[i]->array_range[2].value, tab->sym[i]->array_range[3].value);
                }
            }
            else
            {
                fprintf(fs, "    **    ");
                fprintf(fs, "     **     ");
                fprintf(fs, "     **     ");
            }

            fprintf(fs, "  %4d   ", tab->sym[i]->width);
            fprintf(fs, "   %5d   ", tab->sym[i]->offset);
            fprintf(fs, "    %5d    ", tab->sym[i]->nesting_level);
            fprintf(fs, "\n");
            
        }
    }
    
    // print child tables
    for(int i=0;i<tab->no_child;i++){
        print_symbol_table(fs, tab->child[i]);
    }
    
    return ;
}

void print_symbol_module(FILE* fs, mod sym_module)
{
    // print inlist var
    if(sym_module==NULL){
    printf("module not present\n");
    return;
    }
    printf("current module=%s\n",sym_module->mod_name);
    symbol_list_node in = sym_module->inlist;
    if(in==NULL) printf("\nin is NULL\n");
    while (in != NULL)
    {
        // print variables
    if(in->curr == NULL)
    {printf("curr was null");
    printf("\n");
    continue;
    }
    // printf("%-25s", in->curr->var_name);
    fprintf(fs, "%-25s", in->curr->var_name);
    printf("%-25s", sym_module->table->mod_name);
    fprintf(fs, "%-25s", sym_module->table->mod_name);
    fprintf(fs, "     [%d-%d]     ", in->curr->scope[0], in->curr->scope[1]);
    fprintf(fs, "%-18s", terminal_str[in->curr->type]);
    if (in->curr->is_array)
    {
            fprintf(fs, "  ARRAY   ");
            if (in->curr->is_dynamic)
            {
                fprintf(fs, "   DYNAMIC   ");
                fprintf(fs, "   [%s%s-%s%s]   ", in->curr->array_range[0].value, in->curr->array_range[1].value, in->curr->array_range[2].value, in->curr->array_range[3].value);
            }
            else
            {
                fprintf(fs, "   STATIC  ");
                fprintf(fs, "  [%s%lld-%s%lld]  ",in->curr->array_range[0].value, in->curr->array_range[1].integer, in->curr->array_range[2].value,in->curr->array_range[3].integer);
            }
        }
        else
        {
            fprintf(fs, "    **    ");
            fprintf(fs, "     **     ");
            fprintf(fs, "     **     ");
        } 

        fprintf(fs, "   %4d   ", in->curr->width);
        fprintf(fs, "    %5d   ", in->curr->offset);
        fprintf(fs, "     %5d    ", in->curr->nesting_level);
        fprintf(fs, "\n");
        in = in->next;
    }

    // // print outlist var
    symbol_list_node out = sym_module->outlist;
    if (out == NULL) printf("\nout is NULL\n");
    while (out != NULL)
    {
        // print variables
        fprintf(fs, "%-25s", out->curr->var_name);
        fprintf(fs, "%-25s", sym_module->table->mod_name);
        fprintf(fs, "     [%d-%d]     ", out->curr->scope[0], out->curr->scope[1]);
        fprintf(fs, "%-18s", terminal_str[out->curr->type]);
        if (out->curr->is_array)
        {
            fprintf(fs, "   YES    ");
            if (out->curr->is_dynamic)
            {
                fprintf(fs, "      YES    ");
                fprintf(fs, "   [%s%s-%s%s]   ", out->curr->array_range[0].value, out->curr->array_range[1].value, out->curr->array_range[2].value, out->curr->array_range[3].value);
            }
            else
            {
                fprintf(fs, "     NO    ");
                fprintf(fs, "  [%s%lld-%s%lld]  ", out->curr->array_range[0].value, out->curr->array_range[1].integer, out->curr->array_range[2].value, out->curr->array_range[3].integer);
            }
        }
        else
        {
            fprintf(fs, "    **    ");
            fprintf(fs, "     **    ");
            fprintf(fs, "     **    ");
        }

        fprintf(fs, "%4d   ", out->curr->width);
        fprintf(fs, "%5d   ", out->curr->offset);
        fprintf(fs, "%5d    ", out->curr->nesting_level);
        fprintf(fs, "\n");
        out = out->next;
    }
    
    // symbol table variables
    print_symbol_table(fs,sym_module->table);
    
    return ;
    
}