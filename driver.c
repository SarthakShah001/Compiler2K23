/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "removeComments.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbol_table.h"
#include "intermediateCodeGen.h"
#include "codegen.h"
#define BUFF_SIZE 5000
#define buff_size 200

void printMenu()
{
    printf("\t Select the option you want to see\n ");
    printf("\n------------------------------------------------------\n");
    printf("\n 0 -> for exit\n");
    // printf("\n 1 -> Print the comment removed source code\n");
    printf("\n 1 -> Print tokens generated by lexer\n");
    printf("\n 2 -> Print the Parse Tree\n");
    // printf("\n 4 -> Print the Total Time taken\n");
    printf("\n 3 -> Print the AST Tree\n");
    printf("\n 4 -> Print the amount of memory allocated to parseTree and AST nodes\n");
    printf("\n 5 -> Print the SYMBOL Table\n");
    printf("\n 6 -> Print the Activation Record size\n");
    printf("\n 7 -> Print type expression and width of Array variable\n");
    printf("\n 8 -> Print the Errors and Total Compiling time\n");
    printf("\n 9 -> Print the Assembly Code Generated\n\n");
}
void displayInfo()
{
    printf("\n     ------------------------------------------------------\n");
    printf("\033[32m");
    printf("\n\t FIRST and FOLLOW sets automated\n");
    printf("\t Both lexical and syntax analysis module developed\n");
    printf("\t Modules work with all testcases\n");
    printf("\t Parse tree is constructed for all testcases\n");
    printf("\033[0m");
    printf("\n     ------------------------------------------------------\n\n");

    // display info to be updated

    // printf("\033[32m");
    // printf("\nLEVEL 4: AST/Symbol table/Type checking/Semantic rules Module working.\n");
    // printf("Handled Static and Dynamic arrays in code generation\n\n");
    // printf("\033[0m");

    // if there are no errors of any kind, print below message:
    // printf("*****  CODE Compiles Succesfully  *****");
}

int count_nodes(parseTreeNode node)
{
    if (node == NULL)
    {
        return 0;
    }
    return count_nodes(node->sibling) + count_nodes(node->child) + 1;
}

int count_tree_size(parseTreeNode node)
{
    if (node == NULL)
    {
        return 0;
    }
    return count_tree_size(node->sibling) + count_tree_size(node->child) + sizeof(*node);
}

void print_parseTree_main(parseTreeNode root){
    printf("       LEXEME       ");
    printf("  LINE_NO  ");
    printf("     TOKEN-TYPE      ");
    printf("  VALUE if NUM/RNUM  ");
    printf("       PARENT       ");
    printf("   IS A LEAF NODE  ");
    printf("    NON-TERMINAL     ");
    printf("\n\n");

    printParseTree(root);
    // freeParseTree(root) ;
    // printParseTree(root) ;
    root = NULL;
    return;
}

void print_AST_main(parseTreeNode ast_root){
    printf("       LEXEME       ");
    printf("  LINE_NO  ");
    printf("     TOKEN-TYPE      ");
    printf("  VALUE if NUM/RNUM  ");
    printf("       PARENT       ");
    printf("   IS A LEAF NODE  ");
    printf("    NON-TERMINAL     ");
    printf("\n\n");
    ast_root = ast_root->syn_node;
    if (ast_root->child == NULL)
    {
        printf("YES NULL\n");
    }
    else
    {
        printf("NO NULL %s\n", ast_strings[ast_root->child->ast_name]);
    }

    printf("ast_root=%s\n", nonterminal_str[ast_root->s->nt]);
    printAST(ast_root);
}

void print_symbol_table_main()
{
    printf("no_of_modules=%d\n", no_of_modules);

    printf("VARIABLE NAME     ");
    printf(" SCOPE/MODULE NAME  ");
    printf("   SCOPE(LINE NO) ");
    printf("  VARIABLE TYPE   ");
    printf("IS_ARRAY  ");
    printf(" IS_DYNAMIC  ");
    printf("RANGE_IF_ARR ");
    printf("  WIDTH   ");
    printf("  OFFSET  ");
    printf("   NESTING ");
    printf("\n\n");

    for (int i = 0; i < no_of_modules; i++)
    {

        if (global_symbol_table[i]->table == NULL)
        {
            // table is not present
            printf("\n\nSymbol table not present for module <<%s>>\n\n", global_symbol_table[i]->mod_name);
        }
        else
            print_symbol_module(global_symbol_table[i]);
    }

    return;
}

void print_symbol_table_array_main()
{
    printf("no_of_modules = %d\n", no_of_modules);

    printf("SCOPE/MODULE NAME    ");
    printf("  SCOPE(LINE NO)  ");
    printf("  NAME OF VARIABLE     ");
    printf("  IS_DYNAMIC  ");
    printf("  RANGE_IF_ARR  ");
    printf("  ELEMENT TYPE ");
    printf("\n\n");

    for (int i = 0; i < no_of_modules; i++)
    {

        if (global_symbol_table[i]->table == NULL)
        {
            // table is not present
            printf("\n\nSymbol table not present for module <<%s>>\n\n", global_symbol_table[i]->mod_name);
        }
        else
            print_symbol_module_array(global_symbol_table[i]);
    }
    return;
}

