#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parseTree.h"
#include "parserDef.h"
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
        printf("%d,",currentTreeNode->s->is_terminal);
        if(prevTreeNode != NULL){
            prevTreeNode->sibling = currentTreeNode ;
        }
        prevTreeNode = currentTreeNode ; 
        listNode = listNode->next ;
    }
    // printf("%d,",treeNode->child->s->is_terminal);
    printf("\n");
} 
void addTokenTonode(parseTreeNode treeNode,Token currtoken){
    TOK t=(TOK)malloc(sizeof(Token));
    *t=currtoken;
    treeNode->tok=t;
}
void printParseTree(parseTreeNode treeNode ){

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
    // 1
    // printf("%d\n",treeNode->s->is_terminal);
    if(treeNode->child==NULL&&treeNode->s->t!=epsilon){
        //leaf Node
        TOK currtoken= treeNode->tok;
        if (currtoken->token_type==TK_NUM)
        {
            printf("LEXEME => %lld",currtoken->lex.integer);
        }
        else if (currtoken->token_type==TK_RNUM)
        {
            printf("LEXEME => %lf",currtoken->lex.decimal);
        }
        else
        {
            //  printf("%s\n", current_token.token_type);
            printf("LEXEME => %s",currtoken->lex.value);
        }
    }
    else{
        printf("----");
    }
    // 2 start
    if(treeNode->child==NULL&&treeNode->s->t!=epsilon){
        printf("Lineno=%d",treeNode->tok->line_no);
    }
    else{
        printf("-");
    }
    // 2 end
    // 3 start
    if(treeNode->s->is_terminal){
        printf("%s",terminal_str[treeNode->s->t]);
    }
    else{
        printf("-");
    }
    // //3 end
    // //4 start
    if(treeNode->child==NULL&&treeNode->s->t!=epsilon){
        //leaf Node
        TOK currtoken= treeNode->tok;
        if (currtoken->token_type==TK_NUM)
        {
            printf("%d",currtoken->lex.integer);
        }
        else if (currtoken->token_type==TK_RNUM)
        {
            printf("%f",currtoken->lex.decimal);
        }
        else
        {
            //  printf("%s\n", current_token.token_type);
            printf("-");
        }
    }
    else{
        printf("-");
    }
     //4 end
    // 5 start
    if(treeNode->parent == NULL){

        printf("PARENT = ROOT ,") ;
    }
    else{
        if(treeNode->parent->s->is_terminal){
            printf("PARENT = %s" ,terminal_str[treeNode->parent->s->t]) ; 
        }
        else{
            printf("PARENT = %s" ,nonterminal_str[treeNode->parent->s->nt]); 
        }
    }
    // 5 end
    // // 6 start
    if(treeNode->child == NULL){
        printf("IS LEAF NODE := YES");
    }

    else{
        printf("IS LEAF NODE := NO");
    }
    if(!treeNode->s->is_terminal){
        printf("%s",nonterminal_str[treeNode->s->nt]);
    }
    else{
        printf("-");
    }
    printf("\n"); 
    printParseTree(treeNode->child) ;
    parseTreeNode temp = treeNode->child ;
    while(temp!=NULL){
        printParseTree(temp->sibling) ;
        temp = temp->sibling;
    }
}




