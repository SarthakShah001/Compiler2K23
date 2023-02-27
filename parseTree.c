#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parseTree.h"
#include "parserDef.h"

parseTreeNode createTree(){
    parseTreeNode root = (parseTreeNode)malloc(sizeof(struct PARSETREENODE));
    root->parent = NULL ; 
    root->sibling = NULL ; 
    root->childrenCount = 0 ; 
    symbol new_symbol = (symbol) malloc(sizeof(struct SYMBOL));
    new_symbol->is_terminal = false ; 
    new_symbol->nt = program ; 
    root->s = new_symbol ; 
    return root ; 
}

void addRuleInTree(parseTreeNode treeNode , dlinkedlist list){
    if(treeNode == NULL){
        printf("INVALID TREE NODE\n") ;
        return ; 
    }

    dllnode listNode = list->head ;

    parseTreeNode prevTreeNode = NULL ; 

    bool firstChild = true ; 

    while(listNode){
        parseTreeNode currentTreeNode = createTree();
        if(firstChild){
            firstChild = false ; 
            treeNode->childrenList[treeNode->childrenCount]=currentTreeNode; 
            treeNode->childrenCount++ ; 
        }
        currentTreeNode->parent = treeNode ; 
        currentTreeNode->prevSibling = prevTreeNode ; 
        currentTreeNode->s=listNode->val;
        if(prevTreeNode != NULL){
            prevTreeNode->sibling = currentTreeNode ;
        }
        prevTreeNode = currentTreeNode ; 
        listNode = listNode->next ;
    }

} 

void printParseTree(parseTreeNode treeNode ){

    // TO PRINT THE FOLLOWING 
    // 1. LEXEME -> only if the node is a leaf -> children array = {} else ----
    // 2. CURRENT NODE
    // 3. LINE NUMBER
    // 4. TOKEN NAME
    // 5. VALUE IF NUMBER 
    // 6. PARENTNODESYMBOL
    // 7. IS_LEAF_NODE (YES/NO) 
    // 8. NODE_SYMBOL 


    // CurrLineNo of TreeNode is remaining
    if(treeNode == NULL) return ;


    if(treeNode->parent == NULL){
        printf("PARENT = NULL ,") ;
    }
    else{
        if(treeNode->parent->s->is_terminal){
            printf("PARENT LEXEME TERMINAL %d ," , treeNode->s->t) ; 
        }
        else{
            printf("PARENT LEXEME NON TERMINAL %d ," , treeNode->s->nt); 
        }
    }

    if(treeNode->childrenCount == 0){
        printf("IS LEAF NODE := YES , ");
        if(treeNode->s->is_terminal){
            printf("LEXEME TERMINAL %d " , treeNode->s->t) ; 
        }
        else{
            printf("LEXEME NON TERMINAL %d " , treeNode->s->nt); 
        }
    }

    else{
        printf("IS LEAF NODE := NO , ");
        printf("LEXEME ---- ") ;
    }

    printf("\n"); 

    for(int i = 0 ; i<treeNode->childrenCount ; i++){
        printParseTree(treeNode->childrenList[i]) ;
        parseTreeNode temp = treeNode->childrenList[i] ;

        while(temp){
            printParseTree(temp->sibling) ;
            temp = temp->sibling;
        }
    }
    
}




