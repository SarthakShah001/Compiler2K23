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
#include "parserDef.h"
#include "setADT.h"

setNode initSet(){
    setNode set=(setNode)malloc(sizeof(struct SETNODE));
    for(int i = 0; i<num_terminals; i++){
        set->arr[i] = false; 
    }
    set->is_filled=false;
}

void insertElement(setNode set  , terminal t){
    set->arr[t] = true; 
}


void deleteElement(setNode  set , terminal t){
    set->arr[t] = false ; 
}

bool findInSet(setNode set , terminal t){
    return set->arr[t] ; 
}

void setUnion(setNode s1 , setNode s2 , setNode dest){
    for(int i = 0 ; i<num_terminals ; i++){
        dest->arr[i] = (s1->arr[i] || s2->arr[i]) ;
    }
}

void setIntersection(setNode s1 , setNode s2 , setNode dest){
    for(int i = 0 ; i<num_terminals ; i++){
        dest->arr[i] = (s1->arr[i] && s2->arr[i]) ;
    }
}

bool isSuperset(setNode s1, setNode s2){
    for(int i = 0; i<num_terminals; i++){
        if(s1->arr[i] == true && s2->arr[i] == false){
            return false;
        }
    }
    return true;
    
}

setNode compute_synchro_set(nonterminal nt){
    setNode s=initSet();
    setUnion(s,follow_set[nt],s);
    s->arr[TK_SEMICOLON]=true;
    // s->arr[TK_BC]=true;
    // s->arr[TK_BO]=true;
    // s->arr[TK_SQBC]=true;
    // s->arr[TK_SQBO]=true;
    s->arr[TK_END]=true;
    s->arr[TK_ENDDEF]=true;
    // s->arr[TK_COMMA]=true;
    return s;
}


