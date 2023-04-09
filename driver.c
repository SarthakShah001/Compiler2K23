/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
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

#define BUFF_SIZE 5000

void printMenu()
{
    printf("\t Select the option you want to see\n ");
    printf("\n------------------------------------------------------\n");
    printf("\n 0 -> for exit\n");
    printf("\n 1 -> Print the comment removed source code\n");
    printf("\n 2 -> Print tokens generated by lexer\n");
    printf("\n 3 -> Print the Parse Tree\n");
    printf("\n 4 -> Print the Total Time taken\n");
    printf("\n 5 -> Print the AST Tree\n");
    printf("\n 6 -> Print the SYMBOL Table\n");
}
void displayInfo(){
    printf("\n     ------------------------------------------------------\n");
    printf("\033[32m");
    printf("\n\t FIRST and FOLLOW set automated\n");
    printf("\t Both lexical and syntax analysis module developed\n");
    printf("\t modules work with all testcases\n");
    printf("\t Parse tree is constructed for all testcases\n");
    printf("\033[0m");
    printf("\n     ------------------------------------------------------\n\n");

}
int main(int argc, char *argv[])
{
    global_symbol_table_init();
    displayInfo();
    if (argc < 6)
    {
        printf("very few arguments\n");
        exit(0);
    }
    FILE *fp = NULL;
    FILE *fs = NULL;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("error in opening file\n");
    }
    FILE *fp1 = NULL;
    fp1 = fopen(argv[2], "w");
    parseTreeNode root;
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
            exit(0);
        }
        case 1:
        {
            char buffer[BUFF_SIZE], cleanFile[BUFF_SIZE];
            fread(buffer, sizeof(char), BUFF_SIZE, fp);
            char *testCaseFile = buffer;
            removeComments(testCaseFile, cleanFile);
            puts(cleanFile);
            break;
        }

        case 2:
        {
            print_tokens(fp, atoi(argv[3]));
            break;
        }

        case 3:
        {
            fp1 = fopen(argv[2], "w");
            setbuf(fp1, NULL);
            root = startParser(fp, atoi(argv[3]));
            openparsetreefile(fp1);

            fprintf(fp1, "       LEXEME       ");
            fprintf(fp1, "  LINE_NO  ");
            fprintf(fp1, "     TOKEN-TYPE      ");
            fprintf(fp1, "  VALUE if NUM/RNUM  ");
            fprintf(fp1, "       PARENT       ");
            fprintf(fp1, "   IS A LEAF NODE  ");
            fprintf(fp1, "    NON-TERMINAL     ");
            fprintf(fp1, "\n\n");

            printParseTree(root);
            freeParseTree(root) ; 
            // printParseTree(root) ;
            root=NULL;
            break;
        }

        case 4:
        {
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();

            // PARSER CODE HERE
            startParser(fp, atoi(argv[3]));

            end_time = clock();

            total_CPU_time = (double)(end_time - start_time);

            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            printf("TOTAL CPU CLOCK TICKS := %lf \n", total_CPU_time);
            printf("TOTAL CPU TIME IN SECONDS := %lf \n", total_CPU_time_in_seconds);
            break;
        }
        case 5:
        {
            fp1 = fopen(argv[4], "w");
            setbuf(fp1, NULL);
        root = startParser(fp, atoi(argv[3]));
        parseTreeNode ast_root=generate_ast(root);
        openparsetreefile(fp1);

            fprintf(fp1, "       LEXEME       ");
            fprintf(fp1, "  LINE_NO  ");
            fprintf(fp1, "     TOKEN-TYPE      ");
            fprintf(fp1, "  VALUE if NUM/RNUM  ");
            fprintf(fp1, "       PARENT       ");
            fprintf(fp1, "   IS A LEAF NODE  ");
            fprintf(fp1, "    NON-TERMINAL     ");
            fprintf(fp1, "\n\n");
        ast_root=ast_root->syn_node;
        if(ast_root->child==NULL){
        printf("YES NULL\n"
        );}
        else{
        printf("NO NULL %s\n",ast_strings[ast_root->child->ast_name]);
        }
        
        printf("ast_root=%s\n",nonterminal_str[ast_root->s->nt]);
        printAST(ast_root);
        }
        case 6:
        {
        fs = fopen(argv[5], "w");
        if (fs == NULL)
        {
        printf("File opening failed.\n");
        return 0;
        }
        fp1 = fopen(argv[4], "w");
        setbuf(fp1, NULL);
        root = startParser(fp, atoi(argv[3]));
        parseTreeNode ast_root=generate_ast(root);
        ast_root=ast_root->syn_node;
        generate_symbol_table(ast_root,NULL,0,0,NULL);
        setbuf(fs, NULL);
        fprintf(fs, "VARIABLE NAME       ");
        fprintf(fs, "   SCOPE/MODULE NAME    ");
        fprintf(fs, "    SCOPE(LINE NO) ");
        fprintf(fs, "  VARIABLE TYPE    ");
        fprintf(fs, "  IS_ARRAY ");
        fprintf(fs, "  IS_DYNAMIC ");
        fprintf(fs, "  RANGE_IF_ARR ");
        fprintf(fs, "  WIDTH  ");
        fprintf(fs, "  OFFSET  ");
        fprintf(fs, "  NESTING  ");
        fprintf(fs, "\n\n");
    
        printf("no_of_modules=%d\n", no_of_modules);
        for (int i = 0; i < no_of_modules; i++)
        {
           
            if (global_symbol_table[i]->table == NULL)
            {
                // table is not present
                fprintf(fs, "\n\nSymbol table not present for module <<%s>>\n\n", global_symbol_table[i]->mod_name);
            }
            else print_symbol_module(fs,global_symbol_table[i]);
        }
        fclose(fs);
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