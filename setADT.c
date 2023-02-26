#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parserDef.h"
#include "setADT.h"

void initSet(bool set[]){
    for(int i = 0; i<num_terminals; i++){
        set[i] = false; 
    }
}

void insertElement(bool set[]  , terminal t){
    set[t] = true; 
}


void deleteElement(bool  set[] , terminal t){
    set[t] = false ; 
}

bool findInSet(bool set[] , terminal t){
    return set[t] ; 
}

void setUnion(bool s1[] , bool s2[] , bool dest[]){
    for(int i = 0 ; i<num_terminals ; i++){
        dest[i] = (s1[i] || s2[i]) ;
    }
}

void setIntersection(bool s1[] , bool s2[] , bool dest[]){
    for(int i = 0 ; i<num_terminals ; i++){
        dest[i] = (s1[i] && s2[i]) ;
    }
}


