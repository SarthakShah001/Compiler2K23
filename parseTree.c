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
parseTreeNode createTree(){
    parseTreeNode root = (parseTreeNode)malloc(sizeof(struct PARSETREENODE));
    root->parent = NULL; 
    root->sibling = NULL; 
    root->child=NULL;
    symbol new_symbol = (symbol) malloc(sizeof(struct SYMBOL));
    new_symbol->is_terminal = false; 
    new_symbol->nt = program; 
    root->s = new_symbol;
    root->tok=NULL;
    return root;
}
void addRuleInTree(parseTreeNode treeNode , dlinkedlist list){
    if(treeNode == NULL){
        printf("INVALID TREE NODE\n") ;
        return; 
    }
    dllnode listNode = list->head ;

    parseTreeNode prevTreeNode = NULL ; 

    bool firstChild = true ; 

    while(listNode){
        parseTreeNode currentTreeNode = createTree();
        if(firstChild){
            firstChild = false; 
            treeNode->child=currentTreeNode; 
        }
        currentTreeNode->parent = treeNode ; 
        currentTreeNode->prevSibling = prevTreeNode ; 
        currentTreeNode->s=listNode->val;
        // printf("%d,",currentTreeNode->s->is_terminal);
        if(prevTreeNode != NULL){
            prevTreeNode->sibling = currentTreeNode ;
        }
        prevTreeNode = currentTreeNode ; 
        listNode = listNode->next ;
    }
    // printf("%d,",treeNode->child->s->is_terminal);
    // printf("\n");
} 
void addTokenTonode(parseTreeNode treeNode,Token currtoken){
    TOK t=(TOK)malloc(sizeof(Token));
    *t=currtoken;
    treeNode->tok=t;
}

void printParseTree(parseTreeNode treeNode){

    // TO PRINT THE FOLLOWING 
    // 1. LEXEME -> only if the node is a leaf -> children array = {} else ----
    // 2. LINE NUMBER
    // 3. TOKEN NAME
    // 4. VALUE IF NUMBER 
    // 5. PARENTNODESYMBOL
    // 6. IS_LEAF_NODE (YES/NO) 
    // 7. NODE_SYMBOL 
    // CurrLineNo of TreeNode is remaining
    if(treeNode == NULL) return ;
    printParseTree(treeNode->child);
    // 1
    // printf("%d\n",treeNode->s->is_terminal);
    if(treeNode->child==NULL&&treeNode->tok==NULL){
        if(!treeNode->s->is_terminal||treeNode->s->t!=epsilon){
        // printf("%s:",terminal_str[treeNode->s->t]);
        // printf("No token available for this terminal in parse tree\n");
        return;
        }
    }
        // fprintf(fp, "       LEXEME       ");
        // printf("       LEXEME       ");
        // fprintf(fp, "  LINE_NO  ");
        // printf("  LINE_NO  ");
        // fprintf(fp, "     TOKEN-TYPE      ");
        // printf("     TOKEN-TYPE      ");
        // fprintf(fp, "  VALUE if NUM/RNUM  ");
        // printf("  VALUE if NUM/RNUM  ");
        // fprintf(fp, "       PARENT       ");
        // printf("       PARENT       ");
        // fprintf(fp, "   IS A LEAF NODE  ");
        // printf("   IS A LEAF NODE  ");
        // fprintf(fp, "    NON-TERMINAL     ");
        // printf("    NON-TERMINAL     ");
        // fprintf(fp, "\n\n");
      // printf("\n\n");

    if(treeNode->child==NULL&&treeNode->s->t!=epsilon){
        //leaf Node
        TOK currtoken= treeNode->tok;
        if (currtoken->token_type==TK_NUM)
        {
            fprintf(fp,"    %-15lld",currtoken->lex.integer);
            // printf("%-20lld",currtoken->lex.integer);
        }
        else if (currtoken->token_type==TK_RNUM)
        {
            fprintf(fp,"    %-15lf",currtoken->lex.decimal);
            // printf("%-20lf",currtoken->lex.decimal);
        }
        else
        {
            //  printf("%s\n", current_token.token_type);
            // printf("%-20s",currtoken->lex.value);
            fprintf(fp,"     %-15s",currtoken->lex.value);
        }
    }
    else{
        fprintf(fp,"       ----       ");
        // printf("       ----       ");
    }
    // 2 start
    if(treeNode->child==NULL&&treeNode->s->t!=epsilon){
        fprintf(fp,"  %-9d",treeNode->tok->line_no);
        // printf("%11d",treeNode->tok->line_no);
    }
    else{
        fprintf(fp,"     -     ");
        // printf("     -     ");
    }
    // 2 end
    // 3 start
    if(treeNode->s->is_terminal){
        fprintf(fp,"     %-15s ",terminal_str[treeNode->s->t]);
        // printf("%-21s ",terminal_str[treeNode->s->t]);
    }
    else{
        fprintf(fp,"          -          ");
        // printf("          -          ");
    }
    // // //3 end
    // //4 starts
    if(treeNode->child==NULL&&treeNode->s->t!=epsilon){
        //leaf Node
        TOK currtoken= treeNode->tok;
        if (currtoken->token_type==TK_NUM)
        {
            fprintf(fp,"     %-15lld",currtoken->lex.integer);
            // printf("%-21lld",currtoken->lex.integer);
        }
        else if (currtoken->token_type==TK_RNUM)
        {
            fprintf(fp,"     %-15lf",currtoken->lex.decimal);
            // printf("%-21lf",currtoken->lex.decimal);
        }
        else
        {
            //  printf("%s\n", current_token.token_type);
            fprintf(fp,"          -          ");
            // printf("           -          ");
        }
    }
    else{
        fprintf(fp, "          -          ");
        // printf("           -          ");
    }
    //  //4 end
    // // 5 start
    if(treeNode->parent == NULL){

        fprintf(fp,"      ROOT         ") ;
        // printf("        ROOT      ,") ;
    }
    else{
        if(treeNode->parent->s->is_terminal){
            fprintf(fp,"%-20s" ,terminal_str[treeNode->parent->s->t]) ; 
            // printf("%-20s" ,terminal_str[treeNode->parent->s->t]) ; 
        }
        else{
            fprintf(fp,"%-20s" ,nonterminal_str[treeNode->parent->s->nt]); 
            // printf("%-20s" ,nonterminal_str[treeNode->parent->s->nt]); 
        }
    }
    // // 5 end
    // // // 6 start
    if(treeNode->child == NULL){
        fprintf(fp,"        YES        ");
        // printf("        YES        ");
    }

    else{
        fprintf(fp,"          NO       ");
        // printf("        NO         ");
    }
    // // 6 end
    // // 7 start
    if(!treeNode->s->is_terminal){
        fprintf(fp,"    %-21s",nonterminal_str[treeNode->s->nt]);
        // printf("%-21s",nonterminal_str[treeNode->s->nt]);
    }
    else{
        fprintf(fp,"      -          ");
        // printf("          -          ");
    }
    fprintf(fp,"\n"); 
    
    parseTreeNode temp = treeNode->child ;
    while(temp!=NULL){
        printParseTree(temp->sibling) ;
        temp = temp->sibling;
    }
}
void openparsetreefile(FILE *f){
fp=f;
}




