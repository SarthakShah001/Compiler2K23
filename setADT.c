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




