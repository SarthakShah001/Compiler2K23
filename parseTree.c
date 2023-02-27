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

// int main(){ 

//     return 0 ;  
// }