void print_activation_record_main(){
    printf("Module name      ");
    printf("     Width     ");
    printf("\n\n");

    for (int i = 0; i < no_of_modules; i++)
    {

        if (global_symbol_table[i]->table != NULL)
        {
            // table is present
            printf("%-20s", global_symbol_table[i]->mod_name);
            printf("      %d     ", global_symbol_table[i]->width);
            printf("\n");
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    displayInfo();
    global_symbol_table_init();
    if (argc < 3)
    {
        printf("very few arguments\n");
        exit(0);
    }
    FILE *fp = NULL;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("error in opening file\n");
        exit(0);
    }

    parseTreeNode root;
    parseTreeNode ast_root;
    int t[2] = {0, 0};
    while (true)
    {
        fp = fopen(argv[1], "r");
        printMenu();
        printf("Please Enter the Choice\n");
        int x;
        scanf("%d", &x);
        printf("\n");
        switch (x)
        {
        case 0:
        {
            printf("\nTERMINATING THE PROGRAM\n");
            fclose(fp);
            exit(0);
        }
        case 1:
        {
            // char buffer[BUFF_SIZE], cleanFile[BUFF_SIZE];
            // fread(buffer, sizeof(char), BUFF_SIZE, fp);
            // char *testCaseFile = buffer;
            // removeComments(testCaseFile, cleanFile);
            // puts(cleanFile);
            print_tokens(fp, buff_size);
            break;
        }

        case 2:
        {
            root = startParser(fp, buff_size);

            print_parseTree_main(root);
            // freeParseTree(root) ;
            // printParseTree(root) ;
            break;
        }

        case 3:
        {
            root = startParser(fp, buff_size);
            ast_root = generate_ast(root);
    
            print_AST_main(ast_root);
            // freeParseTree(root) ;

            break;
        }

        case 4:
        {

            // printf("\nMemory allocation to be printed\n");

            root = startParser(fp, buff_size);
            int no_parseTreeNodes = count_nodes(root);
            int size_parseTree = count_tree_size(root);

            ast_root = generate_ast(root);
            ast_root = ast_root->syn_node;
            int no_AstNodes = count_nodes(ast_root);
            int size_AST = count_tree_size(ast_root);

            printf("\n");
            printf("Parse Tree Nodes = %d\n", no_parseTreeNodes);
            printf("Parse Tree Size = %d bytes\n", size_parseTree);
            printf("AST Nodes = %d\n", no_AstNodes);
            printf("AST Size = %d bytes\n", size_AST);

            double compression_percentage = ((double)(size_parseTree - size_AST) / (double)(size_parseTree)) * 100;
            printf("Compression Percentage = %lf\n", compression_percentage);

            break;
        }
        case 5:
        {
            
            root = startParser(fp, buff_size);
            ast_root = generate_ast(root);
            ast_root = ast_root->syn_node;

            generate_symbol_table(ast_root, NULL, 0, 0, NULL, t);

            print_symbol_table_main();

            break;
        }
        case 6:
        {
            printf("\nActivation record to be printed\n");

            print_activation_record_main();

            break;
        }
        case 7:
        {
            // print static and dynamic array variables

            root = startParser(fp, buff_size);
            ast_root = generate_ast(root);
            ast_root = ast_root->syn_node;
            generate_symbol_table(ast_root, NULL, 0, 0, NULL, t);

            print_symbol_table_array_main();

            break;
        }
        case 8:
        {
            // Error reporting and total compiling time

            // if code is syntactically incorrect, print syntactic errors
            // else report all semantic and type checking errors

            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();

            // PARSER CODE HERE
            root = startParser(fp, buff_size);

            // AST and SYMBOL TABLE CODE HERE
            no_of_errors=0;
            ast_root = generate_ast(root);
            ast_root = ast_root->syn_node;
            generate_symbol_table(ast_root, NULL, 0, 0, NULL, t);
            if(no_of_errors>0){
                printf("\nTotal no of errors = %d\n", no_of_errors);
            }
            // INTERMEDIATE AND CODEGEN CODE HERE

            end_time = clock();

            total_CPU_time = (double)(end_time - start_time);

            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            printf("\nTOTAL CPU CLOCK TICKS := %lf \n", total_CPU_time);
            printf("TOTAL CPU TIME IN SECONDS := %lf \n", total_CPU_time_in_seconds);

            break;
        }
        case 9:
        {
            // assembly code generation

            break;
        }
        case 10:
        {
            root = startParser(fp, buff_size);
            
            // AST and SYMBOL TABLE CODE HERE
            ast_root = generate_ast(root);
            ast_root = ast_root->syn_node;
            generate_symbol_table(ast_root, NULL, 0, 0, NULL, t);
            qNode start = (qNode)malloc(sizeof(struct QUADRUPLE));
            // if (ast_root == NULL)
            // {
            //     printf("ast_null\n");
            // }
            print_symbol_table_main();
            generateIR(start, ast_root, NULL, 0);
            printf("operator           arg1               arg2                result\n"); 
            printQuadruple(start->next);
            print_symbol_table_main();
            // createCode(start->next);
            break;
        }
        
        default:
        {
            printf("\nwrong choice\n");
            break;
        }
        }
        printf("\n\n");
    }
    return 0;
}