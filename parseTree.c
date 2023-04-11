/*
                    *****  Group No. - 9  *****
        Name : Sarthak Shah                 ID : 2020A7PS0092P
        Name : BhanuPratap Singh Rathore    ID : 2020A7PS1675P
        Name : Archaj Jain                  ID : 2020A7PS0072P
        Name : Siddharth Khandelwal         ID : 2020A7PS0098P
        Name : Rishi Rakesh Shrivastava     ID : 2020A7PS0108P

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parseTree.h"
#include "parserDef.h"
FILE *fp;
parseTreeNode createTree()
{
    parseTreeNode root = (parseTreeNode)malloc(sizeof(struct PARSETREENODE));
    root->parent = NULL;
    root->sibling = NULL;
    root->child = NULL;
    symbol new_symbol = (symbol)malloc(sizeof(struct SYMBOL));
    new_symbol->is_terminal = false;
    new_symbol->nt = program;
    root->s = new_symbol;
    root->tok = NULL;
    root->is_visited=false;
    root->syn_node=NULL;
    root->inh_node=NULL;
    root->scope[0]=0;
    root->scope[1]=0;
    root->type_inh=epsilon;
    root->type_syn=epsilon;
    return root;
}
void addRuleInTree(parseTreeNode treeNode, dlinkedlist list)
{
    if (treeNode == NULL)
    {
        printf("INVALID TREE NODE\n");
        return;
    }
    dllnode listNode = list->head;

    parseTreeNode prevTreeNode = NULL;

    bool firstChild = true;

    while (listNode)
    {
        parseTreeNode currentTreeNode = createTree();
        if (firstChild)
        {
            firstChild = false;
            treeNode->child = currentTreeNode;
        }
        currentTreeNode->parent = treeNode;
        currentTreeNode->prevSibling = prevTreeNode;
        currentTreeNode->s = listNode->val;
        if (prevTreeNode != NULL)
        {
            prevTreeNode->sibling = currentTreeNode;
        }
        prevTreeNode = currentTreeNode;
        listNode = listNode->next;
    }
}
void addTokenTonode(parseTreeNode treeNode, Token currtoken)
{
    TOK t = (TOK)malloc(sizeof(Token));
    *t = currtoken;
    treeNode->tok = t;
}

void printParseTree(parseTreeNode treeNode)
{
    if (treeNode == NULL)
        return;
    printParseTree(treeNode->child);
    if (treeNode->child == NULL && treeNode->tok == NULL)
    {
        if (!treeNode->s->is_terminal || treeNode->s->t != epsilon)
        {
            return;
        }
    }

    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        // leaf Node
        TOK currtoken = treeNode->tok;
        if (currtoken->token_type == TK_NUM)
        {
            printf("    %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            printf("    %-15lf", currtoken->lex.decimal);
        }
        else
        {
            printf("     %-15s", currtoken->lex.value);
        }
    }
    else
    {
        printf("       ----       ");
    }
    // 2 start
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        printf("  %-9d", treeNode->tok->line_no);
    }
    else
    {
        printf("     -     ");
    }
    // 2 end
    // 3 start
    if (treeNode->s->is_terminal)
    {
        printf("     %-15s ", terminal_str[treeNode->s->t]);
    }
    else
    {
        printf("          -          ");
    }
    // // //3 end
    // //4 starts
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        // leaf Node
        TOK currtoken = treeNode->tok;
        if (currtoken->token_type == TK_NUM)
        {
            printf("     %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            printf("     %-15lf", currtoken->lex.decimal);
        }
        else
        {
            printf("          -          ");
        }
    }
    else
    {
        printf("          -          ");
    }
    //  //4 end
    // // 5 start
    if (treeNode->parent == NULL)
    {

        printf("      ROOT         ");
    }
    else
    {
        if (treeNode->parent->s->is_terminal)
        {
            printf("%-20s", terminal_str[treeNode->parent->s->t]);
        }
        else
        {
            printf("%-20s", nonterminal_str[treeNode->parent->s->nt]);
        }
    }
    // // 5 end
    // // // 6 start
    if (treeNode->child == NULL)
    {
        printf("        YES        ");
    }

    else
    {
        printf("          NO       ");
    }
    if (!treeNode->s->is_terminal)
    {
        printf("    %-21s", nonterminal_str[treeNode->s->nt]);
    }
    else
    {
        printf("      -          ");
    }
    printf("\n");

    parseTreeNode temp = treeNode->child;
    while (temp != NULL)
    {
        printParseTree(temp->sibling);
        temp = temp->sibling;
    }
}

void printAST(parseTreeNode treeNode)
{
    if (treeNode == NULL)
        return;
    printAST(treeNode->child);
    if(treeNode->s == NULL || treeNode->tok == NULL){
        printf("%s", ast_strings[treeNode->ast_name]);
        printf("\n");
        
    }
    else{
    if (treeNode->child == NULL && treeNode->tok == NULL)
    {
        if (!treeNode->s->is_terminal || treeNode->s->t != epsilon)
        {
            return;
        }
    }

    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        // leaf Node
        TOK currtoken = treeNode->tok;
        if (currtoken->token_type == TK_NUM)
        {
            printf("    %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            printf("    %-15lf", currtoken->lex.decimal);
        }
        else
        {
            printf("     %-15s", currtoken->lex.value);
        }
    }
    else
    {
        printf("       ----       ");
    }
    // 2 start
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        printf("  %-9d", treeNode->tok->line_no);
    }
    else
    {
        printf("     -     ");
    }
    // 2 end
    // 3 start
    if (treeNode->s->is_terminal)
    {
        printf("     %-15s ", terminal_str[treeNode->s->t]);
    }
    else
    {
        printf("          -          ");
    }
    // // //3 end
    // //4 starts
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        // leaf Node
        TOK currtoken = treeNode->tok;
        if (currtoken->token_type == TK_NUM)
        {
            printf("     %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            printf("     %-15lf", currtoken->lex.decimal);
        }
        else
        {
            printf("          -          ");
        }
    }
    else
    {
        printf("          -          ");
    }
    //  //4 end
    // // 5 start
    if (treeNode->parent == NULL)
    {

        printf("      ROOT         ");
    }
    else
    {
        if (treeNode->parent->s->is_terminal)
        {
            printf("%-20s", terminal_str[treeNode->parent->s->t]);
        }
        else
        {
            printf("%-20s", nonterminal_str[treeNode->parent->s->nt]);
        }
    }
    // // 5 end
    // // // 6 start
    if (treeNode->child == NULL)
    {
        printf("        YES        ");
    }

    else
    {
        printf("          NO       ");
    }
    if (!treeNode->s->is_terminal)
    {
        printf("    %-21s", nonterminal_str[treeNode->s->nt]);
    }
    else
    {
        printf("      -          ");
    }
    printf("\n");
    }
    parseTreeNode temp = treeNode->child;
    while (temp != NULL)
    {
        printAST(temp->sibling);
        temp = temp->sibling;
    }
}
void openparsetreefile(FILE *f)
{
    fp = f;
}


void freeParseTree(parseTreeNode treeNode){
    if(treeNode == NULL){
        return ; 
    }
    parseTreeNode child = treeNode->child ; 
    parseTreeNode sibling = treeNode->sibling ;

    freeParseTree(child) ; 
    freeParseTree(sibling) ; 
    child=NULL;
    sibling=NULL;
    free(treeNode) ; 
}
