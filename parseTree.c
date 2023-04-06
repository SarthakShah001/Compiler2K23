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
            fprintf(fp, "    %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            fprintf(fp, "    %-15lf", currtoken->lex.decimal);
        }
        else
        {
            fprintf(fp, "     %-15s", currtoken->lex.value);
        }
    }
    else
    {
        fprintf(fp, "       ----       ");
    }
    // 2 start
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        fprintf(fp, "  %-9d", treeNode->tok->line_no);
    }
    else
    {
        fprintf(fp, "     -     ");
    }
    // 2 end
    // 3 start
    if (treeNode->s->is_terminal)
    {
        fprintf(fp, "     %-15s ", terminal_str[treeNode->s->t]);
    }
    else
    {
        fprintf(fp, "          -          ");
    }
    // // //3 end
    // //4 starts
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        // leaf Node
        TOK currtoken = treeNode->tok;
        if (currtoken->token_type == TK_NUM)
        {
            fprintf(fp, "     %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            fprintf(fp, "     %-15lf", currtoken->lex.decimal);
        }
        else
        {
            fprintf(fp, "          -          ");
        }
    }
    else
    {
        fprintf(fp, "          -          ");
    }
    //  //4 end
    // // 5 start
    if (treeNode->parent == NULL)
    {

        fprintf(fp, "      ROOT         ");
    }
    else
    {
        if (treeNode->parent->s->is_terminal)
        {
            fprintf(fp, "%-20s", terminal_str[treeNode->parent->s->t]);
        }
        else
        {
            fprintf(fp, "%-20s", nonterminal_str[treeNode->parent->s->nt]);
        }
    }
    // // 5 end
    // // // 6 start
    if (treeNode->child == NULL)
    {
        fprintf(fp, "        YES        ");
    }

    else
    {
        fprintf(fp, "          NO       ");
    }
    if (!treeNode->s->is_terminal)
    {
        fprintf(fp, "    %-21s", nonterminal_str[treeNode->s->nt]);
    }
    else
    {
        fprintf(fp, "      -          ");
    }
    fprintf(fp, "\n");

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
        fprintf(fp, "%s", ast_strings[treeNode->ast_name]);
        fprintf(fp, "\n");
        
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
            fprintf(fp, "    %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            fprintf(fp, "    %-15lf", currtoken->lex.decimal);
        }
        else
        {
            fprintf(fp, "     %-15s", currtoken->lex.value);
        }
    }
    else
    {
        fprintf(fp, "       ----       ");
    }
    // 2 start
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        fprintf(fp, "  %-9d", treeNode->tok->line_no);
    }
    else
    {
        fprintf(fp, "     -     ");
    }
    // 2 end
    // 3 start
    if (treeNode->s->is_terminal)
    {
        fprintf(fp, "     %-15s ", terminal_str[treeNode->s->t]);
    }
    else
    {
        fprintf(fp, "          -          ");
    }
    // // //3 end
    // //4 starts
    if (treeNode->child == NULL && treeNode->s->t != epsilon)
    {
        // leaf Node
        TOK currtoken = treeNode->tok;
        if (currtoken->token_type == TK_NUM)
        {
            fprintf(fp, "     %-15lld", currtoken->lex.integer);
        }
        else if (currtoken->token_type == TK_RNUM)
        {
            fprintf(fp, "     %-15lf", currtoken->lex.decimal);
        }
        else
        {
            fprintf(fp, "          -          ");
        }
    }
    else
    {
        fprintf(fp, "          -          ");
    }
    //  //4 end
    // // 5 start
    if (treeNode->parent == NULL)
    {

        fprintf(fp, "      ROOT         ");
    }
    else
    {
        if (treeNode->parent->s->is_terminal)
        {
            fprintf(fp, "%-20s", terminal_str[treeNode->parent->s->t]);
        }
        else
        {
            fprintf(fp, "%-20s", nonterminal_str[treeNode->parent->s->nt]);
        }
    }
    // // 5 end
    // // // 6 start
    if (treeNode->child == NULL)
    {
        fprintf(fp, "        YES        ");
    }

    else
    {
        fprintf(fp, "          NO       ");
    }
    if (!treeNode->s->is_terminal)
    {
        fprintf(fp, "    %-21s", nonterminal_str[treeNode->s->nt]);
    }
    else
    {
        fprintf(fp, "      -          ");
    }
    fprintf(fp, "\n");
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
